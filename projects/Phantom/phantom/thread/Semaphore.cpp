// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Semaphore.h"

namespace phantom
{
Semaphore::Semaphore(uint initialValue /*= 0*/, int) : m_value(initialValue), m_waitingCount(0)
{
    PHANTOM_ASSERT(initialValue >= 0);
}

void Semaphore::wait()
{
    bool wakeUpAnother = false;
    {
        std::unique_lock<std::mutex> lck(m_mutex);

        while (m_value == 0)
        {
            m_waitingCount++;
            m_cond.wait(lck);
            m_waitingCount--;
        }

        m_value--;

        if (m_value > 0 && m_waitingCount > 0)
        {
            wakeUpAnother = true;
        }
    }

    if (wakeUpAnother)
    {
        m_cond.notify_one();
    }
}

void Semaphore::signal(int count /*= 1*/)
{
    bool wakeUp = false;
    {
        m_mutex.lock();

        m_value += count;

        if (m_waitingCount > 0)
        {
            wakeUp = true;
        }

        m_mutex.unlock();
    }

    if (wakeUp)
    {
        m_cond.notify_one();
    }
}
} // namespace phantom
