// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "AnonymousSection.h"

#include "Variable.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
AnonymousSection::AnonymousSection(Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol("", modifiers, a_uiFlags)
{
}

void AnonymousSection::addVariable(Variable* a_pVariable)
{
    PHANTOM_ASSERT(a_pVariable);
    addReferencedElement(a_pVariable);
    m_Variables.push_back(a_pVariable);
    m_DataElements.push_back(a_pVariable);
    PHANTOM_ASSERT(a_pVariable->m_pAnonymousSection == nullptr);
    a_pVariable->m_pAnonymousSection = this;
}

void AnonymousSection::addAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    PHANTOM_ASSERT(std::find(m_AnonymousSections.begin(), m_AnonymousSections.end(), a_pAnonymousSection) ==
                   m_AnonymousSections.end());
    addElement(a_pAnonymousSection);
}

void AnonymousSection::removeAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    m_AnonymousSections.erase(std::find(m_AnonymousSections.begin(), m_AnonymousSections.end(), a_pAnonymousSection));
    removeElement(a_pAnonymousSection);
}

void AnonymousSection::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    auto found = std::find(m_Variables.begin(), m_Variables.end(), a_pElement);
    if (found != m_Variables.end())
    {
        m_Variables.erase(found);
        PHANTOM_ASSERT(a_pElement->asVariable());
        static_cast<Variable*>(a_pElement)->m_pAnonymousSection = nullptr;
    }
}

void AnonymousSection::onElementAdded(LanguageElement* a_pElement)
{
    m_AnonymousSections.push_back(static_cast<AnonymousSection*>(a_pElement));
    m_DataElements.push_back(a_pElement);
}

void AnonymousSection::onElementRemoved(LanguageElement* a_pElement)
{
    m_AnonymousSections.erase(
    std::find(m_AnonymousSections.begin(), m_AnonymousSections.end(), static_cast<AnonymousSection*>(a_pElement)));
    m_DataElements.erase(std::find(m_DataElements.begin(), m_DataElements.end(), a_pElement));
}

} // namespace lang
} // namespace phantom
