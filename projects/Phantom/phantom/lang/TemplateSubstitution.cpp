#include "TemplateSubstitution.h"

#include "Placeholder.h"
#include "TemplateParameter.h"

namespace phantom
{
namespace lang
{
TemplateSubstitution TemplateSubstitution::splitVariadics(Placeholder*      a_pPH,
                                                          LanguageElements& a_VariadicExpansion) const
{
    TemplateSubstitution subs;
    for (size_t i = 0; i < m_Placeholders.size(); ++i)
    {
        if (a_pPH == m_Placeholders[i])
            a_VariadicExpansion.push_back(m_Arguments[i]);
        else
            subs.insert(m_Placeholders[i], m_Arguments[i]);
    }
    return subs;
}

TemplateSubstitution::TemplateSubstitution(TemplateSpecialization* a_pInstantiation)
    : m_pInstantiation(a_pInstantiation)
{
}

TemplateSubstitution::TemplateSubstitution() : m_pInstantiation(nullptr) {}

void TemplateSubstitution::insert(Placeholder* a_pPlaceholder, LanguageElement* a_pArgument)
{
    m_Placeholders.push_back(a_pPlaceholder);
    m_Arguments.push_back(a_pArgument);
}

void TemplateSubstitution::substituteVariadic(Placeholder* a_pPlaceholder, LanguageElement* a_pArgument)
{
    for (size_t i = 0; i < m_Placeholders.size(); ++i)
    {
        if (a_pPlaceholder == m_Placeholders[i])
        {
            m_Arguments[i] = a_pArgument;
            return;
        }
    }
    insert(a_pPlaceholder, a_pArgument);
}

LanguageElement* TemplateSubstitution::getArgument(Placeholder* a_pPlaceholder) const
{
    // first check pure equality
    for (size_t i = 0; i < m_Placeholders.size(); ++i)
    {
        if (a_pPlaceholder == m_Placeholders[i])
            return m_Arguments[i];
    }
    // then check equivalence (can be runtime/native specializations having equivalent placeholders)
    TemplateParameter* pTP1 = static_cast<TemplateParameter*>(a_pPlaceholder->asSymbol()->getOwner());
    for (size_t i = 0; i < m_Placeholders.size(); ++i)
    {
        TemplateParameter* pTP2 = static_cast<TemplateParameter*>(m_Placeholders[i]->asSymbol()->getOwner());
        if (pTP1->getTemplate() != pTP2->getTemplate())
            return nullptr;
        if (pTP1->getIndex() == pTP2->getIndex())
            return m_Arguments[i];
    }
    return nullptr;
}

LanguageElements const& TemplateSubstitution::getArguments() const
{
    return m_Arguments;
}

void TemplateSubstitution::setInstantiation(TemplateSpecialization* a_pSpec)
{
    m_pInstantiation = a_pSpec;
}

} // namespace lang
} // namespace phantom
