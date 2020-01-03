// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "VirtualMethodTable.h"

#include "Method.h"
#include "phantom/detail/new.h"

#include <phantom/utils/SmallMap.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
VirtualMethodTable::VirtualMethodTable() : m_pMethods(PHANTOM_NEW(Methods))
{
}
VirtualMethodTable::VirtualMethodTable(size_t a_uiSize) : m_pMethods(PHANTOM_NEW(Methods)())
{
    m_pMethods->resize(a_uiSize);
}

VirtualMethodTable::VirtualMethodTable(VirtualMethodTable* a_pBaseTable)
    : m_pMethods(a_pBaseTable->m_pMethods), m_pBaseTable(a_pBaseTable), m_bShared(true)
{
    m_pBaseTable->m_DerivedTables.push_back(this);
    addReferencedElement(m_pBaseTable);
}

VirtualMethodTable::VirtualMethodTable(VirtualMethodTable* a_pBaseTable, size_t a_uiSize)
    : m_pMethods(PHANTOM_NEW(Methods)()), m_pBaseTable(a_pBaseTable)
{
    PHANTOM_ASSERT(a_uiSize >= a_pBaseTable->getMethodCount(),
                   "a derived vtable must have equal or greater size than base");
    m_pMethods->resize(a_uiSize);
    m_pBaseTable->m_DerivedTables.push_back(this);
    for (size_t i = 0; i < a_pBaseTable->m_pMethods->size(); ++i)
    {
        (*m_pMethods)[i] = (*(a_pBaseTable->m_pMethods))[i];
    }
    addReferencedElement(m_pBaseTable);
}

VirtualMethodTable::VirtualMethodTable(void** a_ppClosures, size_t a_uiSize)
    : m_pMethods(PHANTOM_NEW(Methods)()), m_ppClosures(a_ppClosures)
{
    m_pMethods->resize(a_uiSize);
}

PHANTOM_DTOR VirtualMethodTable::~VirtualMethodTable()
{
    if (m_pBaseTable)
        m_pBaseTable->m_DerivedTables.erase(
        std::find(m_pBaseTable->m_DerivedTables.begin(), m_pBaseTable->m_DerivedTables.end(), this));
    if (NOT(sharesMethods()))
        PHANTOM_DELETE(Methods) m_pMethods;
    if (m_ppClosures)
        PHANTOM_FREE(m_ppClosures);
}

size_t VirtualMethodTable::getIndexOf(Method* a_pMethod) const
{
    size_t i = 0;
    for (; i < m_pMethods->size(); ++i)
    {
        if ((*m_pMethods)[i] == a_pMethod)
            return i;
    }
    return ~size_t(0);
}

VirtualMethodTable* VirtualMethodTable::derive(size_t a_uiSize /*= 0*/) const
{
    if (a_uiSize == 0 OR a_uiSize == m_pMethods->size())
    {
        return PHANTOM_DEFERRED_NEW_EX(VirtualMethodTable)(const_cast<VirtualMethodTable*>(this));
    }
    PHANTOM_ASSERT(a_uiSize > m_pMethods->size());
    return PHANTOM_DEFERRED_NEW_EX(VirtualMethodTable)(const_cast<VirtualMethodTable*>(this), a_uiSize);
}

size_t VirtualMethodTable::getOffset() const
{
    if (getOwner() == nullptr)
        return ~size_t(0);
    return m_pBaseTable ? getOwnerClass()->getBaseClassOffset(m_pBaseTable->getOwnerClass()) + m_pBaseTable->getOffset()
                        : 0;
}

void VirtualMethodTable::addMethod(Method* a_pMethod)
{
    setMethod(getMethodCount(), a_pMethod);
}

void VirtualMethodTable::setMethod(size_t a_uiIndex, Method* a_pMethod)
{
    if (sharesMethods())
        copyOnWrite();
    m_pMethods->resize(std::max(a_uiIndex + 1, getMethodCount()));
    (*m_pMethods)[a_uiIndex] = a_pMethod;
}

bool VirtualMethodTable::insertMethod(Method* a_pMethod, bool a_bOnlyIfOverrides)
{
    Methods* funcs = m_bShared ? m_pBaseTable->m_pMethods : m_pMethods;
    if (funcs && m_pBaseTable)
    {
        size_t i = 0;
        for (Method* pOverridableMethod : *funcs)
        {
            if (pOverridableMethod && pOverridableMethod->isVirtual())
            {
                if (a_pMethod->canOverride(pOverridableMethod))
                {
                    a_pMethod->m_Modifiers |= Modifier::Virtual; // Set virtual if not (indeed, even if not explicitely
                                                                 // virtual, a member function which overrides a virtual
                                                                 // becomes virtual
                    size_t virtualIndex = i;
                    if (getOffset() == 0)
                    {
                        if (a_pMethod->isNative())
                            virtualIndex = pOverridableMethod->computeNativeVirtualIndex();
                    }
                    setMethod(virtualIndex,
                              a_pMethod); // replace base class member function by this one (overrides entry)
                    return true;
                }
            }
            ++i;
        }
    }

    // We did not find any overrides
    if (NOT(a_bOnlyIfOverrides) AND getOffset() ==
        0                           // and it's the main vtable
        AND a_pMethod->isVirtual()) // and the member function is virtual
    {
        // => insert it
        if (a_pMethod->isNative() && !getOwnerClass()->isFinal())
        {
            // to an index given by the function itself
            setMethod(a_pMethod->computeNativeVirtualIndex(), a_pMethod);
        }
        else
        {
            // to a PHANTOM_NEW(index)
            addMethod(a_pMethod);
        }
        return true;
    }
    return false;
}

void VirtualMethodTable::construct(void* a_pInstance)
{
    SmallMap<void***, size_t, 16> vmethodCountAtAddress;
    _construct(a_pInstance, vmethodCountAtAddress);
}

void VirtualMethodTable::_construct(void* a_pInstance, SmallMap<void***, size_t, 16>& a_VTableSizeAtAddress)
{
    void*** pppDest = (void***)((byte*)a_pInstance + getOffset());
    size_t& vtableSizeAtDest = a_VTableSizeAtAddress[pppDest];
    size_t  vTableSize = m_pBaseTable ? std::max(getMethodCount(), m_pBaseTable->getMethodCount()) : getMethodCount();
    if (vtableSizeAtDest == 0) // if not methods already present at this address with more methods
    {
        vtableSizeAtDest = vTableSize;
    }
    else
    {
        PHANTOM_ASSERT(vTableSize <= vtableSizeAtDest,
                       "a base class cannot have more virtual methods than a derived one");
    }
    if (isNative())
    {
        if (m_ppClosures == nullptr)
        {
            size_t vtableSize = getMethodCount() * sizeof(void*);
            m_ppClosures = (void**)PHANTOM_MALLOC(vtableSize * sizeof(void*));
            void* non_init_ptr = nullptr;
            if (memcmp(pppDest, &non_init_ptr, sizeof(void*)) != 0) // memory not isSame 0xdadadada => closures already
                                                                    // present => a base vtable has been installed here
            {
                // extract native vtable closures for replacement
                memcpy(m_ppClosures, *pppDest, vtableSize * sizeof(void*));
            }
        }
        *pppDest = m_ppClosures;
        return;
    }

    if (m_pBaseTable)
    {
        m_pBaseTable->_construct(
        (byte*)a_pInstance + getOwnerClass()->getBaseClassOffset(m_pBaseTable->getOwnerClass()), a_VTableSizeAtAddress);
        if (sharesMethods())
            return;
    }
    if (m_ppClosures == nullptr)
    {
        m_ppClosures = (void**)PHANTOM_MALLOC(vtableSizeAtDest * sizeof(void*));
        if (m_pBaseTable)
        {
            void* non_init_ptr = nullptr;
            if (memcmp(pppDest, &non_init_ptr, sizeof(void*)) !=
                0) // memory not isSame 0xdadadada => closures
                   // already present => a base vtable has been installed here

            {
                memcpy(m_ppClosures, *pppDest,
                       vtableSizeAtDest * sizeof(void*)); // extract native vtable closures for replacement
            }
        }

        auto   vtableOffset = getOffset();
        size_t thisMethodCount = getMethodCount();
        for (size_t i = 0; i < thisMethodCount; ++i)
        {
            if (Method* pMethod = (*m_pMethods)[i])
            {
                void* pVTableClosure = pMethod->getVTableClosure(vtableOffset);
                // method has a vtable closure defined ?
                if (!pMethod->isNative() && pVTableClosure)
                {
                    m_ppClosures[i] = pVTableClosure; // write (resp. overwrite) closures (resp. base closures)
                }

                // method is pure virtual ?
                else if (pMethod->testModifiers(PHANTOM_R_PURE_VIRTUAL))
                {
                    m_ppClosures[i] = 0;
                }

                // method has no vtable closure defined,
                // is not pure virtual,
                // is native
                // and belongs to the same class as this vtable
                // => we can extract its original
                else if (pMethod->isNative() AND pMethod->getOwner() == getOwner())
                {
                    pMethod->setVTableClosure(vtableOffset, (*pppDest)[i]);
                }
                // PHANTOM_ASSERT(m_ppClosures[i], "No vtable closure found for given vtable offset,
                // ensure your compiler has provided all the closures for each possible
                // multi-inheritance this adjustment (thunk)");
            }
        }
    }
    *pppDest = m_ppClosures; // write (resp. overwrite) vtable pointer (resp. base vtable pointer)
}

Method* VirtualMethodTable::getRootMethod(size_t a_uiIndex) const
{
    if (a_uiIndex >= m_pMethods->size())
        return nullptr;
    if (m_pBaseTable)
    {
        Method* pBase = m_pBaseTable->getRootMethod(a_uiIndex);
        if (pBase)
            return pBase;
    }
    return (*m_pMethods)[a_uiIndex];
}

Method* VirtualMethodTable::getRootMethod(Method* a_pMethod) const
{
    if (getOwnerClass() != a_pMethod->getOwnerClass())
        return m_pBaseTable ? m_pBaseTable->getRootMethod(a_pMethod) : nullptr;
    for (size_t i = 0; i < m_pMethods->size(); ++i)
    {
        if ((*m_pMethods)[i] == a_pMethod)
            return getRootMethod(i);
    }
    return nullptr;
}

size_t VirtualMethodTable::getMethodIndex(Method* a_pMethod) const
{
    if (m_pMethods)
    {
        for (size_t i = 0; i < m_pMethods->size(); ++i)
        {
            if ((*m_pMethods)[i] == a_pMethod)
                return i;
        }
    }
    return ~size_t(0);
}

bool VirtualMethodTable::canBeDestroyed() const
{
    return LanguageElement::canBeDestroyed();
}

void VirtualMethodTable::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pBaseTable == a_pElement)
        m_pBaseTable = nullptr;
}

void VirtualMethodTable::copyOnWrite()
{
    PHANTOM_ASSERT(sharesMethods());
    m_pMethods = PHANTOM_NEW(Methods);
    *m_pMethods = *(m_pBaseTable->m_pMethods);
    m_bShared = false;
}

} // namespace reflection
} // namespace phantom
