// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "StaticGlobals.h"

namespace phantom
{
namespace
{
    bool s_destroyedOnce;
}
static Placement<SmallMap<void*, CleanupDelegate, 256>>& _CleanupMap(bool _construct = true)
{
    static Placement<SmallMap<void*, CleanupDelegate, 256>> s_map;
    if (!s_map && _construct)
    {
        PHANTOM_ASSERT(!s_destroyedOnce);
        s_map.construct();
    }
    return s_map;
}

void StaticGlobals::RegisterForCleanup(void* a_pAddr, CleanupDelegate a_Delegate)
{
    PHANTOM_ASSERT(_CleanupMap()->find(a_pAddr) == _CleanupMap()->end());
    (*_CleanupMap())[a_pAddr] = a_Delegate;
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

void StaticGlobals::Release()
{
    for (auto& pair : *_CleanupMap())
    {
        pair.second(pair.first);
    }
    _CleanupMap().destroy();
    s_destroyedOnce = true;
}

} // namespace phantom
