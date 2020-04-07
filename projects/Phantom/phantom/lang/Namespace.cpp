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
extern RawPlacement<lang::Namespace> g_pGlobalNamespace;
}

namespace lang
{
PHANTOM_DEFINE_META_CLASS(Namespace);

Namespace* Namespace::Global()
{
    return phantom::detail::g_pGlobalNamespace;
}

Namespace::Namespace(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*=0*/)
    : Symbol("", a_Modifiers, (a_uiFlags & PHANTOM_R_FLAG_INVALID) ? a_uiFlags : (PHANTOM_R_ALWAYS_VALID | a_uiFlags)),
      Scope(this)
{
}

Namespace::Namespace(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers,
             (a_uiFlags & PHANTOM_R_FLAG_INVALID) ? a_uiFlags : (PHANTOM_R_ALWAYS_VALID | a_uiFlags)),
      Scope(this)
{
}

Namespace::~Namespace() {}

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

Namespace* Namespace::findOrCreateNamespace(Strings* a_HierarchyWords)
{
    if (a_HierarchyWords->empty())
        return this;
    String str = a_HierarchyWords->front();
    a_HierarchyWords->erase(a_HierarchyWords->begin());
    Namespace* pChildNamespace = getNamespace(str);
    if (!(pChildNamespace))
    {
#if defined(PHANTOM_STATIC_LIB_HANDLE)
        pChildNamespace = PHANTOM_DEFERRED_NEW(Namespace)(str);
#else
        pChildNamespace = PHANTOM_DEFERRED_NEW(Namespace)(str);
#endif
        addNamespace(pChildNamespace);
    }
    if (a_HierarchyWords->empty())
    {
        return pChildNamespace;
    }
    else
    {
        return pChildNamespace->findOrCreateNamespace(a_HierarchyWords);
    }
    return NULL;
}

Namespace* Namespace::findOrCreateNamespace(StringView a_strNamespaceName, const char* separatorPattern)
{
    Strings words;
    StringUtil::Split(words, a_strNamespaceName, separatorPattern);
    return findOrCreateNamespace(&words);
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

void Namespace::release(Types& out_types)
{
    Scope::release(out_types);
    while (!m_Namespaces.empty())
    {
        Namespace* pNamespace = m_Namespaces.back();
        pNamespace->release(out_types);
        removeNamespace(pNamespace);
        PHANTOM_DELETE_DYN pNamespace;
    }
}

void Namespace::addNamespace(Namespace* a_pNamespace)
{
    PHANTOM_ASSERT_NOT(getNamespace(a_pNamespace->getName()));
    m_Namespaces.push_back(a_pNamespace);
    addSymbol(a_pNamespace);
    if (Application::Get()->getModules().size())
    {
        PHANTOM_EMIT namespaceAdded(a_pNamespace);
    }
}

void Namespace::removeNamespace(Namespace* a_pNamespace)
{
    removeElement(a_pNamespace);
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
#if defined(PHANTOM_STATIC_LIB_HANDLE)
    Alias* pAlias = PHANTOM_DEFERRED_NEW(Alias)(a_pNamespace, a_strAlias);
#else
    Alias* pAlias = PHANTOM_DEFERRED_NEW(Alias)(a_pNamespace, a_strAlias);
#endif
    addAlias(pAlias);
    m_NamespaceAliases.push_back(pAlias);
    return pAlias;
}

void Namespace::removeNamespaceAlias(StringView a_strAlias)
{
    for (auto it = m_NamespaceAliases.begin(); it != m_NamespaceAliases.end(); ++it)
    {
        if ((*it)->getName() == a_strAlias)
        {
            removeElement(*it);
            return;
        }
    }
    PHANTOM_ASSERT(false, "namespace alias not found");
}

Namespace* Namespace::getNamespaceCascade(StringView a_strQualifiedName, const char* a_SeparatorList) const
{
    Strings words;
    StringUtil::Split(words, a_strQualifiedName,
                      a_SeparatorList); // SplitVec == { "hello abc","ABC","aBc goodbye" }
    return getNamespaceCascade(words);
}

void Namespace::onElementRemoved(LanguageElement* a_pElement)
{
    LanguageElement::onElementRemoved(a_pElement);
    if (a_pElement->asNamespace())
    {
        Namespace* pNamespace = static_cast<Namespace*>(a_pElement);
        PHANTOM_ASSERT(pNamespace->getParentNamespace() == this, "This namespace is attached to another Namespace");
        Namespaces::iterator found = std::find(m_Namespaces.begin(), m_Namespaces.end(), pNamespace);
        PHANTOM_ASSERT(found != m_Namespaces.end(), "Namespace not found");
        m_Namespaces.erase(found);
        if (Application::Get()->getModules().size())
        {
            PHANTOM_EMIT namespaceRemoved(pNamespace);
        }
    }
}

void Namespace::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Scope::scopedElementRemoved(a_pElement);
    LanguageElement::onReferencedElementRemoved(a_pElement);
    if (Symbol* pSymbol = a_pElement->asSymbol())
    {
        if (pSymbol->m_pNamespace == this)
            pSymbol->m_pNamespace = nullptr;
    }
    if (a_pElement->asNamespace())
        for (auto it = m_Namespaces.begin(); it != m_Namespaces.end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_Namespaces.erase(it);
                if (Application::Get()->getModules().size())
                {
                    PHANTOM_EMIT namespaceRemoved(*it);
                }
                return;
            }
        }
    else if (a_pElement->asAlias())
        for (auto it = m_NamespaceAliases.begin(); it != m_NamespaceAliases.end(); ++it)
        {
            if (*it == a_pElement)
            {
                m_NamespaceAliases.erase(it);
                if (Application::Get()->getModules().size())
                {
                    PHANTOM_EMIT namespaceAliasRemoved(*it);
                }
                return;
            }
        }
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

void Namespace::addScopeElement(Symbol* a_pElement)
{
    PHANTOM_ASSERT(a_pElement->m_pNamespace == nullptr);
    if (a_pElement->isNative())
        m_uiFlags |= PHANTOM_R_FLAG_NATIVE; // a namespace become native once any native element is
                                            // added to iterencedElements)
    a_pElement->m_pNamespace = this;
    addUniquelyReferencedElement(a_pElement);
    Scope::scopedElementAdded(a_pElement);
}

void Namespace::removeScopeElement(Symbol* a_pElement)
{
    PHANTOM_ASSERT(a_pElement->m_pNamespace == this);
    removeReferencedElement(a_pElement);
}

void Namespace::getElementDoubles(Symbol* a_pElement, Symbols& out) const
{
    if (m_pReferencedElements)
        for (auto it = m_pReferencedElements->begin(); it != m_pReferencedElements->end(); ++it)
        {
            // Browse namespace elements to find doubles of given element
            if (*it == a_pElement)
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
    for (auto pRef : getReferencedElements())
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
    LanguageElement::getSymbolsWithName(a_Name, a_Symbols);
    for (auto p : getReferencedElements())
    {
        Symbol* pSymbol = p->asSymbol();
        if (pSymbol && pSymbol->getName() == a_Name && !(pSymbol->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)))
            a_Symbols.push_back(pSymbol);
    }
}

void Namespace::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    return getQualifiedName(a_Buf);
}

} // namespace lang

} // namespace phantom
