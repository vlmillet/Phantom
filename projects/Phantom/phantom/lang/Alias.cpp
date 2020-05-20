// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Alias.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Alias::Alias(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*=0*/) : Symbol("", a_Modifiers, a_uiFlags) {}

Alias::Alias(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags)
{
}

Alias::Alias(Symbol* a_pSymbol, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_pAliasedSymbol(a_pSymbol)
{
    PHANTOM_ASSERT(m_pAliasedSymbol);
}

void Alias::terminate()
{
    if (m_pAliases)
    {
        delete_<Aliases>(m_pAliases);
    }
    Symbol::terminate();
}

void Alias::addAlias(Alias* a_pAlias)
{
    if (!m_pAliases)
    {
        m_pAliases = new_<Aliases>();
        m_pAliases->setAllocator(getAllocator());
    }
    m_pAliases->push_back(a_pAlias);
    a_pAlias->setOwner(this);
}

Alias* Alias::getAlias(StringView a_strName) const
{
    if (m_pAliases)
    {
        for (auto pSA : *m_pAliases)
        {
            if (pSA->getName() == a_strName)
                return pSA;
        }
    }
    return nullptr;
}

void Alias::fetchAccessibleSymbols(Symbols&, bool) const
{
    return;
}

} // namespace lang
} // namespace phantom
