// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/plugin.h>
#include <phantom/utils/Delegate.h>

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

    using AllocFunc = Delegate<void*(size_t /*size*/, size_t /*align*/)>;
    using ReallocFunc = Delegate<void*(void* /*memory*/, size_t /*size*/, size_t /*align*/)>;
    using DeallocFunc = Delegate<void(void* /*memory*/)>;

    AllocFunc   allocFunc;
    DeallocFunc deallocFunc;
    ReallocFunc reallocFunc;

    bool operator==(CustomAllocator const& o) const
    {
        return (allocFunc == o.allocFunc && deallocFunc == o.deallocFunc && reallocFunc == o.reallocFunc);
    }
};
} // namespace phantom
