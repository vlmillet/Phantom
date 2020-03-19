// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "LanguageElement.h"

#include "Application.h"
#include "Class.h"
#include "LValueReference.h"
#include "Module.h"
#include "Namespace.h"
#include "TemplateSpecialization.h"
#include "phantom/detail/new.h"

#include <ostream>
#include <phantom/detail/StaticGlobals.h>
#include <phantom/traits/IntTypeBySize.h>
#include <phantom/utils/SmallSet.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static StaticGlobal<LanguageElements> g_pEmptyElements;

LanguageElement::LanguageElement(uint a_uiFlags /*= 0*/)
    : m_pOwner(nullptr),
      m_pElements(nullptr),
      m_pReferencingElements(nullptr),
      m_pReferencedElements(nullptr),
      m_uiFlags(a_uiFlags)
{
    Register(this);
}

LanguageElement::~LanguageElement()
{
    PHANTOM_ASSERT(!rtti.instance);
    PHANTOM_ASSERT(isNative() || m_pElements == nullptr);
    PHANTOM_ASSERT((m_uiFlags & PHANTOM_R_FLAG_TERMINATED) == PHANTOM_R_FLAG_TERMINATED);
}

int LanguageElement::destructionPriority() const
{
    if (this == rtti.metaClass || this == Application::Get())
        return std::numeric_limits<int>::max();
    return static_cast<LanguageElement*>(rtti.metaClass)->destructionPriority() - 1;
}

void LanguageElement::terminate()
{
    m_uiFlags |= PHANTOM_R_FLAG_TERMINATED;
    while (m_pReferencingElements)
    {
        m_pReferencingElements->back()->removeReferencedElement(this);
    }
    while (m_pReferencedElements)
    {
        removeReferencedElement(m_pReferencedElements->back());
    }

    while (m_pElements)
    {
        LanguageElement*   pElm = m_pElements->back();
        PHANTOM_DELETE_DYN pElm;
    }

    if (m_pOwner)
    {
        m_pOwner->removeElement(this);
    }
    m_pOwner = nullptr;

    Unregister(this);
    rtti.instance = nullptr;
}

void LanguageElement::fetchElements(LanguageElements& out, Class* a_pClass /*= nullptr*/) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            if (a_pClass == nullptr ||(*it)->as(a_pClass))
            {
                out.push_back(*it);
            }
        }
    }
}

LanguageElements const& LanguageElement::getElements() const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    return m_pElements ? *m_pElements : *g_pEmptyElements;
}

LanguageElements const& LanguageElement::getReferencedElements() const
{
    return m_pReferencedElements ? *m_pReferencedElements : *g_pEmptyElements;
}

LanguageElements const& LanguageElement::getReferencingElements() const
{
    return m_pReferencingElements ? *m_pReferencingElements : *g_pEmptyElements;
}

void LanguageElement::fetchElementsDeep(LanguageElements& out, Class* a_pClass /*= nullptr*/) const
{
    fetchElements(out, a_pClass);
    if (m_pElements)
    {
        for (size_t i = 0; i < m_pElements->size(); ++i)
        {
            (*m_pElements)[i]->fetchElementsDeep(out, a_pClass);
        }
    }
}

void LanguageElement::addElement(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_pElement);
    PHANTOM_ASSERT(a_pElement != this, "element added to itself");
    PHANTOM_ASSERT(a_pElement->m_pOwner == nullptr, "element already added to this or another element");
    PHANTOM_ASSERT(a_pElement->isNative() == isNative() ||(a_pElement->asNamespace() && asNamespace()) || a_pElement ==
                   Namespace::Global() || this == Application::Get(),
                   "adding non-native element to native one (and vice-versa) is forbidden");
    if (m_pElements == nullptr)
    {
        m_pElements = PHANTOM_NEW(LanguageElements);
    }
    m_pElements->push_back(a_pElement);
    a_pElement->setOwner(this);
    onElementAdded(a_pElement);
    if (a_pElement->isIncomplete())
    {
        setIncomplete();
    }
    if (a_pElement->isTemplateDependant() && asEvaluable())
    {
        setTemplateDependant();
    }
}

void LanguageElement::removeElement(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_pElement->m_pOwner == this);
    onElementRemoved(a_pElement);
    m_pElements->erase(std::find(m_pElements->begin(), m_pElements->end(), a_pElement));
    if (m_pElements->size() == 0)
    {
        PHANTOM_DELETE(LanguageElements) m_pElements;
        m_pElements = nullptr;
    }
    a_pElement->setOwner(nullptr);
}

void LanguageElement::addReferencedElement(LanguageElement* a_pElement)
{
    if (m_pReferencedElements == nullptr)
    {
        m_pReferencedElements = PHANTOM_NEW(LanguageElements);
    }
    // Allows call of this function even if already referenced, because it often happens
    // that the same element is used in both different place in the same referencer element
    // example : a type referenced as function return type and function parameter type
    if (std::find(m_pReferencedElements->begin(), m_pReferencedElements->end(), a_pElement) ==
        m_pReferencedElements->end())
    {
        m_pReferencedElements->push_back(a_pElement);
        a_pElement->registerReferencingElement(this);
        onReferencedElementAdded(a_pElement);
    }
    if (a_pElement->isIncomplete())
        setIncomplete();
    if (a_pElement->isTemplateDependant() && asEvaluable())
    {
        setTemplateDependant();
    }
}

void LanguageElement::addUniquelyReferencedElement(LanguageElement* a_pElement)
{
    if (m_pReferencedElements == nullptr)
    {
        m_pReferencedElements = PHANTOM_NEW(LanguageElements);
    }
    // Allows call of this function even if already referenced, because it often happens
    // that the same element is used in both different place in the same referencer element
    // example : a type referenced as function return type and function parameter type
    PHANTOM_ASSERT(std::find(m_pReferencedElements->begin(), m_pReferencedElements->end(), a_pElement) ==
                   m_pReferencedElements->end(),
                   "Element already referenced");
    m_pReferencedElements->push_back(a_pElement);
    a_pElement->registerReferencingElement(this);
    onReferencedElementAdded(a_pElement);
    if (a_pElement->isIncomplete())
        setIncomplete();
    if (a_pElement->isTemplateDependant() && asEvaluable())
    {
        setTemplateDependant();
    }
}

void LanguageElement::removeReferencedElement(LanguageElement* a_pElement)
{
    m_pReferencedElements->erase(std::find(m_pReferencedElements->begin(), m_pReferencedElements->end(), a_pElement));
    if (m_pReferencedElements->size() == 0)
    {
        PHANTOM_DELETE(LanguageElements) m_pReferencedElements;
        m_pReferencedElements = nullptr;
    }
    a_pElement->unregisterReferencingElement(this);
    onReferencedElementRemoved(a_pElement);
}

void LanguageElement::onReferencedElementAdded(LanguageElement*) {}

void LanguageElement::onReferencedElementRemoved(LanguageElement*) {}

void LanguageElement::registerReferencingElement(LanguageElement* a_pElement)
{
    if (m_pReferencingElements == nullptr)
    {
        m_pReferencingElements = PHANTOM_NEW(LanguageElements);
    }
    m_pReferencingElements->push_back(a_pElement);
}

void LanguageElement::unregisterReferencingElement(LanguageElement* a_pElement)
{
    m_pReferencingElements->erase(
    std::find(m_pReferencingElements->begin(), m_pReferencingElements->end(), a_pElement));
    if (m_pReferencingElements->size() == 0)
    {
        PHANTOM_DELETE(LanguageElements) m_pReferencingElements;
        m_pReferencingElements = nullptr;
    }
}

void LanguageElement::_nativeDetachElementsFromModule()
{
    PHANTOM_ASSERT(isNative());
    if (m_pElements)
    {
        for (auto elem : *m_pElements)
            elem->m_pOwner = nullptr;
        PHANTOM_DELETE(LanguageElements) m_pElements;
        m_pElements = nullptr;
    }
}

bool LanguageElement::canBeDestroyed() const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            if (!((*it)->canBeDestroyed()))
            {
                return false;
            }
        }
    }
    return true;
}

bool LanguageElement::canBeUnloaded() const
{
    if (!(canBeDestroyed()))
        return false;

    Module* pModule = getModule();

    if (!isNative())
    {
        if (m_pReferencingElements)
        {
            for (auto it = m_pReferencingElements->begin(); it != m_pReferencingElements->end(); ++it)
            {
                if (!((*it)->getModule() != pModule))
                {
                    return false; /// An element referencing this one belongs to a different module
                                  /// => cannot unload
                }
            }
        }
    }
    return true;
}

void LanguageElement::checkCompleteness() const {}

void LanguageElement::onElementRemoved(LanguageElement*) {}

Block* LanguageElement::getEnclosingBlock() const
{
    Block* pBlock;
    return m_pOwner ? ((pBlock = m_pOwner->asBlock()) ? pBlock : m_pOwner->getEnclosingBlock()) : nullptr;
}

Scope* LanguageElement::getEnclosingScope() const
{
    Scope* pScope;
    return m_pOwner ? ((pScope = m_pOwner->asScope()) ? pScope : m_pOwner->getEnclosingScope()) : nullptr;
}

ClassType* LanguageElement::getEnclosingClassType() const
{
    ClassType* pClassType;
    return m_pOwner ? ((pClassType = m_pOwner->asClassType()) ? pClassType : m_pOwner->getEnclosingClassType())
                    : nullptr;
}

Namespace* LanguageElement::getEnclosingNamespace() const
{
    Namespace* pNamespace;
    Scope*     pNamingScope = getNamingScope();
    return pNamingScope
    ? ((pNamespace = pNamingScope->asNamespace()) ? pNamespace
                                                  : pNamingScope->asLanguageElement()->getEnclosingNamespace())
    : nullptr;
}

Subroutine* LanguageElement::getEnclosingSubroutine() const
{
    Subroutine* pNamespace;
    return m_pOwner ? ((pNamespace = m_pOwner->asSubroutine()) ? pNamespace : m_pOwner->getEnclosingSubroutine())
                    : nullptr;
}

TemplateSpecialization* LanguageElement::getEnclosingTemplateSpecialization() const
{
    TemplateSpecialization* pTemplateSpecialization;
    return m_pOwner ? ((pTemplateSpecialization = m_pOwner->asTemplateSpecialization())
                       ? pTemplateSpecialization
                       : m_pOwner->getEnclosingTemplateSpecialization())
                    : nullptr;
}

Statement* LanguageElement::getEnclosingStatement() const
{
    return m_pOwner ? (m_pOwner->asStatement() ? m_pOwner->asStatement() : m_pOwner->getEnclosingStatement()) : nullptr;
}

void LanguageElement::_onAncestorChanged(LanguageElement* a_pAncestor)
{
    if (m_pElements)
    {
        LanguageElements elementsCopy(m_pElements->begin(),
                                      m_pElements->end()); /// copy to allow element adding during recursive deep call
        for (auto it = elementsCopy.begin(); it != elementsCopy.end(); ++it)
        {
            (*it)->_onAncestorChanged(a_pAncestor);
        }
    }
    //             if (Source* pSource = a_pAncestor->asSource())
    //             {
    //                 if ((m_uiFlags & PHANTOM_R_FLAG_IMPLICIT) == 0)
    //                 {
    //                     pSource->markOutdated(); // we outdate the source from the moment a
    //                     non-implicit element is added to it
    //                 }
    //             }
    onAncestorChanged(a_pAncestor);
}

void LanguageElement::_onAncestorAboutToBeChanged(LanguageElement* a_pOwner)
{
    if (m_pElements)
    {
        LanguageElements elementsCopy(m_pElements->begin(),
                                      m_pElements->end()); /// copy to allow element adding during recursive deep call
        for (auto it = elementsCopy.begin(); it != elementsCopy.end(); ++it)
        {
            (*it)->_onAncestorAboutToBeChanged(a_pOwner);
        }
    }
    onAncestorAboutToBeChanged(a_pOwner);
}

void LanguageElement::_onElementsAccess()
{
    Module* pModule;
    if (Application::Get() && !(Application::Get()->testFlags(PHANTOM_R_FLAG_TERMINATED)) &&
        (pModule = getModule()) && !(pModule->testFlags(PHANTOM_R_FLAG_TERMINATED)))
    {
        onElementsAccess();
    }
}

void LanguageElement::setOwner(LanguageElement* a_pOwner)
{
    PHANTOM_ASSERT(m_pOwner != a_pOwner);
    if (m_pOwner)
    {
        LanguageElement* pAncestor = m_pOwner;
        while (pAncestor)
        {
            _onAncestorAboutToBeChanged(pAncestor);
            pAncestor = pAncestor->m_pOwner;
        }
    }
    m_pOwner = a_pOwner;
    if (m_pOwner)
    {
        LanguageElement* pAncestor = m_pOwner;
        while (pAncestor)
        {
            _onAncestorChanged(pAncestor);
            pAncestor = pAncestor->m_pOwner;
        }
    }
}

void LanguageElement::onAncestorChanged(LanguageElement*) {}

void LanguageElement::onAncestorAboutToBeChanged(LanguageElement*) {}

LanguageElement* LanguageElement::removeExpression() const
{
    return const_cast<LanguageElement*>(this);
}

Symbol* LanguageElement::removeExpressionAsSymbol() const
{
    return removeExpression()->asSymbol();
}

LanguageElement* LanguageElement::hatchExpression()
{
    LanguageElement* pLanguageElement = removeExpression();
    if (pLanguageElement == this)
        return this;
    PHANTOM_DELETE_DYN this;
    return pLanguageElement;
}

void LanguageElement::onInvalidated() {}

Reference* LanguageElement::asConstLValueReference() const
{
    Reference* pReference = asLValueReference();
    return (pReference && pReference->getReferencedType()->asConstType()) ? pReference : nullptr;
}

bool LanguageElement::partialAccepts(LanguageElement* a_pLanguageElement, size_t& a_Score, PlaceholderMap&) const
{
    if (a_pLanguageElement == this)
    {
        a_Score = ~size_t(0);
        return true;
    }
    return false;
}

bool LanguageElement::isSame(LanguageElement* a_pOther) const
{
    return (this == a_pOther);
}

void LanguageElement::addScopedElement(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_pElement->m_pOwner == nullptr);
    a_pElement->m_pOwner = this;
    addReferencedElement(a_pElement);
}

void LanguageElement::removeScopedElement(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_pElement->m_pOwner == this);
    a_pElement->m_pOwner = nullptr;
    removeReferencedElement(a_pElement);
}

void LanguageElement::replaceElement(LanguageElement* a_pOldElement, LanguageElement* a_pNewElement)
{
    *std::find(m_pElements->begin(), m_pElements->end(), a_pOldElement) = a_pNewElement;
}

void LanguageElement::fetchReferencedModules(SmallSet<Module*>& a_Modules) const
{
    if (m_pReferencedElements)
    {
        for (auto it = m_pReferencedElements->begin(); it != m_pReferencedElements->end(); ++it)
        {
            a_Modules.insert((*it)->getModule());
        }
    }
}

void LanguageElement::fetchReferencingModules(SmallSet<Module*>& a_Modules) const
{
    if (m_pReferencingElements)
    {
        for (auto it = m_pReferencingElements->begin(); it != m_pReferencingElements->end(); ++it)
        {
            if ((*it)->getModule())
            {
                a_Modules.insert((*it)->getModule());
            }
        }
    }
}

void LanguageElement::fetchReferencedModulesDeep(SmallSet<Module*>& a_Modules) const
{
    fetchReferencedModules(a_Modules);
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            (*it)->fetchReferencedModulesDeep(a_Modules);
        }
    }
}

void LanguageElement::fetchReferencingModulesDeep(SmallSet<Module*>& a_Modules) const
{
    fetchReferencingModules(a_Modules);
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            (*it)->fetchReferencingModulesDeep(a_Modules);
        }
    }
}

void LanguageElement::getElements(LanguageElements& a_Elements) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        a_Elements.insert(a_Elements.end(), m_pElements->begin(), m_pElements->end());
    }
}

void LanguageElement::fetchSymbols(Symbols& a_Symbols) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            Symbol* pSymbol = (*it)->asSymbol();
            if (pSymbol)
            {
                a_Symbols.push_back(pSymbol);
            }
        }
    }
}

void LanguageElement::getElementsDeep(LanguageElements& a_Elements) const
{
    getElements(a_Elements);
    if (m_pElements)
    {
        for (size_t i = 0; i < m_pElements->size(); ++i)
        {
            (*m_pElements)[i]->getElementsDeep(a_Elements);
        }
    }
}

bool LanguageElement::hasReferencedElement(LanguageElement* a_pLanguageElement) const
{
    if (m_pReferencedElements)
    {
        for (auto it = m_pReferencedElements->begin(); it != m_pReferencedElements->end(); ++it)
        {
            if (*it == a_pLanguageElement)
                return true;
        }
    }
    return false;
}

bool LanguageElement::hasReferencingElement(LanguageElement* a_pLanguageElement) const
{
    if (m_pReferencingElements)
    {
        for (auto it = m_pReferencingElements->begin(); it != m_pReferencingElements->end(); ++it)
        {
            if (*it == a_pLanguageElement)
                return true;
        }
    }
    return false;
}

void LanguageElement::dumpElementList(std::basic_ostream<char>& out) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            out << (*it)->getUniqueName() << std::endl;
        }
    }
}

void LanguageElement::dumpElementListCascade(std::basic_ostream<char>& out) const
{
    dumpElementList(out);
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            (*it)->dumpElementListCascade(out);
        }
    }
}

bool LanguageElement::hasFriendCascade(Symbol* a_pElement) const
{
    return hasFriend(a_pElement) ||(m_pOwner && m_pOwner->hasFriendCascade(a_pElement));
}

void LanguageElement::addSymbol(Symbol* a_pElement)
{
    // first add the element
    addElement(a_pElement);

#if PHANTOM_DEBUG_LEVEL != PHANTOM_DEBUG_LEVEL_FULL
    if (!isNative())
#endif
    {
        /// If native => check doubles only in debug version and assert if needed
        if (a_pElement->getName().size()) /// we accept anonymous doubles
        {
            for (auto pElm : getElements())
            {
                if (pElm == a_pElement)
                    continue;
                Symbol* pSymbol = pElm->asSymbol();
                (void)pSymbol;
                PHANTOM_ASSERT_DEBUG(pSymbol == nullptr || pSymbol->computeHash() != a_pElement->computeHash(),
                                     "equal element already added : be careful not having "
                                     "duplicate member declarations in your class, or check not "
                                     "registering not two type with same name in the same source");
            };
        }
    }
}

bool LanguageElement::isTemplateElement() const
{
    TemplateSpecialization* pSpec = getEnclosingTemplateSpecialization();
    return pSpec            &&(!(pSpec->isFull()) || pSpec->isTemplateElement());
}

LanguageElements LanguageElement::sm_Elements; // TODO remove

// void setElementMap(element_map a_element_map)
// {
//     uint guid;
//     element_container::iterator it = g_elements->begin();
//     element_container::iterator end = g_elements->end();
//     for (; it != end; it++)
//     {
//         guid = a_element_map[(*it)->getQualifiedDecoratedName()];
//         (*it)->setGuid(guid);
//     }
// }

//
// element_map getElementMap()
// {
//     element_map element_map;
//     element_container::iterator it = g_elements->begin();
//     element_container::iterator end = g_elements->end();
//     for (; it != end; it++)
//     {
//         element_map[(*it)->getQualifiedDecoratedName()] = (*it)->getGuid();
//     }
//
//     return element_map;
// }
//
// void Phantom::updateLanguageElementGuid()
// {
//     static uint s_uitransientGuid = 0;
//
//     element_container::iterator it = g_elements->begin();
//     element_container::iterator end = g_elements->end();
//     for (; it != end; it++)
//     {
//         s_uitransientGuid++;
//         (*it)->setGuid(s_uitransientGuid);
//     }
// }

Symbol* LanguageElement::getUniqueElement(StringView name, Modifiers modifiers /*= 0*/, uint /*= 0*/) const
{
    Symbol* pElement = nullptr;
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            Symbol* pSymbol = (*it)->asSymbol();
            if (pSymbol && pSymbol->getName() == name && pSymbol->testModifiers(modifiers))
            {
                if (pElement)
                    return nullptr;
                else
                    pElement = pSymbol;
            }
        }
    }
    return pElement;
}

size_t LanguageElement::getElementIndex(LanguageElement* a_pElement) const
{
    if (m_pElements)
    {
        size_t count = m_pElements->size();
        for (size_t i = 0; i < count; ++i)
        {
            if ((*m_pElements)[i] == a_pElement)
                return i;
        }
    }
    return ~size_t(0);
}

bool LanguageElement::hasNamingScopeCascade(Scope* a_pScope) const
{
    Scope* pScope = getNamingScope();
    return (pScope != nullptr)
    &&((pScope == a_pScope) || pScope->asLanguageElement()->hasNamingScopeCascade(a_pScope));
}

void LanguageElement::onElementAdded(LanguageElement*) {}

void LanguageElement::clear()
{
    while (m_pElements)
    {
        LanguageElement* pElement = m_pElements->back();
        removeElement(pElement);
        PHANTOM_DELETE_DYN pElement;
    }
}

void LanguageElement::steal(LanguageElement* a_pInput)
{
    // backup this elements
    LanguageElements backup = getElements();

    // remove this elements
    while (m_pElements)
    {
        LanguageElement* pElement = m_pElements->front();
        removeElement(pElement);
    }

    // steal input elements while removing to it
    while (a_pInput->m_pElements)
    {
        LanguageElement* pElement = a_pInput->m_pElements->front();
        a_pInput->removeElement(pElement);
        addElement(pElement);
    }

    // swap backup elements to input
    for (auto pElm : backup)
    {
        a_pInput->addElement(pElm);
    }
}

void LanguageElement::getName(StringBuffer&) const {}

String LanguageElement::getName() const
{
    StringBuffer cstr;
    getName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getQualifiedName(StringBuffer&) const {}

String LanguageElement::getQualifiedName() const
{
    StringBuffer cstr;
    getQualifiedName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getDecoratedName(StringBuffer&) const {}

String LanguageElement::getDecoratedName() const
{
    StringBuffer cstr;
    getDecoratedName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getQualifiedDecoratedName(StringBuffer&) const {}

String LanguageElement::getQualifiedDecoratedName() const
{
    StringBuffer cstr;
    getQualifiedDecoratedName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getUniqueName(StringBuffer&) const {}

Scope* LanguageElement::getNamingScope() const
{
    Scope* pScope;
    return m_pOwner ? ((pScope = m_pOwner->asScope()) ? pScope : m_pOwner->getNamingScope()) : nullptr;
}

void LanguageElement::detach()
{
    if (m_pOwner)
    {
        m_pOwner->removeElement(this);
    }
}

bool LanguageElement::isSymbolHidden(Symbol* a_pSymbol) const
{
    if (a_pSymbol->getOwner() == this)
        return false;
    for (auto p : getElements())
    {
        if (p->asSymbol() && a_pSymbol->getName() == static_cast<Symbol*>(p)->getName())
            return true;
    }
    return m_pOwner ? m_pOwner->isSymbolHidden(a_pSymbol) : false;
}

void LanguageElement::getSymbolsWithName(StringView a_strName, Symbols& a_OutSymbols) const
{
    for (auto p : getElements())
    {
        Symbol* pSymbol = p->asSymbol();
        if (pSymbol && pSymbol->getName() == a_strName && !(pSymbol->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)))
            a_OutSymbols.push_back(pSymbol);
    }
}

void LanguageElement::getSymbols(Symbols& a_OutSymbols) const
{
    for (auto p : getElements())
    {
        if (Symbol* pSymbol = p->asSymbol())
            a_OutSymbols.push_back(pSymbol);
    }
}

bool LanguageElement::hasSymbol(StringView a_strName) const
{
    for (auto p : getElements())
    {
        Symbol* pSymbol = p->asSymbol();
        if (pSymbol && pSymbol->getName() == a_strName)
            return true;
    }
    return false;
}

phantom::lang::Module* LanguageElement::getModule() const
{
    Module* pModule = asModule();
    return pModule ? pModule : (m_pOwner ? m_pOwner->getModule() : nullptr);
}

phantom::lang::Package* LanguageElement::getPackage() const
{
    Package* pPackage = asPackage();
    return pPackage ? pPackage : (m_pOwner ? m_pOwner->getPackage() : nullptr);
}

bool LanguageElement::isIncomplete() const
{
    return testFlags(PHANTOM_R_INCOMPLETE);
}

void LanguageElement::setCodeRange(const CodeRange& a_CodeRange)
{
    if (m_CodeRange == a_CodeRange)
        return;
    m_CodeRange = a_CodeRange;
    //             if (m_pOwner && !(asSource()))
    //             {
    //                 m_pOwner->setCodeRange(m_pOwner->m_CodeRange | a_CodeRange);
    //             }
}

Source* LanguageElement::getCodeLocationSource() const
{
    return m_pOwner ? m_pOwner->getCodeLocationSource() : nullptr;
}

LanguageElement* LanguageElement::getElementAtCodePosition(const CodePosition& a_CodePosition) const
{
    if (!(m_CodeRange.containsCodePosition(a_CodePosition)))
        return nullptr;
    for (auto pElement : getElements())
    {
        LanguageElement* pChild = pElement->getElementAtCodePosition(a_CodePosition);
        if (pChild)
            return pChild;
    }
    return const_cast<LanguageElement*>(this);
}

LanguageElement* LanguageElement::getElementAtLine(uint16 a_Line) const
{
    if (!(m_CodeRange.containsLine(a_Line)))
        return nullptr;
    for (auto pElement : getElements())
    {
        LanguageElement* pChild = pElement->getElementAtLine(a_Line);
        if (pChild)
            return pChild;
    }
    return const_cast<LanguageElement*>(this);
}

void LanguageElement::fetchSymbols(Symbols& a_Symbols, SymbolFilter a_Filter,
                                   bool a_bIncludeUnamedSubSymbols /*= true */, bool a_bRecursive /*= false*/) const
{
    for (auto pElem : getElements())
    {
        Symbol* pSymbol1 = pElem->asSymbol();
        if (pSymbol1 &&(pSymbol1 = a_Filter(pSymbol1, asSymbol() ? asSymbol()->getName().empty() : false)))
        {
            a_Symbols.push_back(pSymbol1);
            if (pSymbol1->getName().empty())
            {
                if (a_bRecursive)
                {
                    if (a_bIncludeUnamedSubSymbols)
                        pSymbol1->fetchSymbols(a_Symbols, a_Filter, a_bIncludeUnamedSubSymbols);
                    continue;
                }
            }
        }
        if (a_bRecursive)
        {
            pElem->fetchSymbols(a_Symbols, a_Filter, a_bIncludeUnamedSubSymbols, a_bRecursive);
        }
    }
}

void LanguageElement::visit(phantom::lang::LanguageElementVisitor* a_pVisitor, VisitorData a_Data)
{
    static_cast<phantom::lang::LanguageElementVisitor*>(a_pVisitor)->visit(this, a_Data);
}

Symbol* LanguageElement::PublicFilter(Symbol* a_pSymbol, bool)
{
    return a_pSymbol->isPublic() ? a_pSymbol : nullptr;
}

Symbol* LanguageElement::PublicIfUnamedSubSymbolFilter(Symbol* a_pSymbol, bool a_bUnamedSubSymbol)
{
    return (!a_bUnamedSubSymbol || a_pSymbol->isPublic()) ? a_pSymbol : nullptr;
}

void LanguageElement::setIncomplete()
{
    if (testFlags(PHANTOM_R_INCOMPLETE) || testFlags(PHANTOM_R_ALWAYS_VALID))
        return;
    /// When an element becomes invalid, the following becomes invalid too :
    /// - the elements who have reference to it
    /// - its owner element
    /// - its children elements
    m_uiFlags |= PHANTOM_R_INCOMPLETE;
    if (m_pReferencingElements)
    {
        for (auto it = m_pReferencingElements->begin(); it != m_pReferencingElements->end(); ++it)
        {
            (*it)->setIncomplete();
        }
    }
    if (m_pOwner)
        m_pOwner->setIncomplete();
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            (*it)->setIncomplete();
        }
    }
}

phantom::lang::Source* LanguageElement::getSource() const
{
    Source* pSource = asSource();
    return pSource ? pSource : (m_pOwner ? m_pOwner->getSource() : nullptr);
}

void LanguageElement::setInvalid()
{
    if (isInvalid() || isAlwaysValid())
        return;
    PHANTOM_ASSERT(!isNative());
    m_uiFlags |= PHANTOM_R_FLAG_INVALID;
    onInvalidated();

    setIncomplete();
}

} // namespace lang
} // namespace phantom
