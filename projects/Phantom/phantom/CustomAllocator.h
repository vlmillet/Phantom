#pragma once

#include <phantom/plugin.h>

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM CustomAllocator
{
    // User must call these for pushing its own allocators
    static void Push(CustomAllocator a_func);
    static void Pop();

    static CustomAllocator const& CurrentOrDefault();
    static CustomAllocator const& Current();
    static CustomAllocator const& Default();
    static void                   Init();
    static void                   Release();

    typedef void* (*AllocFunc)(size_t /*size*/, size_t /*align*/);
    typedef void* (*ReallocFunc)(void* /*memory*/, size_t /*size*/, size_t /*align*/);
    typedef void (*DeallocFunc)(void* /*memory*/);

    AllocFunc   allocFunc;
    DeallocFunc deallocFunc;
    ReallocFunc reallocFunc;

    bool operator==(CustomAllocator const& o) const
    {
        return (allocFunc == o.allocFunc && deallocFunc == o.deallocFunc && reallocFunc == o.reallocFunc);
    }
};
} // namespace phantom
