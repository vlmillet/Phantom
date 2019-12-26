// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/alignof>
#include <phantom/fwd.h>

HAUNT_STOP;

#include <limits>
#include <stdlib.h>

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM MemoryTraits
{
    static void                Push(MemoryTraits a_func);
    static void                Pop();
    static MemoryTraits const* CurrentOrDefault();
    static MemoryTraits const* Current();
    static MemoryTraits const* Default();
    static void                Init();
    static void                Release();

    typedef void* (*AllocFunc)(size_t /*size*/, size_t /*align*/, const char* /*file*/,
                               int /*line*/);
    typedef void* (*ReallocFunc)(void* /*memory*/, size_t /*size*/, size_t /*align*/,
                                 const char* /*file*/, int /*line*/);
    typedef void (*DeallocFunc)(void* /*memory*/);

    AllocFunc   allocFunc;
    DeallocFunc deallocFunc;
    ReallocFunc reallocFunc;

    bool operator==(MemoryTraits const& o) const
    {
        return (allocFunc == o.allocFunc && deallocFunc == o.deallocFunc &&
                reallocFunc == o.reallocFunc);
    }
};

namespace memory
{
PHANTOM_EXPORT_PHANTOM void* allocBytes(size_t size, size_t align = 1, const char* file = "",
                                        int line = 0);
PHANTOM_EXPORT_PHANTOM void  deallocBytes(void* mem);
PHANTOM_EXPORT_PHANTOM void* reallocBytes(void* mem, size_t size, size_t align = 1,
                                          const char* file = "", int line = 0);

} // namespace memory
} // namespace phantom
