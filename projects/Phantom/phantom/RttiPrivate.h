// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include "thread/RecursiveMutex.h"

#include <phantom/new>

namespace phantom
{
typedef SmallMap<void const*, RttiMapData> RttiDataMap; // TODO implement home made hash map

namespace detail
{
struct RttiMutex
{
    RttiMutex()
    {
        m_private = PHANTOM_NEW(RecursiveMutex);
    }

    ~RttiMutex()
    {
        PHANTOM_DELETE(RecursiveMutex) m_private;
    }

    void lock()
    {
        m_private->lock();
    }

    void unlock()
    {
        m_private->unlock();
    }

private:
    RecursiveMutex* m_private = nullptr;
};
} // namespace detail
} // namespace phantom
