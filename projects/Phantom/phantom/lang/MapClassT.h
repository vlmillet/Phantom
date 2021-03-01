// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/ContainerClassT.h>
#include <phantom/lang/MapClass.h>

namespace phantom
{
namespace lang
{
// hacker
template<typename T>
struct map_value_type_without_const
{
    typedef T type;
};

template<typename t_KTy, typename T>
struct map_value_type_without_const<std::pair<const t_KTy, T> >
{
    typedef Pair<t_KTy, T> type;
};

template<typename T, typename Base = MapClass>
class MapClassT : public ContainerClassT<T, Base>
{
    using BaseType = ContainerClassT<T, Base>;

    typedef PHANTOM_TYPENAME T::key_type container_key_type;
    typedef PHANTOM_TYPENAME T::mapped_type container_mapped_type;
    typedef PHANTOM_TYPENAME T::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME T::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME T::iterator ContainerIterator;

public:
    MapClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setKeyType(PHANTOM_TYPEOF(container_key_type));
        this->setMappedType(PHANTOM_TYPEOF(container_mapped_type));
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    size_t size(void const* a_pContainer) const override
    {
        T const* container = static_cast<T const*>(a_pContainer);
        return container->size();
    }

    void* referenceAt(void* a_pContainer, size_t a_uiIndex) const override
    {
        T*   container = static_cast<T*>(a_pContainer);
        auto it = container->begin();
        while (a_uiIndex--)
            ++it;
        return &*it;
    }

    void const* referenceAt(void const* a_pContainer, size_t a_uiIndex) const override
    {
        T const* container = static_cast<T const*>(a_pContainer);
        auto     it = container->begin();
        while (a_uiIndex--)
            ++it;
        return &*it;
    }

    void find(void* a_pContainer, void const* a_pKey, void* a_pOutIt) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        *(ContainerIterator*)a_pOutIt = pContainer->find(*pKey);
    }

    void find(void const* a_pContainer, void const* a_pKey, void* a_pOutIt) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        T const*                  pContainer = static_cast<T const*>(a_pContainer);
        *(ContainerConstIterator*)a_pOutIt = pContainer->find(*pKey);
    }

    void map(void* a_pContainer, void const* a_pKey, void* a_pDest) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        *(void**)a_pDest = (void*)&(*pContainer)[*pKey];
    }

    void eraseKey(void* a_pContainer, void const* a_pKey) const override
    {
        container_key_type const* pKey = static_cast<container_key_type const*>(a_pKey);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        pContainer->erase(pContainer->find(*pKey));
    }

    void insert(void* a_pContainer, void const* a_pPair) const override
    {
        ContainerValueType const* pPair = static_cast<ContainerValueType const*>(a_pPair);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        pContainer->insert(*pPair);
    }

    void assign(void* a_pContainer, void const* a_pPair) const override
    {
        ContainerValueType const* pPair = static_cast<ContainerValueType const*>(a_pPair);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        (*pContainer)[pPair->first] = pPair->second;
    }

    void assign(void* a_pContainer, void const* a_pKey, void const* a_pValue) const override
    {
        auto const* pKey = static_cast<container_key_type const*>(a_pKey);
        auto const* pVal = static_cast<container_mapped_type const*>(a_pValue);
        T*          pContainer = static_cast<T*>(a_pContainer);
        (*pContainer)[*pKey] = *pVal;
    }

    void clear(void* a_pContainer) const override { static_cast<T*>(a_pContainer)->clear(); }
};

} // namespace lang
} // namespace phantom
