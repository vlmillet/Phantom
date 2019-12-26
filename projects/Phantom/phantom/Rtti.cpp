// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "Placement.h"
#include "Rtti.h"
#include "RttiImpl.h"
#include "RttiMapData.h"
#include "RttiPrivate.h"
#include "SmallSet.h"
#include "phantom_priv.h"

namespace phantom
{
namespace detail
{
extern RawPlacement<RttiDataMap> g_pRttiDataMap;
extern RawPlacement<RttiMutex>   g_RttiMapMutex;

static RttiMutex& rttiMapMutex()
{
    dynamic_initializer_();
    return *g_RttiMapMutex;
}
} // namespace detail

#define _PHNTM_RTTI_MTX_GURD                                                                       \
    ::phantom::LockGuard< ::phantom::detail::RttiMutex> __rflct_gurd(                              \
    ::phantom::detail::rttiMapMutex())

void Rtti::Map(void const* a_pThis, reflection::Class* a_pClass)
{
    a_pClass->mapRtti(a_pThis);
}

void Rtti::Unmap(void const* a_pThis)
{
    RttiMapData const& data = Find(a_pThis);
    PHANTOM_ASSERT(!data.isNull());
    data.getObjectClass()->unmapRtti(a_pThis);
}

void* Rtti::MostDerivedOf(void const* a_pThis /*= 0*/)
{
    _PHNTM_RTTI_MTX_GURD;
    auto found = detail::g_pRttiDataMap->find(a_pThis);
    if (found != detail::g_pRttiDataMap->end())
    {
        found->second.getObjectAddress();
    }
    return const_cast<void*>(a_pThis);
}

reflection::Class* Rtti::ClassOf(void const* a_pThis)
{
    _PHNTM_RTTI_MTX_GURD;
    auto found = detail::g_pRttiDataMap->find(a_pThis);
    if (found != detail::g_pRttiDataMap->end())
    {
        found->second.getObjectClass();
    }
    return nullptr;
}

reflection::Class* Rtti::ClassAt(void const* a_pThis)
{
    _PHNTM_RTTI_MTX_GURD;
    auto found = detail::g_pRttiDataMap->find(a_pThis);
    if (found != detail::g_pRttiDataMap->end())
    {
        found->second.getAddressClass();
    }
    return nullptr;
}

const RttiMapData& Rtti::Find(void const* a_pThis)
{
    _PHNTM_RTTI_MTX_GURD;
    static RttiMapData null_info;
    auto               found = detail::g_pRttiDataMap->find(a_pThis);
    if (found != detail::g_pRttiDataMap->end())
    {
        return found->second;
    }
    return null_info;
}

size_t Rtti::AddressesOf(void const* a_pThis, void const** a_pBuffer, size_t a_BufferSize)
{
    _PHNTM_RTTI_MTX_GURD;
    static RttiMapData null_info;
    auto               found = detail::g_pRttiDataMap->find(a_pThis);
    size_t             count = 0;
    if (found != detail::g_pRttiDataMap->end())
    {
        void*                    obj = found->second.getObjectAddress();
        SmallSet<void const*, 4> temp;
        for (auto const& entry : *detail::g_pRttiDataMap)
        {
            if (entry.second.getObjectAddress() == obj)
                temp.insert(entry.first);
        }
        count = std::min(a_BufferSize, temp.size());
        for (size_t i = 0; i < count; ++i)
        {
            *a_pBuffer++ = temp[i];
        }
    }
    return count;
}

const RttiMapData* Rtti::InsertCustomData(void const* a_pThis, const RttiMapData& a_RttiData)
{
    _PHNTM_RTTI_MTX_GURD;
    auto found = detail::g_pRttiDataMap->find(a_pThis);
    PHANTOM_ASSERT(found == detail::g_pRttiDataMap->end());
    return &((*detail::g_pRttiDataMap)[a_pThis] = a_RttiData);
}

void Rtti::ReplaceCustomData(void const* a_pThis, const RttiMapData& a_RttiData)
{
    _PHNTM_RTTI_MTX_GURD;
    auto found = detail::g_pRttiDataMap->find(a_pThis);
    PHANTOM_ASSERT(found != detail::g_pRttiDataMap->end());
    found->second = a_RttiData;
}

void Rtti::EraseCustomData(void const* a_pThis)
{
    _PHNTM_RTTI_MTX_GURD;
    auto found = detail::g_pRttiDataMap->find(a_pThis);
    PHANTOM_ASSERT(found != detail::g_pRttiDataMap->end());
    detail::g_pRttiDataMap->erase(found);
}

} // namespace phantom
