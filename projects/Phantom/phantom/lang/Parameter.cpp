// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Parameter.h"

#include "Application.h"
#include "TypeOf.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Parameter::Parameter(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : LocalVariable(a_pValueType, a_strName, a_Modifiers, a_uiFlags)
{
}

Parameter::Parameter(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Parameter(PHANTOM_TYPEOF(void), {}, a_Modifiers, a_uiFlags)
{
}

Parameter::Parameter(Type* a_pType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Parameter(a_pType, {}, a_Modifiers, a_uiFlags)
{
}

Parameter* Parameter::cloneImpl(LanguageElement* a_pOwner) const
{
    return a_pOwner->New<Parameter>(m_pValueType, getName(), getModifiers());
}

bool Parameter::isEllipsis() const
{
    return getValueType() == PHANTOM_TYPEOF(void);
}

void Parameter::setNativeDefaultArgumentStr(StringView a_DefaultArgument)
{
    m_NativeDefaultArgumentStr = a_DefaultArgument;
}

bool Parameter::hasDefaultArgument() const
{
    return m_NativeDefaultArgumentStr.size() || m_pDefaultArgumentExpression != nullptr;
}

Expression* Parameter::getDefaultArgumentExpression() const
{
    if (m_pDefaultArgumentExpression == nullptr && m_NativeDefaultArgumentStr.size() &&
        !Application::Get()->getCppExpressionParser().empty())
    {
        PHANTOM_ASSERT(isNative());
        m_pDefaultArgumentExpression =
        Application::Get()->cppExpression(m_NativeDefaultArgumentStr, getOwner()->getOwner());
    }
    return m_pDefaultArgumentExpression;
}

void Parameter::setDefaultArgumentExpression(Expression* a_pDefaultArgumentExpression)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(a_pDefaultArgumentExpression && m_pDefaultArgumentExpression == nullptr);
    m_pDefaultArgumentExpression = a_pDefaultArgumentExpression;
    // temporary hack waiting for a more opaque handling of default expressions
}

} // namespace lang
} // namespace phantom
