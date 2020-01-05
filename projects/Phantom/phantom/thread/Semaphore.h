// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <phantom/detail/core.h>

//--- Pause Instruction
// To use inside spin loops to help with power consumption/scheduling/performance.
#if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86_64 || PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
#    include <emmintrin.h>
#    define PHANTOM_MM_PAUSE() _mm_pause()
#else
// Not supported? ARM check out YIELD instruction
#    define PHANTOM_MM_PAUSE()
#endif

//--- Compiler Barrier
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_COMPILER_BARRIER() _ReadWriteBarrier()
#else
#    define PHANTOM_COMPILER_BARRIER() asm volatile("" ::: "memory")
#endif

namespace phantom
{
class PHANTOM_EXPORT_PHANTOM Semaphore
{
public:
    Semaphore(uint initialValue = 0, int unused = 0);

    void wait();
    void signal(int count = 1);

    int getWaitingCountRelaxed() const
    {
        return m_waitingCount;
    }

private:
    int                     m_value;
    int                     m_waitingCount;
    std::mutex              m_mutex;
    std::condition_variable m_cond;
};

/// Based on Jeff Preshing's Lightweightsemaphore:
/// https://github.com/preshing/cpp11-on-multicore/blob/master/common/sema.h
class PHANTOM_EXPORT_PHANTOM LWSemaphore
{
public:
    inline LWSemaphore(int initialCount = 0, int spinCount = 10000);
    inline bool tryWait();
    inline void wait();
    inline void signal(int count = 1);

    inline int getWaitingCountRelaxed() const;

private:
    inline void waitWithPartialSpinning();

    const int       m_spinCount;
    std::atomic_int m_count;
    Semaphore       m_sema;
};

class PHANTOM_EXPORT_PHANTOM SpinSemaphore
{
public:
    inline SpinSemaphore(int initialCount = 0, int unused = 0);
    inline bool tryWait();
    inline void wait();
    inline void signal(int count = 1);

private:
    inline void waitWithSpinning();

    std::atomic_int m_count;
};

LWSemaphore::LWSemaphore(int initialCount /*= 0*/, int spinCount /*= 10000*/)
    : m_spinCount(spinCount), m_count(initialCount)
{
    PHANTOM_ASSERT(initialCount >= 0);
}

void LWSemaphore::waitWithPartialSpinning()
{
    int oldCount;
    int spin = m_spinCount;
    while (spin--)
    {
        oldCount = m_count.load(std::memory_order_relaxed);
        if ((oldCount > 0) && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire))
            return;
        PHANTOM_MM_PAUSE();
        PHANTOM_COMPILER_BARRIER(); // Prevent the compiler from collapsing the loop.
    }
    oldCount = m_count.fetch_add(-1, std::memory_order_acquire);
    if (oldCount <= 0)
    {
        m_sema.wait();
    }
}

bool LWSemaphore::tryWait()
{
    int oldCount = m_count.load(std::memory_order_relaxed);
    return (oldCount > 0 && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire));
}

void LWSemaphore::wait()
{
    if (!tryWait())
        waitWithPartialSpinning();
}

void LWSemaphore::signal(int count /*= 1*/)
{
    int oldCount = m_count.fetch_add(count, std::memory_order_release);
    int toRelease = -oldCount < count ? -oldCount : count;
    if (toRelease > 0)
    {
        m_sema.signal(toRelease);
    }
}

int LWSemaphore::getWaitingCountRelaxed() const
{
    int count = m_count.load(std::memory_order_relaxed);
    if (count < 0)
        return -count;
    else
        return 0;
}

SpinSemaphore::SpinSemaphore(int initialCount /*= 0*/, int) : m_count(initialCount)
{
    PHANTOM_ASSERT(initialCount >= 0);
}

void SpinSemaphore::waitWithSpinning()
{
    int oldCount;
    while (1)
    {
        oldCount = m_count.load(std::memory_order_relaxed);
        if ((oldCount > 0) && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire))
            return;
        PHANTOM_MM_PAUSE();
        PHANTOM_COMPILER_BARRIER(); // Prevent the compiler from collapsing the loop.
    }
}

bool SpinSemaphore::tryWait()
{
    int oldCount = m_count.load(std::memory_order_relaxed);
    return (oldCount > 0 && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire));
}

void SpinSemaphore::wait()
{
    if (!tryWait())
        waitWithSpinning();
}

void SpinSemaphore::signal(int count /*= 1*/)
{
    m_count.fetch_add(count, std::memory_order_release);
}
} // namespace phantom
