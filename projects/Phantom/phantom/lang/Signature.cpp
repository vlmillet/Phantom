// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Signature.h"

#include "Application.h"
#include "Parameter.h"
#include "Source.h"
#include "Subroutine.h"

#include <phantom/utils/StringUtil.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Signature* Signature::Create(LanguageElement* a_pOwner, Type* a_pRet, TypesView a_ParamTs,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    Parameters params;
    for (auto p : a_ParamTs)
        params.push_back(a_pOwner->NewDeferred<Parameter>(p));
    return a_pOwner->NewDeferred<Signature>(a_pRet, params, a_Modifiers, a_uiFlags);
}

Signature* Signature::Create(LanguageElement* a_pOwner, Type* a_pRet, Modifiers a_Modifiers /*= 0*/,
                             uint a_uiFlags /*= 0*/)
{
    return a_pOwner->NewDeferred<Signature>(a_pRet, a_Modifiers, a_uiFlags);
}

Signature::Signature(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Signature(PHANTOM_TYPEOF(void), Parameters{}, a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags)
{
}

Signature::Signature(Type* a_pReturnType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Signature(a_pReturnType, Parameters{}, a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags)
{
}

Signature::Signature(Type* a_pType, const Parameters& a_Parameters, Modifiers a_Modifiers /*= 0 */,
                     uint a_uiFlags /*= 0*/)
    : Symbol("", a_Modifiers & ~PHANTOM_R_NOCONST, a_uiFlags), m_pReturnType(a_pType), m_Parameters(a_Parameters)
{
    PHANTOM_ASSERT((getModifiers() & ~(PHANTOM_R_METHOD_QUAL_MASK)) == 0);
    // FIXME : check assert validity
    //     PHANTOM_ASSERT(m_pReturnType &&
    //                    (m_pReturnType->isTemplateDependant() || (m_pReturnType == PHANTOM_TYPEOF(void)) ||
    //                     m_pReturnType->asReference() || m_pReturnType->isMoveConstructible()));

    Parameter* prev = nullptr;
    for (auto p : m_Parameters)
    {
        PHANTOM_ASSERT(!prev || !prev->hasDefaultArgument() || p->hasDefaultArgument(),
                       "parameter must have a default argument because the previous parameter had one");
        p->addFlags(PHANTOM_R_FLAG_NATIVE * isNative());
        prev = p;
    }
}

void Signature::initialize()
{
    Symbol::initialize();
    addReferencedElement(m_pReturnType);
    for (auto p : m_Parameters)
    {
        addReferencedElement(p);
        p->setOwner(this);
    }
}

size_t Signature::getParameterCount() const
{
    return m_Parameters.size();
}

Type* Signature::getParameterType(size_t a_uiParamIndex) const
{
    PHANTOM_ASSERT(a_uiParamIndex < getParameterCount(), "Index too big");
    return m_Parameters[a_uiParamIndex]->getValueType();
}

Type* Signature::getReturnType() const
{
    return m_pReturnType;
}

void Signature::setReturnType(Type* a_pRetType)
{
    PHANTOM_ASSERT(m_pReturnType == nullptr || m_pReturnType->isTemplateDependant());
    m_pReturnType = a_pRetType;
}

bool Signature::separateParameters(StringView a_strText, Strings& a_OutParameters)
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

bool Signature::parseParameterTypeList(StringView, LanguageElement*)
{
    return false;
}

bool Signature::isSame(Signature* a_pOther) const
{
    if (getModifiers() != a_pOther->getModifiers())
        return false;
    Types types;
    for (size_t i = 0; i < m_Parameters.size(); ++i)
    {
        if (!(m_Parameters[i]->getValueType()->isSame(a_pOther->m_Parameters[i]->getValueType())))
            return false;
    }
    return m_pReturnType->isSame(a_pOther->m_pReturnType);
}

void Signature::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getQualifiedDecoratedName(a_Buf);
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

void Signature::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getRelativeDecoratedName(a_pTo, a_Buf);
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

void Signature::getUniqueName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getUniqueName(a_Buf);
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

void Signature::getQualifiedName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getQualifiedName(a_Buf);
    }
    a_Buf += ')';
}

void Signature::getDecoratedName(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (Parameter* pParam : m_Parameters)
    {
        if (pParam != m_Parameters[0])
            a_Buf += ',';
        pParam->getValueType()->getDecoratedName(a_Buf);
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

Signature* Signature::clone(LanguageElement* a_pOwner) const
{
    Parameters params = m_Parameters;
    for (Parameter*& pParameter : params)
    {
        pParameter = static_cast<Parameter*>(pParameter->clone(a_pOwner));
    }
    return a_pOwner->NewDeferred<Signature>(m_pReturnType, params, getModifiers());
}

bool Signature::isSame(Symbol* a_pOther) const
{
    if (!(a_pOther->asSignature()))
        return false;
    return isSame(static_cast<Signature*>(a_pOther));
}

StringView Signature::getParameterName(size_t a_uiParamIndex) const
{
    return m_Parameters[a_uiParamIndex]->getName();
}

Expression* Signature::getParameterDefaultValueExpression(size_t a_uiParamIndex) const
{
    return m_Parameters[a_uiParamIndex]->getDefaultArgumentExpression();
}

Types Signature::getParameterTypes() const
{
    Types types;
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        types.push_back((*it) ? (*it)->getValueType() : nullptr);
    }
    return types;
}

size_t Signature::getRequiredArgumentCount() const
{
    size_t defaultParameterCount = 0;
    size_t i = m_Parameters.size();
    while (i--)
    {
        if (m_Parameters[i]->hasDefaultArgument())
            defaultParameterCount++;
        else
            break;
    }
    return m_Parameters.size() - defaultParameterCount;
}

bool Signature::matches(TypesView a_ParameterTypes, Modifiers a_Modifiers) const
{
    if (a_ParameterTypes.size() != getParameterCount() || (getModifiers() != a_Modifiers))
    {
        return false;
    }
    for (size_t i = 0; i < m_Parameters.size(); ++i)
    {
        if (a_ParameterTypes[i] != nullptr && !(m_Parameters[i]->getValueType()->isSame(a_ParameterTypes[i])))
            return false;
    }
    return true;
}

void Signature::getUniqueParameterTypeNameList(StringBuffer& a_Buf) const
{
    a_Buf += '(';
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        if (it != m_Parameters.begin())
            a_Buf += ',';
        (*it)->getValueType()->getUniqueName(a_Buf);
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

bool Signature::isRVOCandidate() const
{
    return m_pReturnType->removeQualifiers()->asClassType() != nullptr;
}

Subroutine* Signature::getOwnerSubroutine() const
{
    return static_cast<Subroutine*>(getOwner());
}

Parameter* Signature::getParameter(StringView a_strName) const
{
    for (auto it = m_Parameters.begin(); it != m_Parameters.end(); ++it)
    {
        if ((*it)->getName() == a_strName)
            return *it;
    }
    return nullptr;
}

void Signature::setNativeDefaultArgumentStrings(ArrayView<StringView> a_DefaultArgs)
{
    if (a_DefaultArgs.empty())
        return;
    size_t paramCount = m_Parameters.size();
    PHANTOM_ASSERT(paramCount, "empty signature cannot have default arguments");
    PHANTOM_ASSERT(a_DefaultArgs.size() <= paramCount, "too many default arguments");
    for (size_t i = 0; i < a_DefaultArgs.size(); ++i)
    {
        auto       arg = a_DefaultArgs[i];
        size_t     paramIndex = paramCount - a_DefaultArgs.size() + i;
        Parameter* pParameter = getParameter((size_t)paramIndex);
        pParameter->m_NativeDefaultArgumentStr = arg;
        paramIndex--;
    }
}

StringViews Signature::getNativeDefaultArgumentStrings() const
{
    StringViews args;
    for (auto pParam : m_Parameters)
    {
        StringView view = pParam->getNativeDefaultArgumentString();
        if (view.size())
            args.push_back(view);
    }
    return args;
}

FunctionType* Signature::getFunctionType() const
{
    PHANTOM_ASSERT(getSource(), "signature must be attached to a source via its subroutine");
    return getSource()->functionType(m_pReturnType, getParameterTypes(), getModifiers());
}

} // namespace lang
} // namespace phantom
