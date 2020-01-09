// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
    static Alias* Create(Symbol* a_pSymbol, StringView a_strAlias, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

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

    Type* toType() const override
    {
        return m_pAliasedSymbol ? m_pAliasedSymbol->toType() : nullptr;
    }

    Namespace* toNamespace() const override
    {
        return m_pAliasedSymbol ? m_pAliasedSymbol->toNamespace() : nullptr;
    }

    Alias* asAlias() const override
    {
        return (Alias*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets aliased symbol.
    ///
    /// \return null if it fails, else the aliased symbol.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbol* getAliasedSymbol() const
    {
        return m_pAliasedSymbol;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets aliased element.
    ///
    /// \param [in,out] a_pElement  If non-null, the element.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setAliasedSymbol(Symbol* a_pElement)
    {
        m_pAliasedSymbol = a_pElement;
    }

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

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;
    void onElementRemoved(LanguageElement* a_pElement) override;

private:
    Symbol*  m_pAliasedSymbol = nullptr;
    Aliases* m_pAliases = nullptr;
};

} // namespace lang
} // namespace phantom
