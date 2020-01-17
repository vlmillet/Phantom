// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "Functor.h"

#include <phantom/detail/LambdaCapture.h>
#include <phantom/detail/MethodClosure.h>
#include <phantom/detail/Signal.h>
#include <phantom/thread/RecursiveSpinMutex.h>

namespace phantom
{
template<class Sign, class SignalMutexType>
class Signal;

PHANTOM_EXPORT_PHANTOM void* Emitter();

template<class SignalMutexType, class... Parms>
class Signal<void(Parms...), SignalMutexType>
    : public _Signal::SignalImpl<
      Signal<void(Parms...)>, typename _Signal::SignaturePopBack<void(Parms...)>::type,
      typename _Signal::SignaturePrepend<typename _Signal::SignatureBack<void(Parms...)>::type, void()>::type>
{
public:
    using SignalLockGuardType = LockGuard<SignalMutexType>;
    template<class, class, class>
    friend class _Signal::SignalImpl;
    using ThisType = Signal<void(Parms...)>;
    using BaseType = _Signal::SignalImpl<
    ThisType, typename _Signal::SignaturePopBack<void(Parms...)>::type,
    typename _Signal::SignaturePrepend<typename _Signal::SignatureBack<void(Parms...)>::type, void()>::type>;
    using SlotType = _Signal::Slot<Parms...>;

    using FunctorType = Functor<void(Parms...)>;

    using BaseType::connect;
    using BaseType::disconnect;

    Signal() = default;

    template<class T,
             class = std::enable_if_t<std::is_same<T, ThisType>::value && IsMoveConstructible<SignalMutexType>::value>>
    Signal(T&& _temp) : m_Mutex(std::move(_temp.m_Mutex))
    {
        m_pHead = _temp.m_pHead;
        m_pQueue = _temp.m_pQueue;
        _temp.m_pHead = nullptr;
        _temp.m_pQueue = nullptr;
    }

    template<class T,
             class = std::enable_if_t<std::is_same<T, ThisType>::value && IsMoveAssignable<SignalMutexType>::value>>
    ThisType& operator=(T&& _temp)
    {
        m_Mutex = std::move(_temp.m_Mutex);
        m_pHead = _temp.m_pHead;
        m_pQueue = _temp.m_pQueue;
        _temp.m_pHead = nullptr;
        _temp.m_pQueue = nullptr;
        return *this;
    }

    Signal(ThisType const& _other) = delete;
    ThisType& operator=(ThisType const& _other) = delete;

    ~Signal() { clear(); }

    void clear()
    {
        SlotType* pCurr = m_pHead;
        while (pCurr)
        {
            SlotType* pNext = pCurr->m_pNext;
            pCurr->destroy();
            pCurr = pNext;
        }
    }

    template<class T>
    inline FunctorType MakeEmissionFunctor(T* _emitter)
    {
        return [_emitter, this](Parms... a_Args) { this->emit(_emitter, a_Args...); };
    }

    /// @brief connects another signal
    template<class T>
    FunctorID connect(T* _emitter, ThisType(*_other))
    {
        return connect((_emitter->*_other).MakeEmissionFunctor(_emitter));
    }

    /// @brief disconnects another signal
    template<class T>
    void disconnect(T* _emitter, ThisType& _other)
    {
        return disconnect((_emitter->*_other).MakeEmissionFunctor(_emitter));
    }

    /// @brief connects a lambda or other functor object
    template<class T, class = typename FunctorType::template EnableIfCustomArg<T>>
    FunctorID connect(T&& a_Functor, void* a_pLambdaDiscriminant = nullptr)
    {
        return connect(FunctorType(a_Functor, a_pLambdaDiscriminant));
    }

    // @brief connects a void Functor with same parameters
    FunctorID connect(FunctorType const& a_Functor) { return _connect(a_Functor.getID(), a_Functor); }

    // @brief connects a void Functor with same parameters (move semantic)
    FunctorID connect(FunctorType&& a_Functor) { return _connect(a_Functor.getID(), std::move(a_Functor)); }

    // @brief connects any other non void Functor with same parameters
    template<class R>
    FunctorID connect(Functor<R(Parms...)> const& a_Functor)
    {
        return _connect(a_Functor.getID(), [a_Functor](Parms... a_Args) { a_Functor(a_Args...); });
    }

    // @brief connects a non-void return delegate
    template<class R>
    FunctorID connect(Delegate<R(Parms...)> const& a_Delegate)
    {
        return _connect(a_Delegate.getID(), [a_Delegate]() { a_Delegate(); });
    }

    // @brief connects a void return delegate
    FunctorID connect(Delegate<void(Parms...)> const& a_Delegate) { return connect(FunctorType(a_Delegate)); }

    // @brief connects a non-void return delegate
    template<class R>
    FunctorID connect(Delegate<R(Parms...)>&& a_Delegate)
    {
        return _connect(a_Delegate.getID(), [a_Delegate]() { a_Delegate(); });
    }

    // @brief connects a void return delegate
    FunctorID connect(Delegate<void(Parms...)>&& a_Delegate) { return connect(FunctorType(a_Delegate)); }

    // @brief connects a function
    template<class R>
    FunctorID connect(R (*a_Func)(Parms...))
    {
        return connect(Functor<R(Parms...)>(a_Func));
    }

    // @brief connects a function
    template<class R, class... OtherParams>
    FunctorID connect(R (*a_Func)(OtherParams...))
    {
        return connect(Functor<R(Parms...)>(_Signal::SlotCaster<R(Parms...)>::Cast(a_Func)));
    }

    // @brief connects a non const object + method
    template<class T, class R>
    FunctorID connect(T* a_pThis, R (T::*a_Func)(Parms...))
    {
        return connect(Functor<R(Parms...)>(a_pThis, a_Func));
    }

    // @brief connects a non const object + method
    template<class T, class R, class... OtherParams>
    FunctorID connect(T* a_pThis, R (T::*a_Func)(OtherParams...))
    {
        return connect(Functor<R(Parms...)>(a_pThis, _Signal::SlotCaster<R(Parms...)>::Cast(a_Func)));
    }

    // @brief connects a function acting as a method
    template<class T, class R>
    FunctorID connect(T* a_pThis, R (*a_Func)(T*, Parms...))
    {
        return connect(Functor<R(Parms...)>(a_pThis, a_Func));
    }

    // @brief connects a function acting as a method
    template<class T, class R, class... OtherParams>
    FunctorID connect(T* a_pThis, R (*a_Func)(T*, OtherParams...))
    {
        return connect(Functor<R(Parms...)>(a_pThis, _Signal::SlotCaster<R(Parms...)>::PHANTOM_T Cast<T>(a_Func)));
    }

    // @brief disconnects a lambda
    template<class T, class = typename FunctorType::template EnableIfCustomArg<T>>
    void disconnect(T&& a_Functor, void* a_pLambdaDiscriminant = nullptr)
    {
        _disconnect(FunctorType(a_Functor, a_pLambdaDiscriminant).getID());
    }

    // @brief disconnects a compatible functor
    template<class R>
    void disconnect(Functor<R(Parms...)> const& a_Functor)
    {
        _disconnect(a_Functor.getID());
    }

    // @brief disconnects a function
    template<class R, class... OtherParams>
    void disconnect(R (*a_Func)(OtherParams...))
    {
        _disconnect(FunctorID(size_t(a_Func), 0));
    }

    // @brief disconnects a this/method couple
    template<class T, class R, class... OtherParams>
    void disconnect(T* a_pThis, R (T::*a_Func)(OtherParams...))
    {
        _disconnect(FunctorID(a_pThis, MethodClosure(a_Func).getAddress()));
    }

    void disconnect(FunctorID a_ID) { _disconnect(a_ID); }

    template<class R>
    void disconnect(Delegate<R(Parms...)> const& a_Delegate)
    {
        _disconnect(a_Delegate.getID());
    }

    void disconnect(Delegate<void(Parms...)> const& a_Delegate) { _disconnect(a_Delegate.getID()); }

    template<class R>
    void disconnect(Delegate<R(Parms...)>&& a_Delegate)
    {
        _disconnect(a_Delegate.getID());
    }

    void disconnect(Delegate<void(Parms...)>&& a_Delegate) { _disconnect(a_Delegate.getID()); }

    void operator()(Parms... a_Args) const
    {
        SignalLockGuardType __lck_(m_Mutex);
        SlotType*           pCurr = m_pHead;
        while (pCurr)
        {
            SlotType* pNext = pCurr->m_pNext;
            _Signal::EmissionFrame::PushSlot((_Signal::SlotBase**)&pNext);
            pCurr->m_Functor(a_Args...);
            _Signal::EmissionFrame::PopSlot();
            pCurr = pNext;
        }
    }

    void emit(void* a_pThis, Parms... a_Args) const
    {
        _Signal::EmissionFrame::PushEmitter(a_pThis);
        operator()(a_Args...);
        _Signal::EmissionFrame::PopEmitter();
    }

    inline SignalMutexType& mutex() { return m_Mutex; }

private:
    bool _isConnected(FunctorID a_ID) const
    {
        SlotType* pCurr = m_pHead;
        while (pCurr)
        {
            if (pCurr->m_ID == a_ID)
            {
                return true;
            }
            pCurr = pCurr->m_pNext;
        }
        return false;
    }

    FunctorID _connect(FunctorID a_ID, FunctorType&& a_Functor)
    {
        _connect(new (_Signal::AllocateSlot()) SlotType{nullptr, a_ID, std::move(a_Functor)});
        return a_ID;
    }

    FunctorID _connect(FunctorID a_ID, FunctorType const& a_Functor)
    {
        _connect(new (_Signal::AllocateSlot()) SlotType{nullptr, a_ID, a_Functor});
        return a_ID;
    }

    void _connect(SlotType* a_pSlot)
    {
        PHANTOM_ASSERT(!_isConnected(a_pSlot->m_ID), "functor already connected");
        if (m_pHead == nullptr)
        {
            m_pHead = a_pSlot;
            m_pQueue = a_pSlot;
        }
        else
        {
            m_pQueue->m_pNext = a_pSlot;
            m_pQueue = a_pSlot;
        }
    }

    void _disconnect(FunctorID a_ID)
    {
        // if current slot is being executed, wait until emission is finished
        PHANTOM_ASSERT(m_pHead);
        if (m_pHead->m_ID == a_ID)
        {
            if (m_pHead == m_pQueue)
                m_pQueue = nullptr;
            SlotType* pTemp = m_pHead;
            m_pHead = m_pHead->m_pNext;
            pTemp->destroy();
            return;
        }
        SlotType* pPrev = m_pHead;
        SlotType* pCurr = m_pHead->m_pNext;
        while (pCurr)
        {
            if (pCurr->m_ID == a_ID)
            {
                if (pCurr == m_pQueue)
                    m_pQueue = pPrev;
                pPrev->m_pNext = pCurr->m_pNext;
                pCurr->destroy();
                return;
            }
            pPrev = pCurr;
            pCurr = pCurr->m_pNext;
        }
        PHANTOM_UNREACHABLE();
    }

private:
    SlotType*               m_pHead = nullptr;
    SlotType*               m_pQueue = nullptr;
    mutable SignalMutexType m_Mutex;
};
} // namespace phantom
#define PHANTOM_EMIT if (::phantom::_Signal::Emitter _PHNTM_emitter = this)

#define PHANTOM_CONNECT(Emitter, Signal, Receiver, Method) (Emitter)->Signal.connect(PHANTOM_SLOT(Receiver, Method))

#define PHANTOM_SLOT(Receiver, Method) Receiver, &std::remove_pointer_t<decltype(Receiver)>::Method

#define PHANTOM_DISCONNECT(Emitter, Signal, Receiver, Method)                                                          \
    (Emitter)->Signal.disconnect(PHANTOM_SLOT(Receiver, Method))
