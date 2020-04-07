// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "CppSymbolParser.h"

#include "Alias.h"
#include "Array.h"
#include "ConstType.h"
#include "ConstVolatileType.h"
#include "Constant.h"
#include "FunctionType.h"
#include "LValueReference.h"
#include "Namespace.h"
#include "Placeholder.h"
#include "PlaceholderType.h"
#include "Pointer.h"
#include "PrimitiveType.h"
#include "RValueReference.h"
#include "Scope.h"
#include "Subroutine.h"
#include "Template.h"
#include "TemplateDependantTemplateInstance.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
#include "Type.h"
#include "TypeOf.h"
#include "Variable.h"
#include "VolatileType.h"

#include <inttypes.h>
#include <phantom/detail/new.h>
#include <phantom/utils/ScopeExit.h>
#include <phantom/utils/SmallString.h>
#include <phantom/utils/StringView.h>

#pragma warning(disable : 4996)

namespace phantom
{
namespace lang
{
struct StringViewStream
{
    StringViewStream(StringView str) : str(str) {}

    char peek()
    {
        PHANTOM_ASSERT(!eof());
        return str.front();
    }
    char peek(size_t off)
    {
        PHANTOM_ASSERT(str.size() > off);
        return str[off];
    }
    char get()
    {
        PHANTOM_ASSERT(!eof());
        return str.dropFront();
    }
    bool eof() const { return str.empty(); }
    void ignore()
    {
        PHANTOM_ASSERT(!eof());
        str.dropFront();
    }

private:
    StringView str;
};

#define CPPSYMPARS_ON_EOF                                                                                              \
    if (InStream.eof())                                                                                                \
    {                                                                                                                  \
        CPPSYMPARS_ERROR_IF(contextStack.size() != 1,                                                                  \
                            CPPSYMPARS_IS_INSIDE_FUNC_SIGN                                                             \
                            ? "unterminated function name"                                                             \
                            : CPPSYMPARS_IS_INSIDE_TEMPLATE ? "unterminated template name"                             \
                                                            : "unterminated scope name");                              \
        {                                                                                                              \
            return CPPSYMPARS_SOLVE_MULTIPLE();                                                                        \
        }                                                                                                              \
    }
#define CPPSYMPARS_IGNORE_WHITE_SPACES                                                                                 \
    while (!InStream.eof() && ((c = InStream.peek()) == ' ' || c == '\t' || c == '\n' || c == '\r'))                   \
        InStream.ignore();
#define CPPSYMPARS_DEBUG_ENABLED true

#if CPPSYMPARS_DEBUG_ENABLED
#    define CPPSYMPARS_ASSERT(cond) PHANTOM_ASSERT(cond)
#else
#    define CPPSYMPARS_ASSERT(cond)
#endif

#define CPPSYMPARS_ERROR(str)                                                                                          \
    {                                                                                                                  \
        m_LastError = str;                                                                                             \
        error = true;                                                                                                  \
        return 0;                                                                                                      \
    }
#define CPPSYMPARS_ERROR_IF(cond, str)                                                                                 \
    if (cond)                                                                                                          \
    CPPSYMPARS_ERROR(str)
#define CPPSYMPARS_READ_AND_EXPECTS(c)                                                                                 \
    {                                                                                                                  \
        CPPSYMPARS_ERROR_IF(InStream.peek() != c, (StringBuffer("unexpected token ") + (char)InStream.peek()));        \
        InStream.get();                                                                                                \
    }
#define CPPSYMPARS_BACK_AFTER_VALUE                                                                                    \
    valueStack.pop_back();                                                                                             \
    if (valueStack.empty())                                                                                            \
        return true;                                                                                                   \
    if (valueStack.back()->isArray())                                                                                  \
        goto state_array_next_value;                                                                                   \
    else                                                                                                               \
        goto state_struct;

#define CPPSYMPARS_CASE_WHITE_SPACE                                                                                    \
    case ' ':                                                                                                          \
    case '\t':                                                                                                         \
    case '\r':                                                                                                         \
    case '\n'
#define CPPSYMPARS_CASE_VALUE_SEPARATOR                                                                                \
    CPPSYMPARS_CASE_WHITE_SPACE:                                                                                       \
    case '"':                                                                                                          \
    case ',':                                                                                                          \
    case ']':                                                                                                          \
    case '}'

#define CPPSYMPARS_IS_a_f_A_F(c) (((c) | 0x20) >= 'a' && ((c) | 0x20) <= 'f')
#define CPPSYMPARS_IS_a_z_A_Z(c) (((c) | 0x20) >= 'a' && ((c) | 0x20) <= 'z')
#define CPPSYMPARS_IS_0_9(c) ((c) >= '0' && (c) <= '9')
#define CPPSYMPARS_IS_a_z_A_Z_0_9(c) (CPPSYMPARS_IS_a_z_A_Z(c) || CPPSYMPARS_IS_0_9(c))
#define CPPSYMPARS_IS_IDENTIFIER_CHAR(c) (CPPSYMPARS_IS_a_z_A_Z_0_9(c) || (c) == '_')
#define CPPSYMPARS_IS_FIRST_IDENTIFIER_CHAR(c) (CPPSYMPARS_IS_a_z_A_Z(c) || (c) == '_')

#define CPPSYMPARS_STACK_SIZE 128

#define CPPSYMPARS_SCOPE (contextStack.back().scope)
#define CPPSYMPARS_IDENTIFIER (contextStack.back().identifier)
#define CPPSYMPARS_SOLVE_SCOPE (CPPSYMPARS_SCOPE ? CPPSYMPARS_SCOPE : a_pScope)
#define CPPSYMPARS_QUALIFIERS (contextStack.back().prefixQualifiers)
#define CPPSYMPARS_TEMPLATE_ARGS (contextStack.back().templateArgs)
#define CPPSYMPARS_FUNC_ARGS (contextStack.back().funcArgs)
#define CPPSYMPARS_PUSH_TEMPLATE_ARG()                                                                                 \
    {                                                                                                                  \
        CPPSYMPARS_IS_TEMPLATE() = true;                                                                               \
        contextStack.emplace_back();                                                                                   \
    }
#define CPPSYMPARS_POP_TEMPLATE_ARG()                                                                                  \
    {                                                                                                                  \
        auto pSolved = CPPSYMPARS_SOLVE();                                                                             \
        CPPSYMPARS_SOLVE_TEST_ERROR();                                                                                 \
        contextStack.pop_back();                                                                                       \
        CPPSYMPARS_TEMPLATE_ARGS.push_back(pSolved);                                                                   \
    }
#define CPPSYMPARS_PUSH_FUNC_ARG()                                                                                     \
    {                                                                                                                  \
        contextStack.emplace_back();                                                                                   \
    }
#define CPPSYMPARS_POP_FUNC_ARG()                                                                                      \
    {                                                                                                                  \
        auto pSolved = CPPSYMPARS_SOLVE();                                                                             \
        CPPSYMPARS_SOLVE_TEST_ERROR();                                                                                 \
        Type* pSolvedType = pSolved->toType();                                                                         \
        if (pSolvedType == nullptr)                                                                                    \
        {                                                                                                              \
            CPPSYMPARS_ERROR("expected type, got " + CPPSYMPARS_IDENTIFIER);                                           \
        }                                                                                                              \
        contextStack.pop_back();                                                                                       \
        CPPSYMPARS_FUNC_ARGS.push_back(pSolvedType);                                                                   \
    }
#define CPPSYMPARS_SOLVE() contextStack.back().Solve(a_pScope, a_pLastError)
#define CPPSYMPARS_SOLVE_MULTIPLE() contextStack.back().Solve(a_pScope, a_Symbols, a_pLastError)
#define CPPSYMPARS_IS_FUNC() contextStack.back().isFunc
#define CPPSYMPARS_IS_FUNC_TYPE() contextStack.back().isFuncType
#define CPPSYMPARS_IS_TEMPLATE() contextStack.back().isTemplate
#define CPPSYMPARS_IS_INSIDE_FUNC_SIGN                                                                                 \
    (contextStack.size() >= 2 &&                                                                                       \
     (contextStack[contextStack.size() - 2].isFunc || contextStack[contextStack.size() - 2].isFuncType))
#define CPPSYMPARS_IS_INSIDE_TEMPLATE (contextStack.size() >= 2 && contextStack[contextStack.size() - 2].isTemplate)
#define CPPSYMPARS_SOLVE_TEST_ERROR()                                                                                  \
    if (pSolved == nullptr)                                                                                            \
    {                                                                                                                  \
        CPPSYMPARS_ERROR("cannot solve " + CPPSYMPARS_IDENTIFIER);                                                     \
    }

typedef SmallString<char, 32>  SymbolIdentifier;
typedef SmallVector<Type*, 20> FuncArgs;

static bool ReadInteger(StringViewStream& InStream, int64& a_Int)
{
    String buf;
    char   c = 0;
    while (!InStream.eof() && (c = InStream.peek()) && CPPSYMPARS_IS_0_9(c))
    {
        buf.push_back(c);
        InStream.ignore();
    }
    if (CPPSYMPARS_IS_FIRST_IDENTIFIER_CHAR(c))
        return false;
    return sscanf(buf.data(), "%" PRIi64, &a_Int) == 1;
}

static void ReadIdentifier(StringViewStream& InStream, SymbolIdentifier& currentIdentifier)
{
    char c;
    while ((c = InStream.peek()) &&
           (CPPSYMPARS_IS_FIRST_IDENTIFIER_CHAR(c) || (!currentIdentifier.empty() && CPPSYMPARS_IS_0_9(c))))
    {
        currentIdentifier.push_back(c);
        InStream.ignore();
        if (InStream.eof())
            break;
    }
}
static bool ReadKeyword(StringViewStream& InStream, const char* keyword)
{
    char kc = 0;
    while ((kc = *keyword++))
    {
        if (InStream.eof())
            return false;
        if (InStream.peek() == kc)
        {
            InStream.ignore();
        }
        else
        {
            return false;
        }
    }
    if (InStream.eof() || !CPPSYMPARS_IS_IDENTIFIER_CHAR(InStream.peek())) // next char after keyword is not a-zA-Z0-9
    {
        return true;
    }
    return false;
}
static bool ReadKeyword(StringViewStream& InStream, const char* keyword, SymbolIdentifier& currentIdentifier)
{
    char kc = 0;
    while ((kc = *keyword++))
    {
        if (InStream.eof())
            return false;
        if (InStream.peek() == kc)
        {
            currentIdentifier.push_back(kc);
            InStream.ignore();
        }
        else
        {
            ReadIdentifier(InStream, currentIdentifier);
            return false;
        }
    }
    if (InStream.eof() || !CPPSYMPARS_IS_IDENTIFIER_CHAR(InStream.peek())) // next char after keyword is not a-zA-Z0-9
    {
        currentIdentifier.clear();
        return true;
    }
    return false;
}

void InScopeText(LanguageElement* a_pScope, StringBuffer& a_Buf)
{
    a_Buf += " in scope '";
    a_Buf += a_pScope->getQualifiedDecoratedName();
    a_Buf += '\'';
}

Symbol* CppSymbolParser::parse(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError)
{
    Symbols symbols;
    if (parse(a_Text, symbols, a_pScope, a_pLastError))
    {
        if (symbols.size() == 1)
            return symbols.front();
        if (a_pLastError)
        {
            if (symbols.size() > 1)
            {
                *a_pLastError = "multiple overload exists for '";
                a_pLastError->append(a_Text.data(), a_Text.size());
                a_pLastError->append('\'');
                InScopeText(a_pScope, *a_pLastError);
                return nullptr;
            }
            *a_pLastError = "symbol '";
            a_pLastError->append(a_Text.data(), a_Text.size());
            *a_pLastError += "' not found";
            InScopeText(a_pScope, *a_pLastError);
        }
    }
    return nullptr;
}

bool CppSymbolParser::parse(StringView a_Text, Symbols& a_Symbols, LanguageElement* a_pScope,
                            StringBuffer* a_pLastError /*= nullptr*/)
{
    char             c = 0;
    StringViewStream InStream(a_Text);

    LanguageElements tempOrNotTemps;

    auto scopeExit = makeScopeExit([&]() {
        for (auto pElem : tempOrNotTemps)
            if (pElem->getOwner() == nullptr)
                PHANTOM_DELETE_DYN pElem;
    });

    struct Context
    {
        Symbol* Solve(LanguageElement* a_pScope, StringBuffer* a_LastError)
        {
            Symbols symbols;
            if (Solve(a_pScope, symbols, a_LastError))
            {
                if (symbols.size() == 1)
                {
                    if ((isUnsigned || isLong))
                    {
                        Type* pType = symbols.front()->asIntegralType();
                        if (!pType)
                        {
                            if (isUnsigned)
                                *a_LastError = "invalid 'unsigned' qualifier";
                            else
                                *a_LastError = "invalid 'long' qualifier";
                            return nullptr;
                        }
                        if (isLong)
                            pType = pType->longType();
                        if (isUnsigned)
                            pType = pType->unsignedType();
                        return pType;
                    }
                    return symbols.front();
                }
                if (a_LastError)
                {
                    if (symbols.size() > 1)
                    {
                        *a_LastError = "multiple overload exists for '" + identifier + "'";
                        InScopeText(a_pScope, *a_LastError);
                        return nullptr;
                    }
                    *a_LastError = "symbol '" + identifier + "' not found";
                    InScopeText(a_pScope, *a_LastError);
                }
            }
            return nullptr;
        }

        bool SolveQualifiedClass(Class* a_pClass, Symbols& a_Symbols, StringBuffer* a_LastError)
        {
            if (SolveQualifiedNoClass(a_pClass, a_Symbols, a_LastError))
                return true;
            bool found = false;
            for (Class* pBClass : a_pClass->getBaseClasses())
            {
                if (pBClass->getName() == identifier)
                {
                    if (isTemplate)
                    {
                        if (Template* pTemplate = pBClass->getTemplate())
                        {
                            TemplateSpecialization* pSpec = pTemplate->getTemplateSpecialization(templateArgs);
                            if (pSpec)
                            {
                                PHANTOM_ASSERT(pSpec->getTemplated());
                                a_Symbols.push_back(pSpec->getTemplated());
                                return true;
                            }
                            if (a_LastError)
                            {
                                *a_LastError =
                                "cannot find '" + identifier + "' template specialization with the given arguments";
                                InScopeText(pBClass, *a_LastError);
                            }
                        }
                        else if (a_LastError)
                        {
                            *a_LastError = "base class '" + identifier + "' is not a template";
                            InScopeText(pBClass, *a_LastError);
                        }
                        return false;
                    }
                    else if (!isFunc && !isFuncType)
                    {
                        a_Symbols.push_back(pBClass);
                        return true;
                    }
                }
                Symbols symbols;
                if (SolveQualifiedClass(pBClass, symbols, a_LastError))
                {
                    if (found)
                    {
                        if (a_LastError)
                            *a_LastError = "ambiguous access between multiple base class members";
                        return false;
                    }
                    a_Symbols.swap(symbols);
                    found = true;
                }
            }
            return found;
        }

        bool SolveQualified(LanguageElement* a_pScope, Symbols& a_Symbols, StringBuffer* a_LastError)
        {
            if (Class* pClass = a_pScope->asLanguageElement()->asClass())
            {
                return SolveQualifiedClass(pClass, a_Symbols, a_LastError);
            }
            return SolveQualifiedNoClass(a_pScope, a_Symbols, a_LastError);
        }
        bool SolveQualifiedNoClass(LanguageElement* a_pScope, Symbols& a_Symbols, StringBuffer* a_LastError)
        {
            LanguageElement* pSolveElement = a_pScope;
            Scope*           pSolveScope = a_pScope->asScope();
            if ((isFunc || isTemplate))
            {
                if (pSolveScope == nullptr)
                {
                    pSolveScope = pSolveElement->getEnclosingScope();
                    if (pSolveScope == nullptr)
                    {
                        if (a_LastError)
                            *a_LastError = "expected scope";
                        return false;
                    }
                }
                if (isFunc)
                {
                    Modifiers mod;
                    for (auto& qual_array : suffixQualifiers)
                    {
                        switch (qual_array.first)
                        {
                        case 'c': // const
                            if (mod.testFlag(Modifier::Const))
                            {
                                if (a_LastError)
                                    *a_LastError = "const already specified";
                                return false;
                            }
                            mod |= Modifier::Const;
                            break;
                        case 'v': // volatile
                            if (mod.testFlag(Modifier::Volatile))
                            {
                                if (a_LastError)
                                    *a_LastError = "volatile already specified";
                                return false;
                            }
                            mod |= Modifier::Volatile;
                            break;
                        case '&': // &
                            if (mod.testFlag(Modifier::LValueRef))
                            {
                                if (a_LastError)
                                    *a_LastError = "& l-value ref already specified";
                                return false;
                            }
                            mod |= Modifier::LValueRef;
                            break;
                        case '$': // &&
                            if (mod.testFlag(Modifier::RValueRef))
                            {
                                if (a_LastError)
                                    *a_LastError = "&& r-value ref already specified";
                                return false;
                            }
                            mod |= Modifier::RValueRef;
                            break;
                        default:
                            if (a_LastError)
                                *a_LastError = "unexpected array qualifier";
                            return false;
                        }
                    }
                    auto pFunc = pSolveScope->getSubroutine(identifier, funcArgs, mod);
                    if (pFunc)
                    {
                        a_Symbols.push_back(pFunc);
                        return true;
                    }
                    if (a_LastError)
                    {
                        *a_LastError = "cannot find '" + identifier + "' function name with the given arguments";
                        InScopeText(pSolveElement, *a_LastError);
                    }
                    return false;
                }
                else if (isTemplate)
                {
                    Template* pTemplate = pSolveScope->getTemplate(identifier);
                    if (pTemplate)
                    {
                        bool isTemplateDependant = false;
                        for (LanguageElement*& pArg : templateArgs)
                        {
                            if (TemplateParameter* pTP = pArg->asTemplateParameter())
                            {
                                isTemplateDependant = true;
                                pArg = pTP->getPlaceholder()->asSymbol();
                            }
                            else
                                isTemplateDependant |= pArg->isTemplateDependant();
                        }
                        if (isTemplateDependant)
                        {
                            if (templateArgs.size() <= pTemplate->getTemplateParameters().size())
                            {
                                TemplateDependantTemplateInstance* pInst =
                                PHANTOM_DEFERRED_NEW_EX(TemplateDependantTemplateInstance)(
                                pTemplate, templateArgs,
                                phantom::lang::detail::currentModule() ? PHANTOM_R_FLAG_NATIVE : PHANTOM_R_FLAG_NONE);
                                a_Symbols.push_back(pInst);
                                return true;
                            }
                            if (a_LastError)
                            {
                                *a_LastError = "too many arguments for template '" + identifier + "' signature";
                                InScopeText(pSolveElement, *a_LastError);
                            }
                            return false;
                        }

                        TemplateSpecialization* pSpec = pTemplate->getTemplateInstantiation(templateArgs);
                        if (pSpec)
                        {
                            PHANTOM_ASSERT(pSpec->getTemplated());
                            a_Symbols.push_back(pSpec->getTemplated());
                            return true;
                        }
                        else if (isTemplateDependant)
                        {
                            if (TemplateSpecialization* pEmptySpec = pTemplate->getEmptyTemplateSpecialization())
                            {
                                if (Symbol* pTemplated = pEmptySpec->getTemplated())
                                {
                                    if (Alias* pAlias = pTemplated->asAlias()) // alias template
                                    {
                                        if (TemplateDependantTemplateInstance* pTDInstance =
                                            phantom::Object::Cast<TemplateDependantTemplateInstance>(
                                            pAlias->getAliasedSymbol()))
                                        {
                                            if (Template* pAliasedTpl = pTDInstance->getTemplate())
                                            {
                                                LanguageElements finalArgs;
                                                for (auto dependantArg : pTDInstance->getArguments())
                                                {
                                                    if (dependantArg->isTemplateDependant()) // template
                                                                                             // dependant, we
                                                                                             // try to match
                                                                                             // template above
                                                    {
                                                        auto const& aliasTplPlaceholdersArgs =
                                                        pTemplate->getEmptyTemplateSpecialization()->getArguments();
                                                        for (size_t i = 0; i < aliasTplPlaceholdersArgs.size(); ++i)
                                                        {
                                                            if (aliasTplPlaceholdersArgs[i]->isSame(dependantArg))
                                                            {
                                                                finalArgs.push_back(templateArgs[i]);
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        finalArgs.push_back(dependantArg);
                                                    }
                                                }
                                                if ((pSpec = pAliasedTpl->getTemplateInstantiation(finalArgs)))
                                                {
                                                    PHANTOM_ASSERT(pSpec->getTemplated());
                                                    a_Symbols.push_back(pSpec->getTemplated());
                                                    return true;
                                                }
                                                else
                                                {
                                                    a_Symbols.push_back(
                                                    PHANTOM_DEFERRED_NEW_EX(TemplateDependantTemplateInstance)(
                                                    pTemplate, finalArgs,
                                                    phantom::lang::detail::currentModule() ? PHANTOM_R_FLAG_NATIVE
                                                                                           : PHANTOM_R_FLAG_NONE));
                                                    return true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (a_LastError)
                        {
                            *a_LastError =
                            "cannot find '" + identifier + "' template specialization with the given arguments";
                            InScopeText(pSolveElement, *a_LastError);
                        }
                        return false;
                    }
                    else
                    {
                        if (a_LastError)
                        {
                            *a_LastError = "cannot find template '" + identifier + "'";
                            InScopeText(pSolveElement, *a_LastError);
                            return false;
                        }
                    }
                    return false;
                }
            }
            if (pSolveScope)
            {
                pSolveScope->getScopedSymbolsWithName(identifier, a_Symbols);
            }
            else if (TemplateSignature* pTS = pSolveElement->asTemplateSignature())
            {
                if (TemplateParameter* pTP = pTS->getTemplateParameter(identifier))
                {
                    a_Symbols.push_back(pTP->getPlaceholder()->asSymbol());
                }
            }
            else
            {
                pSolveElement->getSymbolsWithName(identifier, a_Symbols);
            }
            if (prefixQualifiers || suffixQualifiers.size())
            {
                if (a_Symbols.size() != 1)
                {
                    if (a_LastError)
                    {
                        if (a_Symbols.size())
                            *a_LastError = "unexpected qualifiers";
                        else
                            *a_LastError = "unresolved symbol";
                    }
                    return false;
                }
                Type* pType = a_Symbols.front()->toType();
                if (!pType)
                {
                    if (TemplateParameter* pParam = a_Symbols.front()->asTemplateParameter())
                    {
                        pType = pParam->getPlaceholder()->asPlaceholderType();
                    }
                    if (!pType)
                    {
                        if (a_LastError)
                        {
                            *a_LastError = "unexpected qualifiers";
                        }
                        return false;
                    }
                }
                Type* pQType;
                if (prefixQualifiers & Modifier::Const)
                {
                    if ((pQType = pType->makeConst()) == nullptr)
                    {
                        if (a_LastError)
                        {
                            *a_LastError = "unexpected const";
                        }
                        return false;
                    }
                    pType = pQType;
                }
                if (prefixQualifiers & Modifier::Volatile)
                {
                    if ((pQType = pType->makeVolatile()) == nullptr)
                    {
                        if (a_LastError)
                        {
                            *a_LastError = "unexpected volatile";
                        }
                        return false;
                    }
                    pType = pQType;
                }
                for (auto& qual_array : suffixQualifiers)
                {
                    switch (qual_array.first)
                    {
                    case 'c': // const
                        if ((pQType = pType->makeConst()) == nullptr)
                        {
                            if (a_LastError)
                            {
                                *a_LastError = "unexpected const";
                            }
                            return false;
                        }
                        break;
                    case 'v': // volatile
                        if ((pQType = pType->makeVolatile()) == nullptr)
                        {
                            if (a_LastError)
                            {
                                *a_LastError = "unexpected volatile";
                            }
                            return false;
                        }
                        break;
                    case '*': // *
                        if ((pQType = pType->makePointer()) == nullptr)
                        {
                            if (a_LastError)
                            {
                                *a_LastError = "unexpected pointer";
                            }
                            return false;
                        }
                        break;
                    case '&': // &
                        if ((pQType = pType->makeLValueReference()) == nullptr)
                        {
                            if (a_LastError)
                            {
                                *a_LastError = "unexpected l-value reference";
                            }
                            return false;
                        }
                        break;
                    case '$': // &&
                        if ((pQType = pType->makeRValueReference()) == nullptr)
                        {
                            if (a_LastError)
                            {
                                *a_LastError = "unexpected r-value reference";
                            }
                            return false;
                        }
                        break;
                    case '\0': // &&
                        if ((pQType = pType->makeArray(qual_array.second)) == nullptr)
                        {
                            if (a_LastError)
                            {
                                *a_LastError = "unexpected array";
                            }
                            return false;
                        }
                        break;
                    default:
                        if (a_LastError)
                            *a_LastError = "unexpected array qualifier";
                        return false;
                    }
                    pType = pQType;
                }
                a_Symbols.front() = pType;
            }
            if (isFuncType)
            {
                if (a_Symbols.size() == 1)
                {
                    if (Type* pType = a_Symbols.front()->toType())
                    {
                        a_Symbols.front() = functionType(pType, funcArgs);
                        return true;
                    }
                }
            }
            return !a_Symbols.empty();
        }

        bool Solve(LanguageElement* a_pScope, Symbols& a_Symbols, StringBuffer* a_LastError)
        {
            if (constant)
            {
                a_Symbols.push_back(constant);
                return true;
            }
            bool qualified = scope != nullptr;
            if (qualified)
            {
                return SolveQualified(scope, a_Symbols, a_LastError);
            }
            while (a_pScope)
            {
                if (SolveQualified(a_pScope, a_Symbols, a_LastError))
                {
                    return true;
                }
                auto pNamingScope = a_pScope->getNamingScope();
                a_pScope =
                pNamingScope ? pNamingScope->asLanguageElement() : nullptr; // TODO create Scope::getParentScope()
            }
            return false;
        }
        LanguageElement*                     scope = nullptr;
        Constant*                            constant = nullptr;
        SymbolIdentifier                     identifier;
        LanguageElements                     templateArgs;
        FuncArgs                             funcArgs;
        int                                  prefixQualifiers = 0;
        SmallVector<std::pair<char, size_t>> suffixQualifiers;
        bool                                 isTemplate = false;
        bool                                 isFunc = false;
        bool                                 isFuncType = false;
        bool                                 isUnsigned = false;
        bool                                 isLong = false;
    };

    SmallVector<Context> contextStack;
    contextStack.emplace_back();

    bool error = false;

    /// const and volatile
state_prefix_qualifiers:
    CPPSYMPARS_IGNORE_WHITE_SPACES;
    switch (c = InStream.peek())
    {
    case 'c': // const or identifier
        CPPSYMPARS_ON_EOF;
        if (InStream.peek(1) == 'l')
        {
            if (ReadKeyword(InStream, "class", CPPSYMPARS_IDENTIFIER))
            {
                goto state_prefix_qualifiers;
            }
        }
        else if (ReadKeyword(InStream, "const", CPPSYMPARS_IDENTIFIER))
        {
            CPPSYMPARS_ERROR_IF(CPPSYMPARS_QUALIFIERS & PHANTOM_R_CONST, "const qualifier already specified");
            CPPSYMPARS_QUALIFIERS |= PHANTOM_R_CONST;
            goto state_prefix_qualifiers;
        }
        goto symbol_post_identifier;
    case 'v': // volatile or identifier
        if (ReadKeyword(InStream, "volatile", CPPSYMPARS_IDENTIFIER))
        {
            CPPSYMPARS_ERROR_IF(CPPSYMPARS_QUALIFIERS & PHANTOM_R_VOLATILE, "volatile qualifier already specified");
            CPPSYMPARS_QUALIFIERS |= PHANTOM_R_VOLATILE;
            goto state_prefix_qualifiers;
        }
        goto symbol_post_identifier;
    case 't': // typename or identifier
        if (ReadKeyword(InStream, "typename", CPPSYMPARS_IDENTIFIER))
        {
            goto state_prefix_qualifiers;
        }
        goto symbol_post_identifier;

    default:
        goto state_symbol_name;
    }

state_symbol_name:
    CPPSYMPARS_IGNORE_WHITE_SPACES;
    PHANTOM_ASSERT(CPPSYMPARS_IDENTIFIER.empty());
    ReadIdentifier(InStream, CPPSYMPARS_IDENTIFIER);

symbol_post_identifier:
    if ((CPPSYMPARS_IDENTIFIER == "true" || CPPSYMPARS_IDENTIFIER == "false" || CPPSYMPARS_IDENTIFIER.empty()))
    {
        Constant* pConstant = nullptr;
        if (CPPSYMPARS_IDENTIFIER.empty())
        {
            int64_t num;
            CPPSYMPARS_ERROR_IF(!ReadInteger(InStream, num), "expected identifier or constant");
            pConstant = Constant::Create(int(num));
            tempOrNotTemps.push_back(pConstant);
        }
        else
        {
            pConstant = Constant::Create(CPPSYMPARS_IDENTIFIER[0] == 't'); // true / false
        }
        if (phantom::lang::detail::currentModule())
            pConstant->addFlags(PHANTOM_R_FLAG_NATIVE);
        if (CPPSYMPARS_IS_INSIDE_TEMPLATE)
        {
            (contextStack.end() - 2)->templateArgs.push_back(pConstant);
            contextStack.pop_back();
            CPPSYMPARS_IGNORE_WHITE_SPACES;
            switch (c = InStream.peek())
            {
            case '>':
            {
                InStream.ignore();
                //                     auto pSolved = CPPSYMPARS_SOLVE();
                //                     CPPSYMPARS_SOLVE_TEST_ERROR();
                //                     CPPSYMPARS_IDENTIFIER.clear();
                //                     CPPSYMPARS_TEMPLATE_ARGS.clear();
                goto state_symbol_suffix;
            }
            case ',':
                InStream.ignore();
                CPPSYMPARS_PUSH_TEMPLATE_ARG();
                goto state_prefix_qualifiers;
            default:
                CPPSYMPARS_ERROR("expected character after constant");
            }
        }
        else
        {
            contextStack.back().constant = pConstant;
            goto state_post_symbol_name;
        }
    }
    else
    {
        CPPSYMPARS_ERROR_IF(CPPSYMPARS_IDENTIFIER.empty(), "expected identifier");
    }
    CPPSYMPARS_ON_EOF;

state_post_symbol_name:
    CPPSYMPARS_IGNORE_WHITE_SPACES;
    CPPSYMPARS_ON_EOF;
    PHANTOM_ASSERT(CPPSYMPARS_IDENTIFIER.size()); // already read by ReadKeyword
    {
        switch (c = InStream.peek())
        {
        case ':':
        {
            InStream.ignore();
            CPPSYMPARS_READ_AND_EXPECTS(':');
            CPPSYMPARS_ERROR_IF(CPPSYMPARS_IS_FUNC(),
                                "unexpected '::' after function, " + CPPSYMPARS_IDENTIFIER + " is not a valid scope");
            auto pSolved = CPPSYMPARS_SOLVE();
            CPPSYMPARS_SOLVE_TEST_ERROR();
            CPPSYMPARS_ERROR_IF(!(CPPSYMPARS_SCOPE = pSolved),
                                "unexpected '::', " + CPPSYMPARS_IDENTIFIER + " is not a valid scope");
            CPPSYMPARS_IDENTIFIER.clear();
            CPPSYMPARS_TEMPLATE_ARGS.clear();
            goto state_symbol_name;
        }

        case '<':
        {
            InStream.ignore();
            CPPSYMPARS_PUSH_TEMPLATE_ARG();
            goto state_prefix_qualifiers;
        }

        case '(':
        {
            InStream.ignore();
            if (CPPSYMPARS_IS_INSIDE_TEMPLATE)
                CPPSYMPARS_IS_FUNC_TYPE() = true;
            else
                CPPSYMPARS_IS_FUNC() = true;
            CPPSYMPARS_IGNORE_WHITE_SPACES;
            if (InStream.peek() == ')') // empty signature
            {
                InStream.ignore();
                goto state_symbol_suffix;
            }
            CPPSYMPARS_PUSH_FUNC_ARG();
            goto state_prefix_qualifiers;
        }

        case ')':
        {
            InStream.ignore();
            CPPSYMPARS_ERROR_IF(!CPPSYMPARS_IS_INSIDE_FUNC_SIGN, "unexpected ')' outside of function");
            CPPSYMPARS_POP_FUNC_ARG();
            goto state_symbol_suffix;
        }

        default:
        {
            if (isalpha(c) || c == '_')
            {
                if (CPPSYMPARS_IDENTIFIER == "unsigned")
                {
                    contextStack.back().isUnsigned = true;
                    contextStack.back().identifier.clear();
                    goto state_symbol_name;
                }
                else if (CPPSYMPARS_IDENTIFIER == "long")
                {
                    contextStack.back().isLong = true;
                    contextStack.back().identifier.clear();
                    goto state_symbol_name;
                }
            }
        }
        }
    }

state_symbol_suffix:
    CPPSYMPARS_IGNORE_WHITE_SPACES;
    CPPSYMPARS_ON_EOF;
    switch (c = InStream.peek())
    {
    case '>':
    {
        InStream.ignore();
        CPPSYMPARS_ERROR_IF(!CPPSYMPARS_IS_INSIDE_TEMPLATE, "unexpected '>' outside of template");
        CPPSYMPARS_POP_TEMPLATE_ARG();
        CPPSYMPARS_IGNORE_WHITE_SPACES;
        CPPSYMPARS_ON_EOF;
        goto state_symbol_suffix;
    }
    case ')':
    {
        goto state_post_symbol_name;
    }
    case ',':
    {
        InStream.ignore();
        if (CPPSYMPARS_IS_INSIDE_FUNC_SIGN)
        {
            CPPSYMPARS_POP_FUNC_ARG();
            CPPSYMPARS_PUSH_FUNC_ARG();
            goto state_prefix_qualifiers;
        }
        else if (CPPSYMPARS_IS_INSIDE_TEMPLATE)
        {
            CPPSYMPARS_POP_TEMPLATE_ARG();
            CPPSYMPARS_PUSH_TEMPLATE_ARG();
            goto state_prefix_qualifiers;
        }
        else
        {
            CPPSYMPARS_ERROR("unexpected ','");
        }
    }
    case 'c': // const ?
        CPPSYMPARS_ERROR_IF(!ReadKeyword(InStream, "const"), "unexpected identifier");
        contextStack.back().suffixQualifiers.push_back(std::make_pair('c', size_t(0)));
        goto state_symbol_suffix;

    case 'v': // volatile ?
        CPPSYMPARS_ERROR_IF(!ReadKeyword(InStream, "volatile"), "unexpected identifier");
        contextStack.back().suffixQualifiers.push_back(std::make_pair('v', size_t(0)));
        goto state_symbol_suffix;

    case '&': // ref
        InStream.ignore();
        if (!InStream.eof() && InStream.peek() == '&')
        {
            InStream.ignore();
            contextStack.back().suffixQualifiers.push_back(std::make_pair('$', size_t(0)));
        }
        else
        {
            contextStack.back().suffixQualifiers.push_back(std::make_pair('&', size_t(0)));
        }
        goto state_symbol_suffix;

    case '*': // ref
        InStream.ignore();
        contextStack.back().suffixQualifiers.push_back(std::make_pair('*', size_t(0)));
        goto state_symbol_suffix;

    case '[': // array
        InStream.ignore();
        CPPSYMPARS_IGNORE_WHITE_SPACES;
        if (CPPSYMPARS_IS_0_9(InStream.peek()))
        {
            int64 size;
            if (ReadInteger(InStream, size))
            {
                contextStack.back().suffixQualifiers.push_back(std::make_pair('\0', size_t(size)));
            }
            CPPSYMPARS_IGNORE_WHITE_SPACES;
            CPPSYMPARS_READ_AND_EXPECTS(']');
        }
        goto state_symbol_suffix;

    default:
        CPPSYMPARS_ERROR_IF(CPPSYMPARS_IDENTIFIER.size(), "unexpected identifier");
        goto state_symbol_name;
    }

    return 0;
}

} // namespace lang
} // namespace phantom
