// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "phantom/new.h"

#include <functional>
#include <phantom/SmallVector.h>

namespace phantom
{
template<class Signature, class MutexT = NullMutex, size_t t_StaticAllocCount = 4>
class StdFunctionSignal;

template<class R, class MutexT, size_t t_StaticAllocCount, class... Args>
class StdFunctionSignal<R(Args...), MutexT, t_StaticAllocCount>
{
public:
    typedef std::function<R(Args...)>                    StdFunction;
    typedef SmallVector<StdFunction, t_StaticAllocCount> StdFunctions;

public:
    StdFunctionSignal() : m_slots(nullptr)
    {
    }
    ~StdFunctionSignal()
    {
        clear();
    }

    StdFunctionSignal(const StdFunctionSignal& _other)
    {
        _other.m_mutex.lock();
        if (_other.m_slots)
        {
            m_slots = PHANTOM_NEW(StdFunctions);
            for (auto slot : *_other.m_slots)
                m_slots->push_back(slot);
        }
        _other.m_mutex.unlock();
    }
    StdFunctionSignal& operator=(const StdFunctionSignal& _other)
    {
        _other.m_mutex.lock();
        clear();
        if (_other.m_slots)
        {
            m_slots = PHANTOM_NEW(StdFunctions);
            for (auto slot : *_other.m_slots)
                m_slots->push_back(slot);
        }
        _other.m_mutex.unlock();
        return *this;
    }
    StdFunctionSignal(StdFunctionSignal&& _other)
    {
        _other.m_mutex.lock();
        clear();
        m_slots = _other.m_slots;
        _other.m_slots = nullptr;
        _other.m_mutex.unlock();
    }
    StdFunctionSignal& operator=(StdFunctionSignal&& _other)
    {
        m_mutex.lock();
        _other.m_mutex.lock();
        clear();
        m_slots = _other.m_slots;
        _other.m_slots = nullptr;
        m_mutex.unlock();
        _other.m_mutex.unlock();
        return *this;
    }
    template<class T, class Mptr>
    inline void connect(T* obj, Mptr func)
    {
        connect(StdFunction(obj, func));
    }
    inline void connect(R (*func)(Args...))
    {
        connect(StdFunction(func));
    }
    template<class T, class Mptr>
    inline void disconnect(T* obj, Mptr func)
    {
        disconnect(StdFunction(obj, func));
    }
    void connect(StdFunction const& a_StdFunction)
    {
        m_mutex.lock();
        if (m_slots == nullptr)
        {
            m_slots = PHANTOM_NEW(StdFunctions);
        }
        PHANTOM_ASSERT(std::find(m_slots->begin(), m_slots->end(), a_StdFunction) ==
                       m_slots->end());
        m_slots->push_back(a_StdFunction);
        m_mutex.unlock();
    }
    void disconnect(StdFunction const& a_StdFunction)
    {
        m_mutex.lock();
        PHANTOM_ASSERT(m_slots);
        auto found = std::find(m_slots->begin(), m_slots->end(), a_StdFunction);
        PHANTOM_ASSERT(found != m_slots->end());
        m_slots->erase(found);
        if (m_slots->empty())
        {
            PHANTOM_DELETE(StdFunctions) m_slots;
            m_slots = nullptr;
        }
        m_mutex.unlock();
    }

    inline void operator()(Args... args)
    {
        if (m_slots)
        {
            m_mutex.lock();
            StdFunctions slotsCopy = *m_slots;
            m_mutex.unlock();

            for (auto slot : slotsCopy)
            {
                slot(args...);
            }
        }
    }

private:
    inline void clear()
    {
        if (m_slots)
        {
            PHANTOM_DELETE(StdFunctions) m_slots;
            m_slots = nullptr;
        }
    }
    StdFunctions*  m_slots = nullptr;
    mutable MutexT m_mutex;
};
} // namespace phantom

#define o_connect_function(emitterPtr, signal, receiverPtr, func)                                  \
    ((emitterPtr)->signal)                                                                         \
    .connect(receiverPtr,                                                                          \
             &std::remove_pointer<std::remove_reference<decltype(receiverPtr)>::type>::type::func)
#define o_disconnect_function(emitterPtr, signal, receiverPtr, func)                               \
    ((emitterPtr)->signal)                                                                         \
    .disconnect(                                                                                   \
    receiverPtr,                                                                                   \
    &std::remove_pointer<std::remove_reference<decltype(receiverPtr)>::type>::type::func)
