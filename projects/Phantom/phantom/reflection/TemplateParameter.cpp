// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "TemplateParameter.h"

#include "Placeholder.h"
#include "TemplateSignature.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
TemplateParameter::TemplateParameter(Placeholder* a_pPlaceholder, LanguageElement* a_pDefaultArgument,
                                     Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_pPlaceholder ? a_pPlaceholder->asSymbol()->getName() : "", a_Modifiers, a_uiFlags),
      m_pPlaceholder(a_pPlaceholder)
{
    PHANTOM_ASSERT(m_pPlaceholder);
    addElement(m_pPlaceholder->asSymbol());
    if (a_pDefaultArgument)
    {
        setDefaultArgument(a_pDefaultArgument);
    }
}

TemplateParameter* TemplateParameter::clone() const
{
    TemplateParameter* pTP = PHANTOM_NEW(TemplateParameter)(m_pPlaceholder, m_pDefaultArgument, m_Modifiers);
    pTP->setCodeRange(getCodeRange());
    return pTP;
}

TemplateParameter* TemplateParameter::clone(uint a_Flags /*= 0*/) const
{
    return PHANTOM_NEW(TemplateParameter)(getPlaceholder()->clone(), m_pDefaultArgument, Modifiers(), a_Flags);
}

void TemplateParameter::onElementRemoved(LanguageElement* a_pElement)
{
    if (m_pDefaultArgument == a_pElement)
        m_pDefaultArgument = nullptr;
    LanguageElement::onElementRemoved(a_pElement);
}

void TemplateParameter::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pDefaultArgument == a_pElement)
        m_pDefaultArgument = nullptr;
    LanguageElement::onReferencedElementRemoved(a_pElement);
}

void TemplateParameter::setDefaultArgument(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_pElement);
    PHANTOM_ASSERT(m_pDefaultArgument == nullptr, "default argument already defined");
    m_pDefaultArgument = a_pElement;
    if (m_pDefaultArgument)
    {
        if (m_pDefaultArgument->getOwner())
            addReferencedElement(m_pDefaultArgument);
        else
            addElement(m_pDefaultArgument);
    }
}

bool TemplateParameter::partialAccepts(LanguageElement* a_pLanguageElement, size_t& a_Score,
                                       PlaceholderMap& a_Deductions) const
{
    return m_pPlaceholder->asSymbol()->partialAccepts(a_pLanguageElement, a_Score, a_Deductions);
}

bool TemplateParameter::acceptsArgument(LanguageElement* a_pLanguageElement) const
{
    return m_pPlaceholder->accepts(a_pLanguageElement);
}

bool TemplateParameter::isSame(TemplateParameter* a_pOther) const
{
    return a_pOther->m_pPlaceholder AND m_pPlaceholder AND a_pOther->m_pPlaceholder->asSymbol()->isSame(
    m_pPlaceholder->asSymbol());
}

size_t TemplateParameter::getIndex() const
{
    return static_cast<TemplateSignature*>(getOwner())->getTemplateParameterIndex(const_cast<TemplateParameter*>(this));
}

Template* TemplateParameter::getTemplate() const
{
    return getTemplateSignature()->getTemplate();
}

} // namespace reflection
} // namespace phantom
