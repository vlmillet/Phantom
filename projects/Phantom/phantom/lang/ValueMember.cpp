// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ValueMember.h"

#include "Application.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
ValueMember::ValueMember(Type* a_pValueType, StringView a_strName, uint a_uiFilterMask, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags),
      m_pValueType(a_pValueType),
      m_uiFilterMask(a_uiFilterMask * ((m_Modifiers & PHANTOM_R_TRANSIENT) == 0))
{
    PHANTOM_ASSERT(m_pValueType);
    PHANTOM_ASSERT(m_pValueType->isNative() || m_pValueType->getOwner());
    addReferencedElement(m_pValueType);
    if (m_pValueType->isTemplateDependant())
        addFlags(PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
}

void* ValueMember::getAddress(void const*) const
{
    return nullptr;
}

void ValueMember::setDefaultExpression(Expression* a_pExpression)
{
    PHANTOM_ASSERT(!isNative());
    m_pDefaultExpression = a_pExpression;
    addElement(reinterpret_cast<LanguageElement*>(m_pDefaultExpression));
}

void ValueMember::setDefaultExpression(const char* a_pExpression)
{
    PHANTOM_ASSERT(isNative());
    m_pNativeDefaultExpressionStr = a_pExpression;
}

Expression* ValueMember::getDefaultExpression() const
{
    if (m_pDefaultExpression == nullptr && m_pNativeDefaultExpressionStr)
    {
        m_pDefaultExpression = Application::Get()->cppExpression(m_pNativeDefaultExpressionStr, getOwner());
        if (m_pDefaultExpression == nullptr)
        {
            PHANTOM_WARNING(false, "%s : cannot solve default value '%s'", getQualifiedDecoratedName().c_str(),
                            m_pNativeDefaultExpressionStr);
        }
    }
    return m_pDefaultExpression;
}

void ValueMember::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pValueType == a_pElement)
    {
        m_pValueType = nullptr;
    }
}

void ValueMember::setFilterMask(uint a_Mask)
{
    m_uiFilterMask = a_Mask;
}

void ValueMember::copyValue(void* a_pDestinationObject, void const* a_pSourceObject) const
{
    void* sourceBuffer = getValueType()->newInstance();
    getValue(a_pSourceObject, sourceBuffer);
    setValue(a_pDestinationObject, sourceBuffer);
    getValueType()->deleteInstance(sourceBuffer);
}

} // namespace lang
} // namespace phantom
