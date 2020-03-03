// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "StaticGlobals.h"

namespace phantom
{
namespace
{
bool     s_destroyedOnce;
void*    s_releaseInProgress;
uint64_t s_staticGlobalCounter;

struct StaticGlobalInfo
{
    CleanupDelegate dgt;
    uint64_t        cnt = 0;
    void*           hnd = nullptr;
};

const size_t StaticGlobalMapStaticSize = 256;

} // namespace
static Placement<SmallMap<void*, StaticGlobalInfo, StaticGlobalMapStaticSize>>& _CleanupMap(bool _construct = true)
{
    static Placement<SmallMap<void*, StaticGlobalInfo, StaticGlobalMapStaticSize>> s_map;
    if (!s_map && _construct)
    {
        PHANTOM_ASSERT(!s_destroyedOnce);
        s_map.construct();
    }
    return s_map;
}

void StaticGlobals::RegisterForCleanup(void* a_pAddr, void* a_pModuleHandle, CleanupDelegate a_Delegate)
{
    PHANTOM_ASSERT(_CleanupMap()->find(a_pAddr) == _CleanupMap()->end());
    (*_CleanupMap())[a_pAddr] = {a_Delegate, s_staticGlobalCounter++, a_pModuleHandle};
}

void StaticGlobals::UnregisterForCleanup(void* a_pAddr)
{
    PHANTOM_ASSERT(_CleanupMap()->find(a_pAddr) != _CleanupMap()->end());
    _CleanupMap()->erase(a_pAddr);
}

bool StaticGlobals::TryUnregisterForCleanup(void* a_pAddr)
{
    if (_CleanupMap(false) && (_CleanupMap()->find(a_pAddr) != _CleanupMap()->end()))
    {
        _CleanupMap()->erase(a_pAddr);
        return true;
    }
    return false;
}

bool StaticGlobals::ReleaseInProgress(void* a_pModuleHandle)
{
    return s_releaseInProgress == a_pModuleHandle;
}

void StaticGlobals::Release(void* a_pModuleHandle)
{
    PHANTOM_ASSERT(a_pModuleHandle == nullptr || !ReleaseInProgress(a_pModuleHandle));
    s_releaseInProgress = a_pModuleHandle;
    // nasty hack to be able to resort map by time counter instead of by pointer
    SmallVector<Pair<void*, StaticGlobalInfo>, StaticGlobalMapStaticSize>& sortedByTimeReverse =
    (SmallVector<Pair<void*, StaticGlobalInfo>, StaticGlobalMapStaticSize>&)*_CleanupMap();
    std::sort(sortedByTimeReverse.begin(), sortedByTimeReverse.end(),
              [](Pair<void*, StaticGlobalInfo> const& p0, Pair<void*, StaticGlobalInfo> const& p1) -> bool {
                  return p0.second.cnt > p1.second.cnt;
              });

    auto startIt = sortedByTimeReverse.end();
    auto endIt = sortedByTimeReverse.begin();
    if (!a_pModuleHandle)
    {
        for (auto it = sortedByTimeReverse.begin(); it != sortedByTimeReverse.end(); ++it)
            it->second.dgt(it->first);
    }
    else
    {
        for (auto it = sortedByTimeReverse.begin(); it != sortedByTimeReverse.end(); ++it)
        {
            if (it->second.hnd == a_pModuleHandle)
            {
                PHANTOM_ASSERT(endIt == sortedByTimeReverse.begin());
                if (startIt == sortedByTimeReverse.end())
                    startIt = it;
                it->second.dgt(it->first);
            }
            else
            {
                PHANTOM_ASSERT(startIt == sortedByTimeReverse.end() || endIt == sortedByTimeReverse.begin());
                if (startIt != sortedByTimeReverse.end())
                {
                    endIt = it;
                    break;
                }
            }
        }
    }
    if (startIt != sortedByTimeReverse.end())
    {
        if (endIt == sortedByTimeReverse.begin())
            endIt = sortedByTimeReverse.end();
        sortedByTimeReverse.erase(startIt, endIt);
    }
    s_destroyedOnce = true;
    s_releaseInProgress = nullptr;

    // resort it back
    std::sort(sortedByTimeReverse.begin(), sortedByTimeReverse.end(),
              [](Pair<void*, StaticGlobalInfo> const& p0, Pair<void*, StaticGlobalInfo> const& p1) -> bool {
                  return p0.first < p1.first;
              });

    if (a_pModuleHandle == nullptr)
        _CleanupMap().destroy();
}

} // namespace phantom
