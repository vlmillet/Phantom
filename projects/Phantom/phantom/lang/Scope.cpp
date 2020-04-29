// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Scope.h"

#include "Alias.h"
#include "AnonymousStruct.h"
#include "AnonymousUnion.h"
#include "Application.h"
#include "Constant.h"
#include "Function.h"
#include "Signature.h"
#include "Template.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
#include "Variable.h"

#include <phantom/detail/core_internal.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Variables               m_EmptyVariables;
static Constants               m_EmptyConstants;
static Functions               m_EmptyFunctions;
static Types                   m_EmptyTypes;
static Templates               m_EmptyTemplates;
static TemplateSpecializations m_EmptyTemplateSpecializations;
static Aliases                 m_EmptyAliases;
static AnonymousSections       m_EmptyAnonymousSections;

Scope::Scope(LanguageElement* a_pThisElement)
    : m_pThisElement(a_pThisElement),
      m_Functions(a_pThisElement),
      m_Variables(a_pThisElement),
      m_Constants(a_pThisElement),
      m_Types(a_pThisElement),
      m_Templates(a_pThisElement),
      m_TemplateSpecializations(a_pThisElement),
      m_AnonymousSections(a_pThisElement),
      m_Aliases(a_pThisElement)
{
}

Type* Scope::getType(StringView a_strName) const
{
    auto it = m_Types->begin();
    auto end = m_Types->end();
    for (; it != end; ++it)
    {
        if ((*it)->getName() == a_strName)
        {
            return *it;
        }
    }
    return nullptr;
}

Type* Scope::getTypeByGuid(uint) const
{
    //     if(m_pTypes)
    //     {
    //         auto it = m_Types->begin();
    //         auto end = m_Types->end();
    //         for(;it != end; ++it)
    //         {
    //             if((*it)->getGuid() == a_Guid) return *it;
    //         }
    //     }
    return nullptr;
}

Template* Scope::getTemplate(StringView a_strName) const
{
    auto it = m_Templates->begin();
    auto end = m_Templates->end();
    for (; it != end; ++it)
    {
        if ((*it)->getName() == a_strName)
        {
            return *it;
        }
    }
    return nullptr;
}

Template* Scope::getTemplate(size_t a_uiIndex) const
{
    return m_Templates[a_uiIndex];
}

TemplateSpecialization* Scope::getTemplateSpecialization(size_t a_uiIndex) const
{
    return m_TemplateSpecializations[a_uiIndex];
}

TemplateSpecialization* Scope::getTemplateSpecialization(StringView              a_strName,
                                                         const LanguageElements& a_Arguments) const
{
    for (auto p : getTemplateSpecializations())
    {
        if (p->getTemplate()->getName() == a_strName)
        {
            if (p->getArgumentCount() == a_Arguments.size())
            {
                bool ok = true;
                for (size_t i = 0; i < a_Arguments.size(); ++i)
                {
                    if (p->getArgument(i) != a_Arguments[i])
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                    return p;
            }
        }
    }
    Template* pTemplate = getTemplate(a_strName);
    if (pTemplate)
    {
        return pTemplate->getTemplateSpecialization(a_Arguments);
    }
    return nullptr;
}

void Scope::release(Types& out_types)
{
    while (!m_Types->empty())
    {
        out_types.push_back(m_Types->back());
        removeType(m_Types->back());
    }
}

void Scope::addType(Type* a_pType)
{
    onScopeSymbolAdded(a_pType);
}

void Scope::addFunction(Function* a_pFunction)
{
    onScopeSymbolAdded(a_pFunction);
}

void Scope::addVariable(Variable* a_pVariable)
{
    onScopeSymbolAdded(a_pVariable);
}

void Scope::addConstant(Constant* a_pConstant)
{
    onScopeSymbolAdded(a_pConstant);
}

void Scope::addTemplate(Template* a_pTemplate)
{
    onScopeSymbolAdded(a_pTemplate);
}

void Scope::addTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization)
{
    onScopeSymbolAdded(a_pTemplateSpecialization);
}

void Scope::addAlias(Alias* a_pAlias)
{
    onScopeSymbolAdded(a_pAlias);
}

Alias* Scope::addAlias(Symbol* a_pSymbol, StringView a_strAlias, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    PHANTOM_ASSERT(a_pSymbol);
    Alias* pAlias = PHANTOM_DEFERRED_NEW(Alias)(a_pSymbol, a_strAlias, a_Modifiers, a_uiFlags);
    addAlias(pAlias);
    return pAlias;
}

Template* Scope::addAliasTemplate(TemplateSignature* a_pSignature, StringView a_strAliasName,
                                  StringView a_TemplateDependantType, Modifiers a_Modifiers /*= 0*/,
                                  uint a_uiFlags /*= 0*/)
{
    m_pThisElement->addScopedElement(a_pSignature);
    Type* pType = Application::Get()->findCppType(a_TemplateDependantType, a_pSignature);
    m_pThisElement->removeScopedElement(a_pSignature);
    if (pType == nullptr)
    {
        return nullptr;
    }
    Template* pTemplate = PHANTOM_DEFERRED_NEW(Template)(a_pSignature, a_strAliasName, a_Modifiers, a_uiFlags);
    Alias*    pAlias = Alias::Create(pType, a_strAliasName, PHANTOM_R_NONE, a_uiFlags & PHANTOM_R_FLAG_NATIVE);
    pTemplate->getEmptyTemplateSpecialization()->setTemplated(pAlias);
    if (pType->getOwner() == nullptr) // template dependant
    {
        pAlias->addElement(pType);
    }
    addTemplate(pTemplate);
    return pTemplate;
}

Template* Scope::addAliasTemplate(StringView a_TemplateTypes, StringView a_TemplateParams, StringView a_strAliasName,
                                  StringView a_TemplateDependantType, Modifiers a_Modifiers /*= 0*/,
                                  uint a_uiFlags /*= 0*/)
{
    TemplateSignature* pTS =
    TemplateSignature::Parse(a_TemplateTypes, a_TemplateParams, asLanguageElement(), a_uiFlags & PHANTOM_R_FLAG_NATIVE);
    if (pTS == nullptr)
        return nullptr;
    Template* pTemplate = addAliasTemplate(pTS, a_strAliasName, a_TemplateDependantType, a_Modifiers, a_uiFlags);
    if (pTemplate == nullptr)
    {
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "remove leak here (not critical because happens only on errors)")
#endif
        return nullptr;
    }
    return pTemplate;
}

Alias* Scope::getAlias(size_t a_uiIndex) const
{
    return m_Aliases[a_uiIndex];
}

Alias* Scope::getAlias(StringView a_strName) const
{
    for (auto it = m_Aliases->begin(); it != m_Aliases->end(); ++it)
    {
        if ((*it)->getName() == a_strName)
            return *it;
    }
    return nullptr;
}

LanguageElement* Scope::getAliased(StringView a_strAlias) const
{
    Alias* pAlias = getAlias(a_strAlias);
    return pAlias ? pAlias->getAliasedSymbol() : nullptr;
}

void Scope::removeAlias(Alias* a_pAlias)
{
    onScopeSymbolRemoving(a_pAlias);
}

void Scope::removeType(Type* a_pType)
{
    onScopeSymbolRemoving(a_pType);
}

void Scope::removeFunction(Function* a_pFunction)
{
    onScopeSymbolRemoving(a_pFunction);
}

void Scope::removeVariable(Variable* a_pVariable)
{
    onScopeSymbolRemoving(a_pVariable);
}

void Scope::removeConstant(Constant* a_pConstant)
{
    onScopeSymbolRemoving(a_pConstant);
}

void Scope::removeTemplate(Template* a_pTemplate)
{
    onScopeSymbolRemoving(a_pTemplate);
}

void Scope::removeTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization)
{
    onScopeSymbolRemoving(a_pTemplateSpecialization);
}

PrimitiveType* Scope::getPrimitiveType(StringView a_strName) const
{
    Type* pType = getType(a_strName);
    return pType ? pType->asPrimitiveType() : nullptr;
}

Class* Scope::getClass(size_t index) const
{
    size_t i = 0;
    for (auto it = m_Types->begin(); it != m_Types->end(); ++it)
    {
        if ((*it)->asClass())
        {
            if (i == index)
                return static_cast<Class*>(*it);
            ++i;
        }
    }
    return nullptr;
}

Class* Scope::getClass(StringView a_strName) const
{
    Type* pType = getType(a_strName);
    return pType ? pType->asClass() : nullptr;
}

size_t Scope::getClassCount() const
{
    size_t i = 0;
    for (auto it = m_Types->begin(); it != m_Types->end(); ++it)
    {
        if ((*it)->asClass())
        {
            ++i;
        }
    }
    return i;
}

void Scope::scopedElementAdded(LanguageElement* a_pElement)
{
    if (Type* pType = a_pElement->asType())
    {
        if (a_pElement->asClassType() || a_pElement->asEnum())
        {
            m_Types->push_back(pType);
        }
    }
    else if (Alias* pAlias = a_pElement->asAlias())
    {
        m_Aliases->push_back(pAlias);
    }
    else if (Variable* pVar = a_pElement->asVariable())
    {
        m_Variables->push_back(pVar);
    }
    else if (Constant* pCst = a_pElement->asConstant())
    {
        m_Constants->push_back(pCst);
    }
    else if (Function* pFunc = a_pElement->asFunction())
    {
        PHANTOM_ASSERT(acceptsSubroutine(pFunc));
        m_Functions->push_back(pFunc);
    }
    else if (Template* pTPL = a_pElement->asTemplate())
    {
        m_Templates->push_back(pTPL);
    }
    else if (TemplateSpecialization* pTemplateSpecialization = a_pElement->asTemplateSpecialization())
    {
        PHANTOM_ASSERT(pTemplateSpecialization->getTemplate(),
                       "template specialization must be declared in same scope as its specialized template");
        m_TemplateSpecializations->push_back(pTemplateSpecialization);
    }
    else if (AnonymousSection* pAS = a_pElement->asAnonymousSection())
    {
        m_AnonymousSections->push_back(pAS);
    }
}

void Scope::scopedElementRemoved(LanguageElement* a_pElement)
{
    if (a_pElement->asType())
    {
        if (m_Types.m_data)
            for (auto it = m_Types.m_data->begin(); it != m_Types.m_data->end(); ++it)
            {
                Type* pType = *it;
                if (pType == a_pElement)
                {
                    m_Types.m_data->erase(it);
                    return;
                }
            }
    }
    else if (a_pElement->asAlias())
        for (auto it = m_Aliases->begin(); it != m_Aliases->end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_Aliases->erase(it);
                return;
            }
        }
    else if (a_pElement->asVariable())
        for (auto it = m_Variables->begin(); it != m_Variables->end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_Variables->erase(it);
                return;
            }
        }
    else if (a_pElement->asConstant())
        for (auto it = m_Constants->begin(); it != m_Constants->end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_Constants->erase(it);
                return;
            }
        }
    else if (a_pElement->asFunction())
        for (auto it = m_Functions->begin(); it != m_Functions->end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_Functions->erase(it);
                return;
            }
        }
    else if (a_pElement->asTemplate())
        for (auto it = m_Templates->begin(); it != m_Templates->end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_Templates->erase(it);
                return;
            }
        }
    else if (a_pElement->asTemplateSpecialization())
        for (auto it = m_TemplateSpecializations->begin(); it != m_TemplateSpecializations->end(); ++it)
        {
            TemplateSpecialization* pTemplateSpecialization = static_cast<TemplateSpecialization*>(*it);
            if (pTemplateSpecialization == a_pElement)
            {
                m_TemplateSpecializations->erase(it);
                return;
            }
        }
    else if (a_pElement->asAnonymousSection())
        for (auto it = m_AnonymousSections->begin(); it != m_AnonymousSections->end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_AnonymousSections->erase(it);
                return;
            }
        }
}

Function* Scope::getFunction(StringView a_strName, TypesView a_Types) const
{
    auto it = m_Functions->begin();
    auto end = m_Functions->end();
    for (; it != end; ++it)
    {
        Function* pFunction = static_cast<Function*>((*it));
        if (pFunction->matches(a_strName, a_Types))
        {
            return pFunction;
        }
    }
    return nullptr;
}

Function* Scope::getFunction(size_t a_uiIndex) const
{
    return m_Functions[a_uiIndex];
}

void Scope::findFunctions(Functions& a_Functions, StringView a_Name /*= ""*/,
                          const Types* a_pParameterTypes /*= nullptr*/, Type* a_pReturnType /*= nullptr*/) const
{
    for (auto f : *m_Functions)
    {
        if ((a_Name.empty() || f->getName() == a_Name) &&
            (a_pReturnType == nullptr || f->getReturnType() == a_pReturnType) &&
            (a_pParameterTypes == nullptr || f->getSignature()->matches(*a_pParameterTypes)))
        {
            a_Functions.push_back(f);
        }
    }
}

Variable* Scope::getVariable(size_t a_uiIndex) const
{
    return m_Variables[a_uiIndex];
}

Variable* Scope::getVariable(StringView a_strName) const
{
    auto it = m_Variables->begin();
    auto end = m_Variables->end();
    for (; it != end; ++it)
    {
        Variable* pVariable = static_cast<Variable*>((*it));
        if (pVariable->getName() == a_strName)
            return pVariable;
    }
    return nullptr;
}

Constant* Scope::getConstant(size_t a_uiIndex) const
{
    return m_Constants[a_uiIndex];
}

Constant* Scope::getConstant(StringView a_strName) const
{
    auto it = m_Constants->begin();
    auto end = m_Constants->end();
    for (; it != end; ++it)
    {
        Constant* pConstant = static_cast<Constant*>((*it));
        if (pConstant->getName() == a_strName)
            return pConstant;
    }
    return nullptr;
}

void Scope::addSubroutine(Subroutine* a_pSubroutine)
{
    PHANTOM_ASSERT(a_pSubroutine);
    if (a_pSubroutine->asFunction())
        addFunction(a_pSubroutine->asFunction());
    else
        PHANTOM_ASSERT_NO_IMPL();
}

Subroutine* Scope::getSubroutine(StringView a_strName, TypesView a_ParameterTypes, Modifiers a_Modifiers) const
{
    PHANTOM_ASSERT(a_Modifiers == (a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK));
    if (a_Modifiers == 0)
        return getFunction(a_strName, a_ParameterTypes);
    return nullptr;
}

Subroutine* Scope::getSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_ParameterTypes,
                                 Modifiers a_Modifiers /*= 0*/) const
{
    Subroutine* pSubroutine = getSubroutine(a_strName, a_ParameterTypes, a_Modifiers);
    if (pSubroutine && pSubroutine->getReturnType() && pSubroutine->getReturnType()->isSame(a_pReturnType))
        return pSubroutine;
    return nullptr;
}

bool Scope::acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types, Modifiers, uint,
                              Subroutines* a_pOutConflictingSubroutines /*= nullptr*/) const
{
    bool bResult = true;
    auto it = m_Functions->begin();
    auto end = m_Functions->end();
    for (; it != end; ++it)
    {
        Function*          pFunction = *it;
        ESignatureRelation eOR = pFunction->getSignatureRelationWith(
        a_pReturnType, a_strName, a_Types,
        0); // 0 because static functions consider const and non-const as conflicting
        if (eOR != e_SignatureRelation_None)
        {
            if (a_pOutConflictingSubroutines)
            {
                a_pOutConflictingSubroutines->push_back(pFunction);
                bResult = false;
            }
            else
                return false;
        }
    }
    return bResult;
}

bool Scope::acceptsSubroutine(Subroutine* a_pSubroutine, Subroutines* a_pOutConflictingSubroutines) const
{
    return acceptsSubroutine(a_pSubroutine->getReturnType(), a_pSubroutine->getName(),
                             a_pSubroutine->getParameterTypes(), a_pSubroutine->getModifiers(),
                             a_pSubroutine->getFlags(), a_pOutConflictingSubroutines);
}

void Scope::getElementDoubles(Symbol* a_pElement, Symbols& out) const
{
    size_t elementIndex = m_pThisElement->getElementIndex(a_pElement);
    PHANTOM_ASSERT(elementIndex != ~size_t(0));
    Symbol* pThisSymbol = m_pThisElement->asSymbol();
    if (pThisSymbol)
    {
        Symbols thisDoubles;
        pThisSymbol->getDoubles(thisDoubles);
        for (auto it = thisDoubles.begin(); it != thisDoubles.end(); ++it)
        {
            Symbol* pDouble = static_cast<Symbol*>((*it)->getElements()[elementIndex]);
            PHANTOM_ASSERT(pDouble->getQualifiedDecoratedName() == a_pElement->getQualifiedDecoratedName());
            out.push_back(pDouble);
        }
    }
}

Alias* Scope::getTypedef(StringView a_strTypedef) const
{
    Alias* pAlias = getAlias(a_strTypedef);
    return pAlias->getAliasedSymbol()->asType() ? pAlias : nullptr;
}

void Scope::addUsing(Symbol* a_pElement)
{
    addAlias(PHANTOM_NEW(Alias)(a_pElement, a_pElement->getName()));
}

void Scope::fetchTypesCascade(Types& a_Types) const
{
    for (size_t i = 0; i < m_Types->size(); ++i)
    {
        Type* pType = m_Types[i];
        a_Types.push_back(pType);
        if (pType->asClassType())
        {
            static_cast<ClassType*>(pType)->fetchTypesCascade(a_Types);
        }
    }
    for (size_t i = 0; i < m_TemplateSpecializations->size(); ++i)
    {
        TemplateSpecialization* pSpec = m_TemplateSpecializations[i];
        Symbol*                 pBody = pSpec->getTemplated();
        if (pBody && pBody->asClassType())
        {
            a_Types.push_back(static_cast<ClassType*>(pBody));
            static_cast<ClassType*>(pBody)->fetchTypesCascade(a_Types);
        }
    }
}

AnonymousSection* Scope::getAnonymousSection(size_t a_uiIndex) const
{
    return m_AnonymousSections[a_uiIndex];
}

void Scope::addAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    onScopeSymbolAdded(a_pAnonymousSection);
}

void Scope::removeAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    onScopeSymbolRemoving(a_pAnonymousSection);
}

// ex: union(m, struct(x, y, union(z, w)))
AnonymousSection* Scope::addAnonymousSection(StringView a_strCode, Modifiers /*= 0*/, uint /*= 0*/)
{
    String str = a_strCode;
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    enum state
    {
        state_union_or_struct,
        state_list,
        state_ended,
    };
    state             s = state_union_or_struct;
    String            word;
    AnonymousSections sections;
    AnonymousSection* pSection = nullptr;
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        char c = *it;
        switch (s)
        {
        case state_union_or_struct:
            if (PHANTOM_CHAR_IS_CPP_IDENTIFIER(c))
            {
                word += c;
            }
            else if (c == '(')
            {
                if (word == "union")
                {
                    sections.push_back(PHANTOM_NEW(AnonymousUnion));
                }
                else if (word == "struct")
                {
                    sections.push_back(PHANTOM_NEW(AnonymousStruct));
                }
                else
                {
                    PHANTOM_DELETE_DYN(sections.front());
                    return nullptr;
                }
                s = state_list;
                word.clear();
            }
            else
            {
                PHANTOM_DELETE_DYN(sections.front());
                return nullptr;
            }
            break;

        case state_list:
            if (PHANTOM_CHAR_IS_CPP_IDENTIFIER(c))
            {
                word += c;
            }
            else if (c == '(')
            {
                if (word == "union")
                {
                    sections.push_back(PHANTOM_NEW(AnonymousUnion));
                }
                else if (word == "struct")
                {
                    sections.push_back(PHANTOM_NEW(AnonymousStruct));
                }
                else
                {
                    if (sections.size())
                    {
                        PHANTOM_DELETE_DYN(sections.front());
                    }
                    return nullptr;
                }
                word.clear();
            }
            else if (c == ',')
            {
                if (word.size())
                {
                    if (sections.empty())
                        return nullptr;
                    Variable* pElement = getVariable(word);
                    if (pElement == nullptr)
                    {
                        if (sections.size())
                        {
                            PHANTOM_DELETE_DYN(sections.front());
                        }
                        return nullptr;
                    }
                    sections.back()->addVariable(pElement);
                }
                // data member
                word.clear();
            }
            else if (c == ')')
            {
                if (sections.empty())
                    return nullptr;
                pSection = sections.back();
                sections.pop_back();
                if (sections.empty())
                {
                    addAnonymousSection(pSection);
                    s = state_ended;
                }
                else
                {
                    sections.back()->addAnonymousSection(pSection);
                    s = state_list;
                }
            }
            else
            {
                if (sections.size())
                {
                    PHANTOM_DELETE_DYN(sections.front());
                }
                return nullptr;
            }
            break;

        case state_ended:
            if (sections.size())
            {
                PHANTOM_DELETE_DYN(sections.front());
            }
            return nullptr;
        }
    }
    return pSection;
}

bool Scope::isSymbolHidden(Symbol* a_pSymbol) const
{
    return m_pThisElement->isSymbolHidden(a_pSymbol);
}

void Scope::getScopedSymbolsWithName(StringView a_Name, Symbols& a_Symbols) const
{
    Template* pTemplate = getTemplate(a_Name);
    if (pTemplate)
    {
        a_Symbols.push_back(pTemplate);
        return;
    }
    asLanguageElement()->getSymbolsWithName(a_Name, a_Symbols);
}

TemplateSpecialization* Scope::addTemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                         const LanguageElements& a_Arguments, Symbol* a_pBody)
{
    TemplateSpecialization* pTemplateSpecialization;

    if (!(m_pThisElement->isNative()) &&
        (pTemplateSpecialization = a_pTemplate->getTemplateSpecialization(a_Arguments)) &&
        pTemplateSpecialization->getModule() == m_pThisElement->getModule())
    {
        PHANTOM_ASSERT(false);
        return nullptr;
    }
    addTemplateSpecialization(pTemplateSpecialization =
                              TemplateSpecialization::Create(a_pTemplate, a_pTemplateSignature, a_Arguments, a_pBody,
                                                             a_pTemplate->getFlags() & PHANTOM_R_FLAG_NATIVE));
    pTemplateSpecialization->setFlags(PHANTOM_R_FLAG_NATIVE * m_pThisElement->isNative());
    return pTemplateSpecialization;
}

TemplateSpecialization* Scope::addTemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                         const LanguageElements& a_Arguments)
{
    TemplateSpecialization* pTemplateSpecialization;
    if (!(m_pThisElement->isNative()) &&
        (pTemplateSpecialization = a_pTemplate->getTemplateSpecialization(a_Arguments)) &&
        pTemplateSpecialization->getModule() == m_pThisElement->getModule())
    {
        PHANTOM_ASSERT(false, "template already instantiated in this module");
        return nullptr;
    }
    addTemplateSpecialization(pTemplateSpecialization = PHANTOM_NEW(TemplateSpecialization)(
                              a_pTemplate, a_pTemplateSignature, a_Arguments,
                              PHANTOM_R_FLAG_PRIVATE_VIS | (PHANTOM_R_FLAG_NATIVE * m_pThisElement->isNative())));
    return pTemplateSpecialization;
}

TemplateSpecialization*
Scope::addTemplateInstantiation(TemplateSpecialization* a_pInstantiationSpecialization,
                                const LanguageElements& a_Arguments,
                                const PlaceholderMap&   a_PartialSpecializationParameterDeductions)
{
    TemplateSpecialization* pTemplateSpecialization;
    if (!(m_pThisElement->isNative()) &&
        (pTemplateSpecialization =
         a_pInstantiationSpecialization->getTemplate()->getTemplateSpecialization(a_Arguments)) &&
        pTemplateSpecialization->getModule() == m_pThisElement->getModule())
    {
        PHANTOM_ASSERT(false, "template already instantiated in this module");
        return nullptr;
    }
    addTemplateSpecialization(pTemplateSpecialization = PHANTOM_NEW(TemplateSpecialization)(
                              a_pInstantiationSpecialization, a_Arguments, a_PartialSpecializationParameterDeductions));
    // pTemplateSpecialization->setFlags(PHANTOM_R_FLAG_NATIVE*m_pThisElement->isNative());
    return pTemplateSpecialization;
}

void Scope::findClasses(Classes& a_Classes, Class* a_pBaseClass /*= nullptr*/,
                        bool a_bDefaultInstanciable /*= false*/) const
{
    for (size_t i = 0; i < m_Types->size(); ++i)
    {
        if (Class* pClass = (*m_Types)[i]->asClass())
        {
            if ((a_pBaseClass == nullptr || (pClass->isA(a_pBaseClass) && !(pClass->isSame(a_pBaseClass)))) &&
                (a_bDefaultInstanciable == false || pClass->isDefaultInstanciable()))
                a_Classes.push_back(pClass);
            pClass->findClasses(a_Classes, a_pBaseClass, a_bDefaultInstanciable);
        }
    }

    for (size_t i = 0; i < m_TemplateSpecializations->size(); ++i)
    {
        if (auto pTemplated = (*m_TemplateSpecializations)[i]->getTemplated())
        {
            if (Class* pClass = pTemplated->asClass())
            {
                if ((a_pBaseClass == nullptr || (pClass->isA(a_pBaseClass) && !(pClass->isSame(a_pBaseClass)))) &&
                    (a_bDefaultInstanciable == false || pClass->isDefaultInstanciable()))
                    a_Classes.push_back(pClass);
                pClass->findClasses(a_Classes, a_pBaseClass, a_bDefaultInstanciable);
            }
        }
    }
}

} // namespace lang
} // namespace phantom
