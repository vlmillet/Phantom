// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "FunctionType.h"

#include "FunctionPointer.h"
#include "Source.h"
#ifndef __DOXYGEN__
#    include <phantom/utils/StringUtil.h>
#endif
#include "Application.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
FunctionType::FunctionType(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(TypeKind::Function, "", 0, 0, a_Modifiers, a_uiFlags), m_pReturnType(nullptr)
{
}

FunctionType::FunctionType(Type* a_pReturnType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(TypeKind::Function, "", 0, 0, a_Modifiers, a_uiFlags), m_pReturnType(nullptr)
{
    setReturnType(a_pReturnType);
}

FunctionType::FunctionType(Type* a_pReturnType, Type* a_pSingleParameterType, Modifiers a_Modifiers /*= 0*/,
                           uint a_uiFlags /*= 0*/)
    : Type(TypeKind::Function, "", 0, 0, a_Modifiers, a_uiFlags), m_pReturnType(nullptr)
{
    setReturnType(a_pReturnType);
    addParameterType(a_pSingleParameterType);
}

FunctionType::FunctionType(Type* a_pType, TypesView a_Types, Modifiers a_Modifiers /*= 0 */, uint a_uiFlags /*=0*/)
    : Type(TypeKind::Function, "", 0, 0, a_Modifiers, a_uiFlags), m_pReturnType(nullptr)
{
    setReturnType(a_pType);
    for (auto it = a_Types.begin(); it != a_Types.end(); ++it)
    {
        addParameterType(*it);
    }
}

void FunctionType::parse(StringView a_strFunctionType, LanguageElement* a_pContextScope)
{
    size_t i = 0;
    size_t length = a_strFunctionType.size();
    String returnType;
    char   prevChar = '0';
    for (; i < length; ++i)
    {
        char c = a_strFunctionType[i];
        if (c == '(')
        {
            PHANTOM_ASSERT(!(returnType.empty()), "no return type specified in the signature String");
            Type* pReturnType = m_pReturnType;
            if (pReturnType == nullptr)
            {
                pReturnType = Application::Get()->findCppType(returnType, a_pContextScope);
            }
            PHANTOM_ASSERT(pReturnType, "%s", returnType.c_str());
            setReturnType(pReturnType);
            Types parameterTypes;
            PHANTOM_VERIFY(parseParameterTypeList(a_strFunctionType.substr(i), parameterTypes, a_pContextScope), "%.*s",
                           PHANTOM_STRING_AS_PRINTF_ARG(a_strFunctionType.substr(i)));
            for (auto p : parameterTypes)
            {
                addParameterType(p);
            }
            return;
        }
        else
        {
            if (prevChar == '>' && c == '>') /// avoid shift right / end of template ambiguity
                returnType += ' ';
            returnType += c;
        }
        prevChar = c;
    }
}

FunctionType::~FunctionType() {}

void FunctionType::addParameterType(Type* a_pType)
{
    m_ParameterTypes.push_back(a_pType);
    addReferencedElement(a_pType);
    if (a_pType->isTemplateDependant())
        setTemplateDependant();
}

void FunctionType::setReturnType(Type* a_pType)
{
    m_pReturnType = a_pType;
    PHANTOM_ASSERT(m_pReturnType);
    addReferencedElement(a_pType);
    if (a_pType->isTemplateDependant())
        setTemplateDependant();
}

size_t FunctionType::getParameterTypeCount() const
{
    return m_ParameterTypes.size();
}

Type* FunctionType::getReturnType() const
{
    return m_pReturnType;
}

bool FunctionType::separateParameters(StringView a_strText, Strings& a_OutParameters)
{
    String parameter;
    size_t length = a_strText.size();
    size_t i = 0;
    size_t template_level = 0;
    for (; i < length; ++i)
    {
        char c = a_strText[i];
        if (c == ',' && template_level == 0)
        {
            a_OutParameters.push_back(parameter);
            parameter.clear();
        }
        else
        {
            if (c == '<')
            {
                ++template_level;
            }
            else if (c == '>')
            {
                if (template_level != 0)
                    --template_level;
            }
            parameter += c;
        }
    }
    if (template_level == 0 && !(parameter.empty()))
    {
        a_OutParameters.push_back(parameter);
        return true;
    }
    a_OutParameters.clear();
    return false;
}

bool FunctionType::parseParameterTypeList(StringView a_strText, Types&, LanguageElement* a_pContextScope)
{
    if (a_strText.empty())
        return false;
    Strings words;
    StringUtil::Split(words, a_strText, "()");
    if (words.empty())
        return true;
    if (a_strText[0] != '(')
    {
        words.erase(words.begin());
    }
    if (words.size() != 1)
        return false;

    Strings parameters;
    if (!(separateParameters(words.front(), parameters)))
        return false;

    for (auto it = parameters.begin(); it != parameters.end(); ++it)
    {
        String name;
        StringUtil::ReplaceAll(*it, ">", "> ");
        Symbol* pTypeElement = Application::Get()->findCppSymbol(*it, a_pContextScope);
        if (pTypeElement == nullptr)
        {
            PHANTOM_ASSERT(!isNative(), "%s", (*it).c_str());
            addParameterType(nullptr);
        }
        else
        {
            PHANTOM_ASSERT(pTypeElement->asType());
            addParameterType(static_cast<Type*>(pTypeElement));
        }
    }
    return true;
}

bool FunctionType::isSame(FunctionType* a_pOther) const
{
    if (getModifiers() != a_pOther->getModifiers())
        return false;
    if (m_ParameterTypes.size() != a_pOther->m_ParameterTypes.size())
        return false;
    Types types;
    for (size_t i = 0; i < m_ParameterTypes.size(); ++i)
    {
        if (!(m_ParameterTypes[i]->isSame(a_pOther->m_ParameterTypes[i])))
            return false;
    }
    return m_pReturnType->isSame(a_pOther->m_pReturnType);
}

void FunctionType::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pReturnType->getQualifiedDecoratedName(a_Buf);
    a_Buf += '(';
    for (Type* pType : m_ParameterTypes)
    {
        if (pType != m_ParameterTypes[0])
            a_Buf += ',';
        pType->getQualifiedDecoratedName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += " const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += " volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';

        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

void FunctionType::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pReturnType->getQualifiedName(a_Buf);
    a_Buf += '(';
    for (Type* pType : m_ParameterTypes)
    {
        if (pType != m_ParameterTypes[0])
            a_Buf += ',';
        pType->getQualifiedName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += " const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += " volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';

        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

void FunctionType::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pReturnType->getDecoratedName(a_Buf);
    a_Buf += '(';
    for (Type* pType : m_ParameterTypes)
    {
        if (pType != m_ParameterTypes[0])
            a_Buf += ',';
        pType->getDecoratedName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += " const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += " volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';

        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

void FunctionType::getUniqueName(StringBuffer& a_Buf) const
{
    m_pReturnType->getUniqueName(a_Buf);
    a_Buf += '(';
    for (Type* pType : m_ParameterTypes)
    {
        if (pType != m_ParameterTypes[0])
            a_Buf += ',';
        pType->getUniqueName(a_Buf);
    }
    a_Buf += ')';

    if (testModifiers(PHANTOM_R_CONST))
    {
        a_Buf += "@const";
    }
    if (testModifiers(PHANTOM_R_VOLATILE))
    {
        a_Buf += "@volatile";
    }
    if (testModifiers(PHANTOM_R_LVALUEREF) || testModifiers(PHANTOM_R_RVALUEREF))
    {
        a_Buf += '&';

        if (testModifiers(PHANTOM_R_RVALUEREF))
        {
            a_Buf += '&';
        }
    }
}

bool FunctionType::isSame(Symbol* a_pOther) const
{
    if (!(a_pOther->asFunctionType()))
        return false;
    return isSame(static_cast<FunctionType*>(a_pOther));
}

TypesView FunctionType::getParameterTypes() const
{
    return m_ParameterTypes;
}

bool FunctionType::hasEllipsis() const
{
    return m_ParameterTypes.size() && m_ParameterTypes.back()->asEllipsis();
}

bool FunctionType::matches(TypesView a_ParameterTypes, Modifiers a_Modifiers, uint) const
{
    if (a_ParameterTypes.size() != getParameterTypeCount() || !(testModifiers(a_Modifiers)))
    {
        return false;
    }
    for (size_t i = 0; i < m_ParameterTypes.size(); ++i)
    {
        if (!(m_ParameterTypes[i]->isSame(a_ParameterTypes[i])))
            return false;
    }
    return true;
}

bool FunctionType::isRVOCandidate() const
{
    return m_pReturnType->removeConst()->asClassType() != nullptr;
}

} // namespace lang
} // namespace phantom
