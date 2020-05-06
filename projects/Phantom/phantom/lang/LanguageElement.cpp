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

#include <ostream>
#include <phantom/detail/StaticGlobals.h>
#include <phantom/traits/IntTypeBySize.h>
#include <phantom/utils/SmallSet.h>

#if PHANTOM_CONSISTENCY_CHECK_ENABLED
#    define PHANTOM_CONSISTENCY_CHECK_ASSERT PHANTOM_ASSERT
#    define PHANTOM_CONSISTENCY_CHECK_CODE(...) __VA_ARGS__
#else
#    define PHANTOM_CONSISTENCY_CHECK_ASSERT(...)
#    define PHANTOM_CONSISTENCY_CHECK_CODE(...)
#endif

/* *********************************************** */
namespace phantom
{
namespace lang
{
LanguageElement::LanguageElement(uint a_uiFlags /*= 0*/) : m_pOwner(nullptr), m_uiFlags(a_uiFlags) {}

void LanguageElement::initialize()
{
    m_Elements.setAllocator(getAllocator());
}

LanguageElement::~LanguageElement()
{
    PHANTOM_ASSERT((m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATED) != 0,
                   "missing super call to terminate() somewhere");
    if (m_pSource)
    {
        if (m_pSource->m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATING)
        {
            m_Elements.clear(); // ensure deallocation will be made with still valid allocator in Source
            return;
        }
        auto found =
        std::next(std::find(m_pSource->m_CreatedElements.rbegin(), m_pSource->m_CreatedElements.rend(), this)).base();
        PHANTOM_CONSISTENCY_CHECK_ASSERT(found != m_pSource->m_CreatedElements.end());
        m_pSource->m_CreatedElements.erase_unsorted(found);
    }

    size_t i = m_Elements.size();
    while (i--)
    {
        m_Elements[i]->~LanguageElement();
    }
}

int LanguageElement::destructionPriority() const
{
    if (this == rtti.metaClass || this == Application::Get())
        return std::numeric_limits<int>::max();
    return static_cast<LanguageElement*>(rtti.metaClass)->destructionPriority() - 1;
}

void LanguageElement::terminate()
{
    if (m_pSource && (m_pSource->m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATING))
    {
        m_uiFlags |= PHANTOM_R_INTERNAL_FLAG_TERMINATED;
        return;
    }
    PHANTOM_CONSISTENCY_CHECK_ASSERT((m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATED) == 0);
    m_uiFlags |= PHANTOM_R_INTERNAL_FLAG_TERMINATED;
    setOwner(nullptr);
    size_t i = m_Elements.size();
    while (i--)
    {
        m_Elements[i]->rtti.metaClass->unregisterInstance(m_Elements[i]->rtti.instance);
        m_Elements[i]->_terminate();
    }
}

void LanguageElement::fetchElements(LanguageElements& out, Class* a_pClass /*= nullptr*/) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    for (auto& m_pElement : m_Elements)
    {
        if (a_pClass == nullptr || m_pElement->as(a_pClass))
        {
            out.push_back(m_pElement);
        }
    }
}

CustomAllocator const* LanguageElement::getAllocator() const
{
    if (testFlags(PHANTOM_R_INTERNAL_FLAG_SPECIAL))
        return &CustomAllocator::CurrentOrDefault();
    PHANTOM_ASSERT(m_pSource);
    return m_pSource->Source::getAllocator();
}

void LanguageElement::Delete(LanguageElement* a_pElem)
{
    PHANTOM_CONSISTENCY_CHECK_ASSERT(a_pElem->m_pOwner == this);
    PHANTOM_CONSISTENCY_CHECK_ASSERT((!m_pSource || !m_pSource->testFlags(PHANTOM_R_INTERNAL_FLAG_TERMINATING)),
                                     "never invoke Delete yourself in terminate(), it's the Source's job to do that");
    PHANTOM_CONSISTENCY_CHECK_ASSERT(m_pSource);
    m_pSource->Delete(a_pElem);
    // --free(...)-- we never deallocate individually
    // (we always deallocate per-source(script) or per-module(c++) chunks for speed)
}

LanguageElementsView LanguageElement::getElements() const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    return m_Elements;
}

void LanguageElement::fetchElementsDeep(LanguageElements& out, Class* a_pClass /*= nullptr*/) const
{
    fetchElements(out, a_pClass);
    for (size_t i = 0; i < m_Elements.size(); ++i)
    {
        (m_Elements)[i]->fetchElementsDeep(out, a_pClass);
    }
}

void LanguageElement::_nativeDetachElementsFromModule()
{
    PHANTOM_ASSERT(isNative());
    for (auto elem : m_Elements)
        elem->m_pOwner = nullptr;
}

bool LanguageElement::canBeDestroyed() const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    for (auto pElem : getElements())
    {
        if (!(pElem->canBeDestroyed()))
        {
            return false;
        }
    }
    return true;
}

bool LanguageElement::canBeUnloaded() const
{
    if (!(canBeDestroyed()))
        return false;

    //     Module* pModule = getModule();
    //
    //     if (!isNative())
    //     {
    //         //         if (m_pReferencingElements)
    //         //         {
    //         //             for (auto it = m_pReferencingElements->begin(); it != m_pReferencingElements->end(); ++it)
    //         //             {
    //         //                 if (!((*it)->getModule() != pModule))
    //         //                 {
    //         //                     return false; /// An element referencing this one belongs to a different module
    //         //                                   /// => cannot unload
    //         //                 }
    //         //             }
    //         //         }
    //     }
    return true;
}

void LanguageElement::checkCompleteness() const {}

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
    Namespace*       pNamespace;
    LanguageElement* pNamingScope = getNamingScope();
    return pNamingScope
    ? ((pNamespace = pNamingScope->asNamespace()) ? pNamespace : pNamingScope->getEnclosingNamespace())
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

void LanguageElement::_onElementsAccess()
{
    if (testFlags(PHANTOM_R_INTERNAL_FLAG_SPECIAL))
        return;
    onElementsAccess();
}

void LanguageElement::setOwner(LanguageElement* a_pOwner)
{
    if (m_pOwner == a_pOwner)
        return;

    if (m_pOwner && (m_pOwner->m_uiFlags & PHANTOM_R_INTERNAL_FLAG_TERMINATING) == 0)
    {
        m_pOwner->onElementsAccess();

        // we search for element in reverse order because generally we wan't to change ownership of a recently added
        // element
        m_pOwner->m_Elements.erase_unsorted(
        std::next(std::find(m_pOwner->m_Elements.rbegin(), m_pOwner->m_Elements.rend(), this)).base());
        m_pOwner->m_uiFlags &= ~PHANTOM_R_FLAG_TEMPLATE_DEPENDANT; // FIXME : add a template dependant tracking counter
    }
    m_pOwner = a_pOwner;
    if (m_pOwner)
    {
        PHANTOM_ASSERT(m_pOwner->m_pSource == m_pSource || m_pSource == this);
        m_pOwner->onElementsAccess();
        m_pOwner->m_Elements.push_back(this);
        if (isTemplateDependant() && m_pOwner->asEvaluable())
        {
            m_pOwner->m_uiFlags |= PHANTOM_R_FLAG_TEMPLATE_DEPENDANT;
        }

#if PHANTOM_DEBUG_LEVEL != PHANTOM_DEBUG_LEVEL_FULL
        if (!m_pOwner->isNative())
#endif
        {
            /// If native => check doubles only in debug version and assert if needed
            if (!getName().empty() && asSymbol()) /// we accept anonymous doubles
            {
                for (auto pElm : m_pOwner->getElements())
                {
                    if (pElm == this)
                        continue;
                    Symbol* pSymbol = pElm->asSymbol();
                    (void)pSymbol;
                    PHANTOM_ASSERT_DEBUG(pSymbol == nullptr || pSymbol->getVisibility() == Visibility::Private ||
                                         asSymbol()->getVisibility() == Visibility::Private ||
                                         pSymbol->getDecoratedName() != getDecoratedName(),
                                         "equal element already added : be careful not having "
                                         "duplicate member declarations in your class, or check not "
                                         "registering not two type with same name in the same source");
                };
            }
        }
    }
}

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
    Delete(this);
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
    return (this == a_pOther) && m_uiFlags == a_pOther->m_uiFlags;
}

void LanguageElement::replaceElement(LanguageElement* a_pOldElement, LanguageElement* a_pNewElement)
{
    *std::find(m_Elements.begin(), m_Elements.end(), a_pOldElement) = a_pNewElement;
}

void LanguageElement::getElements(LanguageElements& a_Elements) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    a_Elements.insert(a_Elements.end(), m_Elements.begin(), m_Elements.end());
}

void LanguageElement::fetchSymbols(Symbols& a_Symbols) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    for (auto pElem : m_Elements)
    {
        Symbol* pSymbol = pElem->asSymbol();
        if (pSymbol)
        {
            a_Symbols.push_back(pSymbol);
        }
    }
}

void LanguageElement::getElementsDeep(LanguageElements& a_Elements) const
{
    getElements(a_Elements);
    for (size_t i = 0; i < m_Elements.size(); ++i)
    {
        (m_Elements)[i]->getElementsDeep(a_Elements);
    }
}

bool LanguageElement::hasReferencedElement(LanguageElement*) const
{
    // 	for (auto it = m_pReferencedElements->begin(); it != m_pReferencedElements->end(); ++it)
    // 	{
    // 		if (*it == a_pLanguageElement)
    // 			return true;
    // 	}
    return false;
}

bool LanguageElement::hasReferencingElement(LanguageElement*) const
{
    //     if (m_pReferencingElements)
    //     {
    //         for (auto it = m_pReferencingElements->begin(); it != m_pReferencingElements->end(); ++it)
    //         {
    //             if (*it == a_pLanguageElement)
    //                 return true;
    //         }
    //     }
    return false;
}

void LanguageElement::dumpElementList(std::basic_ostream<char>& out) const
{
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    for (auto pElem : m_Elements)
    {
        out << pElem->getUniqueName() << std::endl;
    }
}

void LanguageElement::dumpElementListCascade(std::basic_ostream<char>& out) const
{
    dumpElementList(out);
    for (auto pElem : m_Elements)
    {
        pElem->dumpElementListCascade(out);
    }
}

bool LanguageElement::hasFriendCascade(Symbol* a_pElement) const
{
    return hasFriend(a_pElement) || (m_pOwner && m_pOwner->hasFriendCascade(a_pElement));
}

bool LanguageElement::isTemplateElement() const
{
    TemplateSpecialization* pSpec = getEnclosingTemplateSpecialization();
    return pSpec && (!(pSpec->isFull()) || pSpec->isTemplateElement());
}

Symbol* LanguageElement::getUniqueElement(StringView name, Modifiers modifiers /*= 0*/, uint /*= 0*/) const
{
    Symbol* pElement = nullptr;
    const_cast<LanguageElement*>(this)->_onElementsAccess();
    for (auto pElem : m_Elements)
    {
        Symbol* pSymbol = pElem->asSymbol();
        if (pSymbol && pSymbol->getName() == name && pSymbol->testModifiers(modifiers))
        {
            if (pElement)
                return nullptr;
            else
                pElement = pSymbol;
        }
    }
    return pElement;
}

size_t LanguageElement::getElementIndex(LanguageElement* a_pElement) const
{
    size_t count = m_Elements.size();
    for (size_t i = 0; i < count; ++i)
    {
        if ((m_Elements)[i] == a_pElement)
            return i;
    }
    return ~size_t(0);
}

bool LanguageElement::hasNamingScopeCascade(LanguageElement* a_pScope) const
{
    LanguageElement* pScope = getNamingScope();
    return (pScope != nullptr) && ((pScope == a_pScope) || pScope->hasNamingScopeCascade(a_pScope));
}

void LanguageElement::detachAll()
{
    while (!m_Elements.empty())
    {
        m_Elements.back()->setOwner(nullptr);
    }
}

void LanguageElement::getName(StringBuffer&) const {}

String LanguageElement::getName() const
{
    StringBuffer cstr;
    getName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getQualifiedName(StringBuffer& a_Buf) const
{
    size_t s = a_Buf.size();
    if (m_pOwner)
        m_pOwner->getQualifiedDecoratedName(a_Buf);
    if (s != a_Buf.size())
        a_Buf += "::";
    getName(a_Buf);
}

String LanguageElement::getQualifiedName() const
{
    StringBuffer cstr;
    getQualifiedName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (auto pScope = getNamingScope())
    {
        if (pScope != a_pTo)
        {
            size_t sz = a_Buf.size();
            pScope->getRelativeName(a_pTo, a_Buf);
            if (sz != a_Buf.size())
                a_Buf += "::";
        }
    }
    return getName(a_Buf);
}

void LanguageElement::getDecoratedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
}

String LanguageElement::getDecoratedName() const
{
    StringBuffer cstr;
    getDecoratedName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getQualifiedName(a_Buf);
}

String LanguageElement::getQualifiedDecoratedName() const
{
    StringBuffer cstr;
    getQualifiedDecoratedName(cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (a_pTo == this || a_pTo == getNamingScope())
        return getDecoratedName(a_Buf);

    if (hasNamingScopeCascade(a_pTo))
    {
        if (auto pScope = getNamingScope())
        {
            if (pScope != a_pTo)
            {
                size_t sz = a_Buf.size();
                pScope->getRelativeDecoratedName(a_pTo, a_Buf);
                if (sz != a_Buf.size())
                    a_Buf += "::";
            }
        }
        return getDecoratedName(a_Buf);
    }
    return getQualifiedDecoratedName(a_Buf);
}

String LanguageElement::getRelativeDecoratedName(LanguageElement* a_pTo) const
{
    StringBuffer cstr;
    getRelativeDecoratedName(a_pTo, cstr);
    return String(cstr.data(), cstr.size());
}

void LanguageElement::getUniqueName(StringBuffer&) const {}

LanguageElement* LanguageElement::getNamingScope() const
{
    return m_pOwner;
}

void LanguageElement::detach()
{
    setOwner(nullptr);
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
        if (pSymbol && pSymbol->getName() == a_strName && !(pSymbol->getVisibility() == Visibility::Private))
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

Module* LanguageElement::getModule() const
{
    return m_pSource ? m_pSource->getModule() // source or symbol
                     : m_pOwner->asModule();  // package
}

Package* LanguageElement::getPackage() const
{
    return m_pSource->getPackage();
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
        if (pSymbol1 && (pSymbol1 = a_Filter(pSymbol1, asSymbol() ? asSymbol()->getName().empty() : false)))
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

void LanguageElement::visit(LanguageElementVisitor* a_pVisitor, VisitorData a_Data)
{
    static_cast<LanguageElementVisitor*>(a_pVisitor)->visit(this, a_Data);
}

Symbol* LanguageElement::PublicFilter(Symbol* a_pSymbol, bool)
{
    return a_pSymbol->isPublic() ? a_pSymbol : nullptr;
}

Symbol* LanguageElement::PublicIfUnamedSubSymbolFilter(Symbol* a_pSymbol, bool a_bUnamedSubSymbol)
{
    return (!a_bUnamedSubSymbol || a_pSymbol->isPublic()) ? a_pSymbol : nullptr;
}

} // namespace lang
} // namespace phantom
