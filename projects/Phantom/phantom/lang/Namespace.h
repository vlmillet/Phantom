// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Scope.h>
#include <phantom/lang/Symbol.h>
#include <phantom/utils/Signal.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Namespace : public Symbol, public Scope
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Namespace);

public:
    friend class phantom::Phantom;
    friend class Type;
    friend class Symbol;

public:
    static Namespace* Global();

public:
    Namespace(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    Namespace(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    PHANTOM_DTOR ~Namespace() override;

    Scope*     asScope() const override { return (Namespace*)this; }
    Namespace* asNamespace() const override { return (Namespace*)this; }
    Namespace* toNamespace() const override { return (Namespace*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the root namespace.
    ///
    /// \return The root namespace.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getRootNamespace() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the sub namespace with the given name.
    ///
    /// \param  a_strName   The namespace name.
    ///
    /// \return null if no sub namespace with given name, else the namespace found.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getNamespace(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a sub namespace recursively from a namespace name list.
    ///
    /// \param a_NameList    If non-null, the hierarchy words.
    ///
    /// \return null if it fails, else the namespace cascade.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getNamespaceCascade(StringView qualifiedName, const char* a_SeparatorList = ":") const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parent namespace.
    ///
    /// \return null if no parent (root), else the parent namespace.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getParentNamespace() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for a namespace recursively or create it if not found.
    ///
    /// \param  a_strNamespaceName  Name of the namespace.
    /// \param  separatorPattern    (optional) a pattern specifying the separator.
    ///
    /// \return null if it fails, else the found or create namespace cascade.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getOrCreateNamespace(StringView a_strNamespaceName, const char* separatorPattern = ":");

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a namespace alias to this namespace.
    ///
    /// \param  a_strAlias      The alias name.
    /// \param  a_pNamespace    The aliased namespace.
    ///
    /// \return null if it fails, else the newly created namespace alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias* addNamespaceAlias(StringView a_strAlias, Namespace* a_pNamespace);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes the namespace alias with given name.
    ///
    /// \param  a_strAlias  The alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeNamespaceAlias(StringView a_strAlias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a namespace alias with the given name.
    ///
    /// \param  a_strAlias  The alias name.
    ///
    /// \return null if it fails, else the namespace alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Alias* getNamespaceAlias(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a namespace aliased by the given name.
    ///
    /// \param  a_strAlias  The alias name.
    ///
    /// \return null if it fails, else the namespace aliased.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getNamespaceAliased(StringView a_strAlias) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the child namespaces of this namespace.
    ///
    /// \return the child namespace list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespaces const& getNamespaces() const { return m_Namespaces; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the namespace aliases referenced in this namespace.
    ///
    /// \return the namespace aliases list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Aliases const& getNamespaceAliases() const { return m_NamespaceAliases; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the namespace aliases referenced in this namespace.
    ///
    /// \return the namespace aliases list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbols const& getSymbols() const { return m_Symbols; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Converts this namespace name to a path with given separator.
    ///
    /// \param  separator   (optional) the separator.
    ///
    /// \return .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    String asPath(char separator = '/') const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given symbol has its name hidden by a higher scoped declaration.
    ///
    /// \param  a_pSymbol   The symbol to test.
    ///
    /// \return true if symbol hidden, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isSymbolHidden(Symbol* a_pSymbol) const override;

    void getScopedSymbolsWithName(StringView a_Name, Symbols& a_Symbols) const override;

    void getElementDoubles(Symbol* a_pElement, Symbols& out) const override;

    bool isSame(Symbol* a_pOther) const override { return a_pOther == this; }

    using Symbol::getQualifiedName;
    using Symbol::getQualifiedDecoratedName;
    using Symbol::getRelativeName;
    using Symbol::getRelativeDecoratedName;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

protected:
    virtual void onScopeSymbolAdded(Symbol* a_pElement) override;

private:
    Namespace* getNamespaceCascade(Strings& a_HierarchyWords) const;
    Namespace* getOrCreateNamespace(Strings* a_HierarchyWords);
    void       onNamespaceChanging(Namespace* a_pNamespace) override final;
    void       onNamespaceChanged(Namespace* a_pNamespace) override final;
    void       _registerSymbol(Symbol* a_pSym) { m_Symbols.push_back(a_pSym); }
    void       _unregisterSymbol(Symbol* a_pSym)
    {
        m_Symbols.erase_unsorted(std::find(m_Symbols.rbegin(), m_Symbols.rend(), a_pSym).base());
    }

private:
    Namespaces m_Namespaces;
    Aliases    m_NamespaceAliases;
    Symbols    m_Symbols;
};

} // namespace lang
} // namespace phantom
