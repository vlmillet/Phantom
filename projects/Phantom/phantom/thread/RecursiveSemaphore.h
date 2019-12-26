// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "LockGuard.h"
#include "Semaphore.h"

#include <thread>

namespace phantom
{
template<class t_Semaphore>
class RecursiveSemaphore
{
public:
    inline RecursiveSemaphore(int _spinCount = 10000);

    inline void                                lock();
    inline bool                                tryLock();
    inline void                                unlock();
    LockGuard<RecursiveSemaphore<t_Semaphore>> autoLock()
    {
        return LockGuard<RecursiveSemaphore<t_Semaphore>>(*this);
    }

private:
    std::atomic<size_t> m_owner{ 0 }; // atomic thread id
    t_Semaphore         m_sema;
    std::atomic<int>    m_contentionCount{0};
    int                 m_recursion = 0;
};

template<class t_Semaphore>
RecursiveSemaphore<t_Semaphore>::RecursiveSemaphore(int _spinCount /*= 10000*/)
    : m_sema(0, _spinCount)
{
}

template<class t_Semaphore>
void RecursiveSemaphore<t_Semaphore>::lock()
{
    size_t thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
    if (m_contentionCount.fetch_add(1, std::memory_order_acquire) > 0)
    {
        if (thread_hash != m_owner.load(std::memory_order_relaxed))
            m_sema.wait();
    }
    //--- We are now inside the lock ---
    m_owner.store(thread_hash, std::memory_order_relaxed);
    m_recursion++;
}

template<class t_Semaphore>
bool RecursiveSemaphore<t_Semaphore>::tryLock()
{
    size_t thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
    if (m_owner.load(std::memory_order_relaxed) == thread_hash)
    {
        // Already inside the lock
        m_contentionCount.fetch_add(1, std::memory_order_relaxed);
    }
    else
    {
        if (m_contentionCount.load(std::memory_order_relaxed) != 0)
            return false;
        int expected = 0;
        if (!m_contentionCount.compare_exchange_strong(
            expected, 1, std::memory_order_acquire)) // compare_exchange_strong(expected, 1,
                                                     // std::memory_order_acquire))
            return false;
        //--- We are now inside the lock ---
        m_owner.store(thread_hash, std::memory_order_relaxed);
    }
    m_recursion++;
    return true;
}

template<class t_Semaphore>
void RecursiveSemaphore<t_Semaphore>::unlock()
{
    PHANTOM_ASSERT(std::hash<std::thread::id>()(std::this_thread::get_id()) ==
                   m_owner.load(std::memory_order_relaxed));
    int recur = --m_recursion;
    if (recur == 0)
        m_owner.store(0, std::memory_order_relaxed);
    if (m_contentionCount.fetch_add(-1, std::memory_order_release) >
        1) // fetch_sub(1, std::memory_order_release) > 1)
    {
        if (recur == 0)
            m_sema.signal();
    }
    //--- We are now outside the lock ---
}
} // namespace phantom
