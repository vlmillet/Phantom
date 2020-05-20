// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Alias : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

public:
    PHANTOM_DECLARE_META_CLASS(Alias);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Default Constructor.
    ///
    /// \param  a_Modifiers The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an empty alias (useful to create compound/qualified aliases).
    ///
    /// \param  a_strName   the alias name.
    /// \param  a_Modifiers (optional) the alias' modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an alias.
    ///
    /// \param [in,out] a_pSymbol       the aliased symbol.
    /// \param  a_strName               (optional) the alias name. if empty, a_pSymbol->getName() is
    /// used instead. \param  a_Modifiers             (optional) the alias' modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias(Symbol* a_pSymbol, StringView a_strName = "", Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    void initialize()
    {
        Symbol::initialize();
        if (m_pAliasedSymbol)
            addReferencedElement(m_pAliasedSymbol);
    }
    void terminate() override;

    Type* toType() const override { return m_pAliasedSymbol ? m_pAliasedSymbol->toType() : nullptr; }

    Namespace* toNamespace() const override { return m_pAliasedSymbol ? m_pAliasedSymbol->toNamespace() : nullptr; }

    Alias* asAlias() const override { return (Alias*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets aliased symbol.
    ///
    /// \return null if it fails, else the aliased symbol.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbol* getAliasedSymbol() const { return m_pAliasedSymbol; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets aliased element.
    ///
    /// \param [in,out] a_pElement  If non-null, the element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setAliasedSymbol(Symbol* a_pElement) { m_pAliasedSymbol = a_pElement; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a sub alias to this alias.
    ///
    /// \param [in,out] a_pAlias    If non-null, the alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addAlias(Alias* a_pAlias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a sub alias from its name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if it fails, else the alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias* getAlias(StringView a_strName) const;

    virtual void fetchAccessibleSymbols(Symbols& a_Symbols, bool a_bIncludeUnamedSubSymbols = true) const;

private:
    Symbol*  m_pAliasedSymbol = nullptr;
    Aliases* m_pAliases = nullptr;
};

} // namespace lang
} // namespace phantom
