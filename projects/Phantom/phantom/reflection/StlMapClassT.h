// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/new>
#include <phantom/reflection/StlContainerClassT.h>
#include <phantom/reflection/StlMapClass.h>

namespace phantom
{
namespace reflection
{
// hacker
template<typename t_Ty>
struct map_value_type_without_const
{
    typedef t_Ty type;
};

template<typename t_KTy, typename t_Ty>
struct map_value_type_without_const<std::pair<const t_KTy, t_Ty> >
{
    typedef Pair<t_KTy, t_Ty> type;
};

template<typename t_Ty, typename Base = StlMapClass>
class StlMapClassT : public StlContainerClassT<t_Ty, Base>
{
    using BaseType = StlContainerClassT<t_Ty, Base>;

    typedef PHANTOM_TYPENAME t_Ty::key_type container_key_type;
    typedef PHANTOM_TYPENAME t_Ty::mapped_type container_mapped_type;
    typedef PHANTOM_TYPENAME t_Ty::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME t_Ty::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME t_Ty::iterator ContainerIterator;

public:
    StlMapClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setKeyType(PHANTOM_TYPEOF(container_key_type));
        this->setMappedType(PHANTOM_TYPEOF(container_mapped_type));
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    virtual size_t size(void const* a_pContainer) const override
    {
        t_Ty const* container = static_cast<t_Ty const*>(a_pContainer);
        return container->size();
    }

    virtual void* referenceAt(void* a_pContainer, size_t a_uiIndex) const override
    {
        t_Ty* container = static_cast<t_Ty*>(a_pContainer);
        auto  it = container->begin();
        while (a_uiIndex--)
            ++it;
        return &*it;
    }

    virtual void const* referenceAt(void const* a_pContainer, size_t a_uiIndex) const override
    {
        t_Ty const* container = static_cast<t_Ty const*>(a_pContainer);
        auto        it = container->begin();
        while (a_uiIndex--)
            ++it;
        return &*it;
    }

    virtual void find(void* a_pContainer, void const* a_pKey, void* a_pOutIt) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        *(ContainerIterator*)a_pOutIt = pContainer->find(*pKey);
    }

    virtual void find(void const* a_pContainer, void const* a_pKey, void* a_pOutIt) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        t_Ty const*               pContainer = static_cast<t_Ty const*>(a_pContainer);
        *(ContainerConstIterator*)a_pOutIt = pContainer->find(*pKey);
    }

    virtual void map(void* a_pContainer, void const* a_pKey, void* a_pDest) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        *(void**)a_pDest = (void*)&(*pContainer)[*pKey];
    }

    virtual void eraseKey(void* a_pContainer, void const* a_pKey) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        pContainer->erase(pContainer->find(*pKey));
    }

    virtual void insert(void* a_pContainer, void const* a_pPair) const override
    {
        ContainerValueType const* pPair = static_cast<ContainerValueType const*>(a_pPair);
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        pContainer->insert(*pPair);
    }

    virtual void clear(void* a_pContainer) const override
    {
        static_cast<t_Ty*>(a_pContainer)->clear();
    }
};

} // namespace reflection
} // namespace phantom
