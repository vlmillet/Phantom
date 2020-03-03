// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Alias.h"

#include <phantom/detail/new.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Alias::Alias(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*=0*/) : Symbol("", a_Modifiers, a_uiFlags)
{
}

Alias::Alias(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags)
{
}

Alias::Alias(Symbol* a_pSymbol, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags), m_pAliasedSymbol(a_pSymbol)
{
    if (m_pAliasedSymbol)
    {
        addReferencedElement(m_pAliasedSymbol);
    }
    else
        setInvalid();
}

void Alias::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pAliasedSymbol == a_pElement)
        m_pAliasedSymbol = nullptr;
}

void Alias::onElementRemoved(LanguageElement* a_pElement)
{
    if (m_pAliases)
    {
        for (auto it = m_pAliases->begin(); it != m_pAliases->end(); ++it)
        {
            if ((*it) == a_pElement)
            {
                m_pAliases->erase(it);
                if (m_pAliases->empty())
                {
                    PHANTOM_DELETE(Aliases) m_pAliases;
                    m_pAliases = nullptr;
                    break;
                }
            }
        }
    }
    Symbol::onElementRemoved(a_pElement);
}

void Alias::addAlias(Alias* a_pAlias)
{
    if (m_pAliases == nullptr)
        m_pAliases = PHANTOM_NEW(Aliases);
    m_pAliases->push_back(a_pAlias);
    addElement(a_pAlias);
}

Alias* Alias::getAlias(StringView a_strName) const
{
    if (m_pAliases)
    {
        for (auto it = m_pAliases->begin(); it != m_pAliases->end(); ++it)
        {
            if ((*it)->getName() == a_strName)
                return *it;
        }
    }
    return nullptr;
}

void Alias::fetchAccessibleSymbols(Symbols&, bool) const
{
    return;
}

Alias* Alias::Create(Symbol* a_pSymbol, StringView a_strAlias, Modifiers a_Modifiers, uint a_uiFlags)
{
    return PHANTOM_DEFERRED_NEW_EX(Alias)(a_pSymbol, a_strAlias, a_Modifiers, a_uiFlags);
}

} // namespace lang
} // namespace phantom
