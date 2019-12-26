// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Parameter.h"

#include "Application.h"
#include "TypeOf.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Parameter::Parameter(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : LocalVariable(a_pValueType, a_strName, a_Modifiers, a_uiFlags)
{
}

Parameter::Parameter(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Parameter(PHANTOM_TYPEOF(void), "", a_Modifiers, a_uiFlags)
{
}

Parameter::Parameter(Type* a_pType, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Parameter(a_pType, "", a_Modifiers, a_uiFlags)
{
}

Parameter* Parameter::cloneImpl() const
{
    return PHANTOM_NEW(Parameter)(m_pValueType, m_strName, m_Modifiers);
}

bool Parameter::isEllipsis() const
{
    return getValueType() == PHANTOM_TYPEOF(void);
}

void Parameter::setNativeDefaultArgumentStr(StringView a_DefaultArgument)
{
    m_NativeDefaultArgumentStr = a_DefaultArgument;
}

void Parameter::setDefinitionName(StringView a_strName)
{
    m_strName = a_strName;
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
        return Application::Get()->cppExpression(m_NativeDefaultArgumentStr, getOwner()->getOwner());
    }
    return m_pDefaultArgumentExpression;
}

void Parameter::setDefaultArgumentExpression(Expression* a_pDefaultArgumentExpression)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(a_pDefaultArgumentExpression && m_pDefaultArgumentExpression == nullptr);
    m_pDefaultArgumentExpression = a_pDefaultArgumentExpression;
    // temporary hack waiting for a more opaque handling of default expressions
    addElement(reinterpret_cast<LanguageElement*>(m_pDefaultArgumentExpression));
}

} // namespace reflection
} // namespace phantom
