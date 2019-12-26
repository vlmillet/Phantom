// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "Delegate.h"

#include <phantom/new>

HAUNT_STOP;

#define PHANTOM_DEBUG_RECORD_SIGNAL_HISTORY 0

#if PHANTOM_DEBUG_RECORD_SIGNAL_HISTORY

#    include "History.h"

phantom::OpaqueDelegate::MFPtr RecordedMethod();
static const int               callstackSize = 8;
static const int               historySize = 256;

#    define PHANTOM_RECORD_SIGNAL_HISTORY(__type)                                                  \
        if (RecordedMethod() == nullptr ||                                                         \
            a_Delegate.getOpaque().getFunction() == RecordedMethod())                              \
        {                                                                                          \
            auto object = a_Delegate.getOpaque().getThis();                                        \
            m_history.record("%s : %p", #__type, object);                                          \
        }
#else
#    define PHANTOM_RECORD_SIGNAL_HISTORY(__type)
#endif

#include "newImpl.h"

#include <algorithm>

namespace phantom
{
template<class Signature, class MutexT = NullMutex, size_t t_StaticAllocCount = 4>
class DelegateSignal;

template<class R, class MutexT, size_t t_StaticAllocCount, class... Args>
class DelegateSignal<R(Args...), MutexT, t_StaticAllocCount>
{
public:
    typedef Delegate<R(Args...)>                      Delegate;
    typedef SmallVector<Delegate, t_StaticAllocCount> Delegates;

public:
    DelegateSignal() : m_slots(nullptr)
    {
    }
    ~DelegateSignal()
    {
        clear();
    }

    DelegateSignal(const DelegateSignal& _other)
    {
        _other.m_mutex.lock();
        if (_other.m_slots)
        {
            m_slots = PHANTOM_NEW(Delegates);
            for (auto slot : *_other.m_slots)
                m_slots->push_back(slot);
        }
        _other.m_mutex.unlock();
    }
    DelegateSignal& operator=(const DelegateSignal& _other)
    {
        _other.m_mutex.lock();
        clear();
        if (_other.m_slots)
        {
            m_slots = PHANTOM_NEW(Delegates);
            for (auto slot : *_other.m_slots)
                m_slots->push_back(slot);
        }
        _other.m_mutex.unlock();
        return *this;
    }
    DelegateSignal(DelegateSignal&& _other)
    {
        _other.m_mutex.lock();
        clear();
        m_slots = _other.m_slots;
        _other.m_slots = nullptr;
        _other.m_mutex.unlock();
    }
    DelegateSignal& operator=(DelegateSignal&& _other)
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
        connect(Delegate(obj, func));
    }
    inline void connect(R (*func)(Args...))
    {
        connect(Delegate(func));
    }
    template<class T, class Mptr>
    inline void disconnect(T* obj, Mptr func)
    {
        disconnect(Delegate(obj, func));
    }
    void connect(Delegate const& a_Delegate)
    {
        m_mutex.lock();
        PHANTOM_RECORD_SIGNAL_HISTORY(Connect);

        if (m_slots == nullptr)
        {
            m_slots = PHANTOM_NEW(Delegates);
        }
        PHANTOM_ASSERT(std::find(m_slots->begin(), m_slots->end(), a_Delegate) == m_slots->end());
        m_slots->push_back(a_Delegate);
        m_mutex.unlock();
    }
    void disconnect(Delegate const& a_Delegate)
    {
        m_mutex.lock();
        PHANTOM_RECORD_SIGNAL_HISTORY(Disconnect);
        PHANTOM_ASSERT(m_slots);
        auto found = std::find(m_slots->begin(), m_slots->end(), a_Delegate);
        PHANTOM_ASSERT(found != m_slots->end());
        m_slots->erase(found);
        if (m_slots->empty())
        {
            PHANTOM_DELETE_DYN m_slots;
            m_slots = nullptr;
        }
        m_mutex.unlock();
    }

    inline void operator()(Args... args)
    {
        emit(args...);
    }

    inline void emit(Args... args)
    {
        if (m_slots)
        {
            m_mutex.lock();
            Delegates slotsCopy = *m_slots;
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
            PHANTOM_DELETE_DYN m_slots;
            m_slots = nullptr;
        }
    }
    Delegates*     m_slots = nullptr;
    mutable MutexT m_mutex;

#if PHANTOM_DEBUG_RECORD_SIGNAL_HISTORY
    History<callstackSize> m_history;
#endif
};
} // namespace phantom

#define o_connect_delegate(emitterPtr, signal, receiverPtr, func)                                  \
    ((emitterPtr)->signal)                                                                         \
    .connect(receiverPtr,                                                                          \
             &std::remove_pointer<std::remove_reference<decltype(receiverPtr)>::type>::type::func)
#define o_disconnect_delegate(emitterPtr, signal, receiverPtr, func)                               \
    ((emitterPtr)->signal)                                                                         \
    .disconnect(                                                                                   \
    receiverPtr,                                                                                   \
    &std::remove_pointer<std::remove_reference<decltype(receiverPtr)>::type>::type::func)
