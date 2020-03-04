// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include <haunt>
HAUNT_STOP;

#include <phantom/alignof>
#include <phantom/plugin.h>

namespace phantom
{
template<class, class>
class Signal;

PHANTOM_EXPORT_PHANTOM void* Emitter();

namespace _Signal
{
PHANTOM_EXPORT_PHANTOM void* AllocateSlot();
PHANTOM_EXPORT_PHANTOM void  DeallocateSlot(void*);

struct Emitter;
struct EmissionFrameStack;
struct SlotBase
{
    SlotBase* m_pNext;
    FunctorID m_ID;
};
template<class... Parms>
struct Slot;

class PHANTOM_EXPORT_PHANTOM EmissionFrame
{
    friend struct EmissionFrameStack;
    friend struct Emitter;
    template<class, class>
    friend class phantom::Signal;
    template<class...>
    friend struct Slot;
    friend void* phantom::Emitter();

    static const int MaxStackSize = 256;
    struct Stack;

    static inline void PushEmitter(void const* a_pEmitter);
    ;
    static inline void PopEmitter();

    static EmissionFrame* Top();
    static void           OnSlotDestruction(SlotBase* a_pSlot);
    static void           PushSlot(SlotBase** a_ppNext);
    static void           PopSlot();

    void* getEmitter() const { return m_pEmitter; }

    inline EmissionFrame() = default;
    inline EmissionFrame(void* a_pEmitter) : m_pEmitter(a_pEmitter) {}

    void*      m_pEmitter = nullptr;
    SlotBase** m_ppNext = nullptr;
};

struct Emitter
{
    Emitter(void* _this) { EmissionFrame::PushEmitter(_this); }
    ~Emitter() { EmissionFrame::PopEmitter(); }
    operator bool() { return true; }
};

template<class SignIn, class SignOut>
struct StaticAssertSlotArgCompatible;

template<>
struct StaticAssertSlotArgCompatible<void(), void()>
{
};

template<class In, class... InParams, class Out, class... Params>
struct StaticAssertSlotArgCompatible<void(In, InParams...), void(Out, Params...)>
{
    PHANTOM_STATIC_ASSERT((std::is_same<In, Out>::value ||
                           (std::is_same<void*, Out>::value && std::is_pointer<In>::value)),
                          "SlotCast : args not compatible");
};

template<class Sign>
struct SlotCaster;

template<class R, class... Params>
struct SlotCaster<R(Params...)>
{
    template<class T, class... InParams>
    static auto Cast(R (T::*a_In)(InParams...))
    {
        PHANTOM_STATIC_ASSERT(sizeof...(InParams) == sizeof...(Params), "SlotCast : parameter count does not match");
        StaticAssertSlotArgCompatible<void(InParams...), void(Params...)> _sa;
        (void)_sa;
        return (R(T::*)(Params...))a_In;
    }
    template<class... InParams>
    static auto Cast(R (*a_In)(InParams...))
    {
        PHANTOM_STATIC_ASSERT(sizeof...(InParams) == sizeof...(Params), "SlotCast : parameter count does not match");
        StaticAssertSlotArgCompatible<void(InParams...), void(Params...)> _sa;
        (void)_sa;
        return (R(*)(Params...))a_In;
    }
    template<class T, class... InParams>
    static auto Cast(R (*a_In)(T*, InParams...))
    {
        PHANTOM_STATIC_ASSERT(sizeof...(InParams) == sizeof...(Params), "SlotCast : parameter count does not match");
        StaticAssertSlotArgCompatible<void(InParams...), void(Params...)> _sa;
        (void)_sa;
        return (R(*)(T*, Params...))a_In;
    }
};

template<class Type, class Sign>
struct SignaturePrepend;

template<class Type, class R, class... Params>
struct SignaturePrepend<Type, R(Params...)>
{
    using type = R(Type, Params...);
};

template<class R, class... Params>
struct SignaturePrepend<void, R(Params...)>
{
    using type = void;
};

template<class Sign, class Type>
struct SignatureAppend;

template<class Type, class R, class... Params>
struct SignatureAppend<R(Params...), Type>
{
    using type = R(Params..., Type);
};

template<class Sign>
struct SignaturePopFront;

template<class R, class Front, class... Params>
struct SignaturePopFront<R(Front, Params...)>
{
    using type = R(Params...);
};

template<class R>
struct SignaturePopFront<R()>
{
    using type = void;
};

template<class List, class Sign>
struct SignaturePopBackH;

template<class List, class R, class Back>
struct SignaturePopBackH<List, R(Back)>
{
    using type = List;
};

template<class List, class R, class Front, class... Params>
struct SignaturePopBackH<List, R(Front, Params...)>
    : SignaturePopBackH<typename SignatureAppend<List, Front>::type, R(Params...)>
{
};

template<class Sign>
struct SignaturePopBack;

template<class R, class... Params>
struct SignaturePopBack<R(Params...)> : SignaturePopBackH<R(), R(Params...)>
{
};

template<class R>
struct SignaturePopBack<R()>
{
    using type = void;
};

template<class Sign>
struct SignatureBack;

template<class R, class Back>
struct SignatureBack<R(Back)>
{
    using type = Back;
};

template<class R, class Front, class... Params>
struct SignatureBack<R(Front, Params...)> : SignatureBack<R(Params...)>
{
};

template<class R>
struct SignatureBack<R()>
{
    using type = void;
};

template<class SignalClass, class Used, class Unused>
class SignalImpl;

template<class SignalClass>
class SignalImpl<SignalClass, void, void>
{
    template<class, class>
    friend class phantom::Signal;
    template<class, class, class>
    friend class SignalImpl;

private:
    struct Dummy
    {
    };

protected:
    void connect(Dummy);
    void disconnect(Dummy);
};

template<class SignalClass, class... UsedParms, class... UnusedParms>
class SignalImpl<SignalClass, void(UsedParms...), void(UnusedParms...)>
    : public SignalImpl<
      SignalClass, typename SignaturePopBack<void(UsedParms...)>::type,
      typename SignaturePrepend<typename SignatureBack<void(UsedParms...)>::type, void(UnusedParms...)>::type>
{
    template<class, class>
    friend class phantom::Signal;
    template<class, class, class>
    friend class SignalImpl;

    using BaseType =
    SignalImpl<SignalClass, typename SignaturePopBack<void(UsedParms...)>::type,
               typename SignaturePrepend<typename SignatureBack<void(UsedParms...)>::type, void(UnusedParms...)>::type>;

    using FunctorType = Functor<void(UsedParms...)>;
    using SignalFunctorType = Functor<void(UsedParms..., UnusedParms...)>;
    using SignalType = phantom::Signal<void(UsedParms...)>;

    using BaseType::connect;
    using BaseType::disconnect;

    /// @brief connects another signal
    template<class T>
    FunctorID connect(T* _emitter, SignalType(*_other))
    {
        return connect((_emitter->*_other).MakeEmissionFunctor(_emitter));
    }

    /// @brief disconnects another signal
    template<class T>
    void disconnect(T* _emitter, SignalType& _other)
    {
        return disconnect((_emitter->*_other).MakeEmissionFunctor(_emitter));
    }

    // lambda & co
    template<class T, class = std::enable_if_t<std::is_convertible<T, FunctorType>::value>>
    FunctorID connect(T&& a_Functor, void* a_pLambdaDiscriminant = nullptr)
    {
        return connect(FunctorType(a_Functor, a_pLambdaDiscriminant));
    }

    // function pointer
    template<class R>
    FunctorID connect(R (*a_Func)(UsedParms...))
    {
        return static_cast<SignalClass*>(this)->_connect(
        FunctorID(a_Func, nullptr),
        SignalFunctorType([a_Func](UsedParms... a_Used, UnusedParms...) { a_Func(a_Used...); }));
    }

    // function pointer
    template<class R, class... OtherParams>
    FunctorID connect(R (*a_Func)(OtherParams...))
    {
        return static_cast<SignalClass*>(this)->_connect(
        FunctorID(a_Func, nullptr), SignalFunctorType([a_Func](UsedParms... a_Used, UnusedParms...) {
            _Signal::SlotCaster<R(UsedParms...)>::Cast(a_Func)(a_Used...);
        }));
    }

    // instance + method pointer
    template<class T, class R>
    FunctorID connect(T* a_pThis, R (T::*a_Func)(UsedParms...))
    {
        return static_cast<SignalClass*>(this)->_connect(
        FunctorID(a_pThis, MethodClosure(a_Func).getAddress()),
        SignalFunctorType([a_pThis, a_Func](UsedParms... a_Used, UnusedParms...) { (a_pThis->*a_Func)(a_Used...); }));
    }
    template<class T, class R>
    FunctorID connect(T const* a_pThis, R (T::*a_Func)(UsedParms...) const)
    {
        return static_cast<SignalClass*>(this)->_connect(
        FunctorID(a_pThis, MethodClosure(a_Func).getAddress()),
        SignalFunctorType([a_pThis, a_Func](UsedParms... a_Used, UnusedParms...) { (a_pThis->*a_Func)(a_Used...); }));
    }

    // @brief connects a non const object + method
    template<class T, class R, class... OtherParams>
    FunctorID connect(T* a_pThis, R (T::*a_Func)(OtherParams...))
    {
        return static_cast<SignalClass*>(this)->_connect(
        FunctorID(a_pThis, MethodClosure(a_Func).getAddress()),
        SignalFunctorType([a_pThis, a_Func](UsedParms... a_Used, UnusedParms...) {
            (a_pThis->*_Signal::SlotCaster<R(UsedParms...)>::Cast(a_Func))(a_Used...);
        }));
    }

    // @brief connects a function acting as a method
    template<class T, class R>
    FunctorID connect(T* a_pThis, R (*a_Func)(T*, UsedParms...))
    {
        return connect(Functor<R(UsedParms...)>(a_pThis, a_Func));
    }

    // @brief connects a function acting as a method
    template<class T, class R, class... OtherParams>
    FunctorID connect(T* a_pThis, R (*a_Func)(T*, OtherParams...))
    {
        return static_cast<SignalClass*>(this)->_connect(
        FunctorID(a_pThis, MethodClosure(a_Func).getAddress()),
        SignalFunctorType([a_pThis, a_Func](UsedParms... a_Used, UnusedParms...) {
            _Signal::SlotCaster<R(UsedParms...)>::PHANTOM_T Cast<T>(a_Func)(a_pThis, a_Used...);
        }));
    }

    // @brief disconnects a lambda
    template<class T, class = std::enable_if_t<std::is_convertible<T, FunctorType>::value>>
    void disconnect(T&& a_Functor, void* a_pLambdaDiscriminant = nullptr)
    {
        return static_cast<SignalClass*>(this)->_disconnect(FunctorType(a_Functor, a_pLambdaDiscriminant).getID());
    }

    // @brief disconnects a compatible functor
    template<class R>
    void disconnect(Functor<R(UsedParms...)> const& a_Functor)
    {
        static_cast<SignalClass*>(this)->_disconnect(a_Functor.getID());
    }

    template<class R>
    void disconnect(Delegate<R(UsedParms...)> const& a_Delegate)
    {
        static_cast<SignalClass*>(this)->_disconnect(a_Delegate.getID());
    }

    void disconnect(Delegate<void(UsedParms...)> const& a_Delegate)
    {
        static_cast<SignalClass*>(this)->_disconnect(a_Delegate.getID());
    }

    // functor
    template<class R>
    FunctorID connect(Functor<R(UsedParms...)> const& a_Functor)
    {
        return static_cast<SignalClass*>(this)->_connect(
        a_Functor.getID(),
        SignalFunctorType([a_Functor](UsedParms... a_Used, UnusedParms...) { a_Functor(a_Used...); }));
    }

    // functor
    template<class R>
    FunctorID connect(Functor<R(UsedParms...)>&& a_Functor)
    {
        return static_cast<SignalClass*>(this)->_connect(
        a_Functor.getID(),
        SignalFunctorType(LambdaCapture(
        std::move(a_Functor),
        [](Functor<R(UsedParms...)>& a_Functor, UsedParms... a_Used, UnusedParms...) { a_Functor(a_Used...); })));
    }
};

template<class... Parms>
struct Slot
{
    using ThisType = Slot<Parms...>;
    void destroy()
    {
        EmissionFrame::OnSlotDestruction(reinterpret_cast<SlotBase*>(this));
        this->~ThisType();
        _Signal::DeallocateSlot(this);
    }
    ThisType*               m_pNext;
    FunctorID               m_ID;
    Functor<void(Parms...)> m_Functor;
};

struct EmissionFrameStack
{
    friend class EmissionFrame;
    friend void* phantom::Emitter();

private:
    static inline EmissionFrameStack& Current()
    {
        static thread_local EmissionFrameStack c;
        return c;
    }
    EmissionFrame&                                                                top();
    EmissionFrame&                                                                at(intptr_t i);
    std::aligned_storage_t<sizeof(EmissionFrame), PHANTOM_ALIGNOF(EmissionFrame)> stack[EmissionFrame::MaxStackSize];
    intptr_t                                                                      pointer = -1;
};

inline EmissionFrame& EmissionFrameStack::top()
{
    return (EmissionFrame&)stack[pointer];
}

inline EmissionFrame& EmissionFrameStack::at(intptr_t i)
{
    return (EmissionFrame&)stack[i];
}

inline void EmissionFrame::PushEmitter(void const* a_pEmitter)
{
    PHANTOM_ASSERT(EmissionFrameStack::Current().pointer < EmissionFrame::MaxStackSize - 1);
    new (&EmissionFrameStack::Current().stack[++EmissionFrameStack::Current().pointer])
    EmissionFrame(const_cast<void*>(a_pEmitter));
}

inline void EmissionFrame::PopEmitter()
{
    PHANTOM_ASSERT(EmissionFrameStack::Current().pointer > -1);
    --EmissionFrameStack::Current().pointer;
}

inline void EmissionFrame::PushSlot(SlotBase** a_ppNext)
{
    PHANTOM_ASSERT(EmissionFrameStack::Current().pointer > -1,
                   "'PHANTOM_EMIT' has been forgotten before call to signal");
    EmissionFrame& frame = EmissionFrameStack::Current().top();
    frame.m_ppNext = a_ppNext;
}

inline void EmissionFrame::OnSlotDestruction(SlotBase* a_pSlot)
{
    auto&    stack = EmissionFrameStack::Current();
    intptr_t i = EmissionFrameStack::Current().pointer + 1;
    while (i--)
    {
        EmissionFrame& f = stack.at(i);
        if (*(f.m_ppNext) == a_pSlot) // current emission next slot is being destroyed...
        {
            *(f.m_ppNext) = a_pSlot->m_pNext; // ... update next pointer to next's next
        }
    }
}

inline void EmissionFrame::PopSlot() {}

inline EmissionFrame* EmissionFrame::Top()
{
    if (EmissionFrameStack::Current().pointer == -1)
        return nullptr;
    return &EmissionFrameStack::Current().top();
}

} // namespace _Signal

inline void* Emitter()
{
    return _Signal::EmissionFrameStack::Current().top().getEmitter();
}

} // namespace phantom
