// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#ifndef __DOXYGEN__
#    include <phantom/detail/core_internal.h>
#    include <phantom/utils/StringUtil.h>
#endif
#include "Alias.h"
#include "Application.h"
#include "Namespace.h"
#include "Namespace.hxx"
#include "Template.h"

#include <phantom/detail/core_internal.h>
#include <phantom/utils/Placement.h>
/* *********************************************** */
namespace phantom
{
namespace detail
{
}

namespace lang
{
extern lang::Namespace* g_pGlobalNamespace;

PHANTOM_DEFINE_META_CLASS(Namespace);

Namespace* Namespace::Global()
{
    return g_pGlobalNamespace;
}

Namespace::Namespace(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*=0*/)
    : Symbol("", a_Modifiers, (PHANTOM_R_ALWAYS_VALID | PHANTOM_R_INTERNAL_FLAG_SPECIAL | a_uiFlags)),
      Scope(this, nullptr)
{
}

Namespace::Namespace(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, (PHANTOM_R_ALWAYS_VALID | PHANTOM_R_INTERNAL_FLAG_SPECIAL | a_uiFlags)),
      Scope(this, nullptr)
{
}

void Namespace::initialize()
{
    Symbol::initialize();
    Scope::initialize();
    // namespaces are public by default
    setVisibility(Visibility::Public);
}

void Namespace::onScopeSymbolAdded(Symbol* a_pSym)
{
    a_pSym->setNamespace(this);
}

void Namespace::onScopeSymbolRemoving(Symbol* a_pSym)
{
    a_pSym->setNamespace(nullptr);
}

Namespace* Namespace::getNamespaceCascade(Strings& a_HierarchyWords) const
{
    String str = a_HierarchyWords.front();
    a_HierarchyWords.erase(a_HierarchyWords.begin());
    Namespace* childNamespace = getNamespace(str);
    if (!(childNamespace))
        return NULL;
    if (a_HierarchyWords.empty())
    {
        return childNamespace;
    }
    else
    {
        return childNamespace->getNamespaceCascade(a_HierarchyWords);
    }
    return NULL;
}

Namespace* Namespace::getOrCreateNamespace(Strings* a_HierarchyWords)
{
    if (a_HierarchyWords->empty())
        return this;
    String str = a_HierarchyWords->front();
    a_HierarchyWords->erase(a_HierarchyWords->begin());
    Namespace* pChildNamespace = getNamespace(str);
    if (!(pChildNamespace))
    {
        pChildNamespace = newNamespace(str);
    }
    if (a_HierarchyWords->empty())
    {
        return pChildNamespace;
    }
    else
    {
        return pChildNamespace->getOrCreateNamespace(a_HierarchyWords);
    }
    return NULL;
}

Namespace* Namespace::getOrCreateNamespace(StringView a_strNamespaceName, const char* separatorPattern)
{
    Strings words;
    StringUtil::Split(words, a_strNamespaceName, separatorPattern);
    return getOrCreateNamespace(&words);
}

Namespace* Namespace::newNamespace(StringView a_strName)
{
    Namespace* pNS = phantom::new_<Namespace>(a_strName);
    pNS->m_pSource = m_pSource;
    pNS->rtti.instance = pNS;
    if (dynamic_initializer_()->installed())
    {
        pNS->rtti.metaClass = PHANTOM_CLASSOF(Namespace);
        pNS->rtti.metaClass->registerInstance(pNS);
    }
    else
    {
        phantom::detail::deferInstallation("phantom::lang::Namespace", &pNS->rtti);
    }
    pNS->setNamespace(this);
    pNS->initialize();
    return pNS;
}

Namespace* Namespace::getNamespace(StringView a_strName) const
{
    for (Namespace* pNamespace : m_Namespaces)
    {
        if (pNamespace->getName() == a_strName)
            return pNamespace;
    }
    return NULL;
}

Alias* Namespace::getNamespaceAlias(StringView a_strName) const
{
    for (Alias* pNamespaceAlias : m_NamespaceAliases)
    {
        if (pNamespaceAlias->getName() == a_strName)
            return pNamespaceAlias;
    }
    return nullptr;
}

void Namespace::onNamespaceChanging(Namespace* /*a_pNamespace*/)
{
    Namespace* pParentNamespace = Symbol::getNamespace();
    auto       found =
    std::next(std::find(pParentNamespace->m_Namespaces.rbegin(), pParentNamespace->m_Namespaces.rend(), this)).base();
    PHANTOM_ASSERT(found != pParentNamespace->m_Namespaces.end(), "Namespace not found");
    pParentNamespace->m_Namespaces.erase_unsorted(found);
    setOwner(nullptr);
    setVisibility(Visibility::Private);
}

void Namespace::onNamespaceChanged(Namespace* /*a_pNamespace*/)
{
    Namespace* pParentNamespace = Symbol::getNamespace();
    setOwner(pParentNamespace);
    PHANTOM_ASSERT(pParentNamespace->getNamespace(getName()) == nullptr);
    PHANTOM_ASSERT(std::find(pParentNamespace->m_Namespaces.rbegin(), pParentNamespace->m_Namespaces.rend(), this) ==
                   pParentNamespace->m_Namespaces.rend(),
                   "Namespace not found");
    pParentNamespace->m_Namespaces.push_back(this);
    setVisibility(Visibility::Public);
}

void Namespace::addCustomSymbol(Symbol* a_pSymbol)
{
    a_pSymbol->setNamespace(this);
}

void Namespace::removeCustomSymbol(Symbol* a_pSymbol)
{
    PHANTOM_ASSERT(a_pSymbol->getNamespace() == this);
    a_pSymbol->setNamespace(nullptr);
}

Namespace* Namespace::getRootNamespace() const
{
    if (getOwner() == nullptr || getOwner() == Namespace::Global())
        return const_cast<Namespace*>(this);
    return getParentNamespace()->getRootNamespace();
}

#define PHANTOM_ASSERT_simple_name_collision(elem)                                                                     \
    PHANTOM_ASSERT(!hasElementWithName(elem->getName()),                                                               \
                   "An element has already been registered with name %s in scope %s", elem->getName().c_str(),         \
                   m_pThisElement->getQualifiedDecoratedName().c_str());

Alias* Namespace::addNamespaceAlias(StringView a_strAlias, Namespace* a_pNamespace)
{
    Alias* pAlias = Application::Get()->getDefaultSource()->addAlias(a_pNamespace, a_strAlias);
    addAlias(pAlias);
    m_NamespaceAliases.push_back(pAlias);
    return pAlias;
}

void Namespace::removeNamespaceAlias(StringView a_strAlias)
{
    m_NamespaceAliases.erase_unsorted(std::find_if(m_NamespaceAliases.begin(), m_NamespaceAliases.end(),
                                                   [=](Alias* a) { return a->getName() == a_strAlias; }));
}

Namespace* Namespace::getNamespaceCascade(StringView a_strQualifiedName, const char* a_SeparatorList) const
{
    Strings words;
    StringUtil::Split(words, a_strQualifiedName,
                      a_SeparatorList); // SplitVec == { "hello abc","ABC","aBc goodbye" }
    return getNamespaceCascade(words);
}

Namespace* Namespace::getNamespaceAliased(StringView a_strAlias) const
{
    Alias* pAlias = getNamespaceAlias(a_strAlias);
    return pAlias ? static_cast<Namespace*>(pAlias->getAliasedSymbol()) : nullptr;
}

String Namespace::asPath(char separator) const
{
    Namespace* pParent = getOwner() ? getOwner()->asNamespace() : nullptr;
    String     prefix = pParent ? pParent->asPath(separator) : "";
    return prefix.empty() ? m_strName : prefix + separator + m_strName;
}

void Namespace::getElementDoubles(Symbol* a_pElement, Symbols& out) const
{
    for (auto pSym : m_Symbols)
    {
        // Browse namespace elements to find doubles of given element
        if (pSym == a_pElement)
            continue;
        Symbol* pSymbol = a_pElement->asSymbol();
        if (pSymbol && a_pElement->getDecoratedName() == pSymbol->getDecoratedName())
        {
            PHANTOM_ASSERT(a_pElement->getModule() != pSymbol->getModule());
            out.push_back(pSymbol);
        }
    }
}

void Namespace::getQualifiedName(StringBuffer& a_Buf) const
{
    Namespace* pParent = getParentNamespace();
    if (pParent == nullptr || pParent == Namespace::Global())
        return getName(a_Buf);

    pParent->getQualifiedName(a_Buf);
    a_Buf += ':';
    a_Buf += ':';
    getName(a_Buf);
}

void Namespace::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    Namespace* pParent = getParentNamespace();
    if (pParent == nullptr || pParent == Namespace::Global() || a_pTo == pParent)
        return getName(a_Buf);
    pParent->getRelativeName(a_pTo, a_Buf);
    a_Buf += ':';
    a_Buf += ':';
    getName(a_Buf);
}

void Namespace::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    getRelativeName(a_pTo, a_Buf);
}

Namespace* Namespace::getParentNamespace() const
{
    return getOwner() ? getOwner()->asNamespace() : nullptr;
}

bool Namespace::isSymbolHidden(Symbol* a_pSymbol) const
{
    if (a_pSymbol->getNamespace() == this || a_pSymbol->getOwner() == this)
        return false;
    for (auto pRef : getSymbols())
    {
        if (pRef->asSymbol() && a_pSymbol->getName() == static_cast<Symbol*>(pRef)->getName())
            return true;
    }
    return getOwner() ? getOwner()->isSymbolHidden(a_pSymbol) : false;
}

void Namespace::getScopedSymbolsWithName(StringView a_Name, Symbols& a_Symbols) const
{
    Template* pTemplate = getTemplate(a_Name);
    if (pTemplate)
    {
        a_Symbols.push_back(pTemplate);
        return;
    }
    for (auto pSymbol : m_Symbols)
    {
        if (pSymbol->getName() == a_Name)
            a_Symbols.push_back(pSymbol);
    }
}

void Namespace::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    return getQualifiedName(a_Buf);
}

} // namespace lang

} // namespace phantom
