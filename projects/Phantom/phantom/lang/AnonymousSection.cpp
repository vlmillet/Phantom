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
    m_Variables.push_back(a_pVariable);
    m_DataElements.push_back(a_pVariable);
    PHANTOM_ASSERT(a_pVariable->m_pAnonymousSection == nullptr);
    a_pVariable->m_pAnonymousSection = this;
}

void AnonymousSection::addAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    m_AnonymousSections.push_back(a_pAnonymousSection);
    m_DataElements.push_back(a_pAnonymousSection);
}

void AnonymousSection::removeAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    m_AnonymousSections.erase(std::find(m_AnonymousSections.begin(), m_AnonymousSections.end(), a_pAnonymousSection));
    m_DataElements.erase(std::find(m_DataElements.begin(), m_DataElements.end(), a_pAnonymousSection));
}

} // namespace lang
} // namespace phantom
