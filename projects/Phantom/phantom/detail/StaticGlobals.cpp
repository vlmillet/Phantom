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
bool            s_destroyedOnce;
void*           s_releaseInProgress;
uint64_t        s_staticGlobalCounter;
SmallSet<void*> s_encounteredModules;
void*           s_lastEncounteredModules;

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
    PHANTOM_ASSERT(voidptr(_dllModuleHandleFromAddress(a_pAddr)) == a_pModuleHandle);
    auto& map = (*_CleanupMap());
    map[a_pAddr] = {a_Delegate, s_staticGlobalCounter++, a_pModuleHandle};
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

void StaticGlobals::Release(void* a_pModuleStartAddress, void* a_pModuleEndAddress)
{
    auto& sgmap = *_CleanupMap();
    if (a_pModuleStartAddress == nullptr) // special main case
    {
        PHANTOM_ASSERT(a_pModuleEndAddress == nullptr);
        // we release all remaining addresses
        s_releaseInProgress = a_pModuleStartAddress;
        auto start = std::make_reverse_iterator(sgmap.end());
        auto end = std::make_reverse_iterator(sgmap.begin());
        for (auto it = start; it != end; ++it)
        {
            if (it->second.dgt)
                it->second.dgt(it->first);
        }
        sgmap.clear();

        s_destroyedOnce = true;
        s_releaseInProgress = nullptr;
        _CleanupMap().destroy();
    }
    else
    {
        auto sgmapend = sgmap.end();
        auto moduleStart = sgmap.find(a_pModuleStartAddress);
        PHANTOM_ASSERT(moduleStart != sgmap.end());
        auto start = moduleStart + 1; // skip base address (its a fake entry)
        auto end = start;

        PHANTOM_ASSERT(a_pModuleStartAddress == nullptr || !ReleaseInProgress(a_pModuleStartAddress));
        s_releaseInProgress = a_pModuleStartAddress;

        SmallVector<Pair<void* const, StaticGlobalInfo>*, 1024> toExecReverse;

        for (end = start; end != sgmapend; ++end)
        {
            if (end->first >= a_pModuleEndAddress)
                break; // we are out of this module range, we can stop search
            toExecReverse.push_back(end);
        }

        for (auto it = toExecReverse.rbegin(); it != toExecReverse.rend(); ++it)
        {
            (*it)->second.dgt((*it)->first);
        }

        sgmap.erase(moduleStart, end);

        s_destroyedOnce = true;
        s_releaseInProgress = nullptr;
    }
}

} // namespace phantom
