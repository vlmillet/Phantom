// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "TemplateParameter.h"

#include "Placeholder.h"
#include "Template.h"
#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemplateParameter::TemplateParameter(Placeholder* a_pPlaceholder, LanguageElement* a_pDefaultArgument,
                                     uint a_uiFlags /*= 0*/)
    : Symbol(a_pPlaceholder->asSymbol()->getName(), 0, a_uiFlags), m_pPlaceholder(a_pPlaceholder)
{
    PHANTOM_ASSERT(m_pPlaceholder);
    PHANTOM_ASSERT(m_pPlaceholder->asSymbol()->getOwner() == this);
    if (a_pDefaultArgument)
    {
        setDefaultArgument(a_pDefaultArgument);
    }
}

TemplateParameter* TemplateParameter::clone(LanguageElement* a_pOwner) const
{
    TemplateParameter* pTP = a_pOwner->New<TemplateParameter>(m_pPlaceholder, m_pDefaultArgument);
    pTP->setCodeRange(getCodeRange());
    return pTP;
}

TemplateParameter* TemplateParameter::clone(LanguageElement* a_pOwner, uint a_Flags /*= 0*/) const
{
    return a_pOwner->New<TemplateParameter>(getPlaceholder()->clone(a_pOwner), m_pDefaultArgument, a_Flags);
}

TemplateSignature* TemplateParameter::getTemplateSignature() const
{
    PHANTOM_ASSERT(getOwner() && getOwner()->asTemplateSignature());
    return static_cast<TemplateSignature*>(getOwner());
}

hash64 TemplateParameter::computeLocalHash() const
{
    PHANTOM_ASSERT(getOwner());
    return getIndex();
}

void TemplateParameter::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (a_pTo == getTemplateSpecialization() || a_pTo == getTemplateSignature() || a_pTo == getTemplate())
    {
        return getName(a_Buf);
    }
    size_t sz = a_Buf.size();
    getTemplateSpecialization()->getRelativeName(a_pTo, a_Buf);
    PHANTOM_ASSERT(sz != a_Buf.size());
    a_Buf += "::";
    getName(a_Buf);
}

void TemplateParameter::setDefaultArgument(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_pElement);
    PHANTOM_ASSERT(m_pDefaultArgument == nullptr, "default argument already defined");
    m_pDefaultArgument = a_pElement;
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

size_t TemplateParameter::getIndex() const
{
    return static_cast<TemplateSignature*>(getOwner())->getTemplateParameterIndex(const_cast<TemplateParameter*>(this));
}

Template* TemplateParameter::getTemplate() const
{
    return getTemplateSignature()->getTemplate();
}

TemplateSpecialization* TemplateParameter::getTemplateSpecialization() const
{
    return getTemplateSignature()->getTemplateSpecialization();
}

} // namespace lang
} // namespace phantom
