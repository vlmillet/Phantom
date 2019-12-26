// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "LockGuard.h"

#include <atomic>

namespace phantom
{
class SpinMutex
{
public:
    inline void lock()
    {
        while (lck.test_and_set(std::memory_order_acquire))
            ;
    }

    inline void unlock()
    {
        lck.clear(std::memory_order_release);
    }

    LockGuard<SpinMutex> autoLock()
    {
        return LockGuard<SpinMutex>(*this);
    }

private:
    std::atomic_flag lck = ATOMIC_FLAG_INIT;
};

} // namespace phantom
