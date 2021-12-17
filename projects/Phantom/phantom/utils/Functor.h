// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Delegate.h"
#include "DynDelegate.h"
#include "FunctorID.h"
#include "SmallVector.h"

#include <functional>
#include <phantom/alignof>
#include <type_traits>
/* *********************************************** */

namespace phantom
{
template<class S>
class Functor
#if defined(__HAUNT__)
{
}
#endif
;

template<class T>
struct IsFunctor : std::false_type
{
};

template<class R, class... Params>
struct IsFunctor<Functor<R(Params...)>> : std::true_type
{
};

template<class T>
struct _Functor_proxy_funcs
{
    void destructor() { reinterpret_cast<T*>(this)->~T(); }

    void* copyOrMoveConstructor(void* _mem, bool _move)
    {
        return _move ? new (_mem) T(std::move(*reinterpret_cast<T*>(this))) : new (_mem) T(*reinterpret_cast<T*>(this));
    }
};

template<class R, class... Params>
class Functor<R(Params...)>
{
    HAUNT_OFF struct _Generic_Functor
    {
    };
    using _GenericDtorPtr = void (_Generic_Functor::*)();
    using _GenericCopyOrMovePtr = void* (_Generic_Functor::*)(void*, bool);

    static const auto SmallBufferSize = 24;

public:
    template<class T>
    struct ImplConv
    {
        ImplConv(T);
    };
    using ThisType = Functor<R(Params...)>;

    using DelegateT = Delegate<R(Params...)>;
    using DynDelegateT = DynDelegate<R(Params...)>;

    template<class T>
    using EnableIfCustomArg = std::enable_if_t<!std::is_same<ThisType&, T>::value>;

    Functor() : m_dgt{} {}
    Functor(std::nullptr_t) : m_dgt{} {}

#if defined(__HAUNT__)
#    define PHANTOM_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(FUNCTOR, RET, ARGS, MYSELF) void
#else
#    define PHANTOM_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(FUNCTOR, RET, ARGS, MYSELF)                                  \
        typename std::enable_if_t<std::is_invocable_r_v<RET, FUNCTOR, ARGS> &&                                         \
                                  !std::is_same_v<std::decay_t<FUNCTOR>, MYSELF> &&                                    \
                                  !std::is_same_v<std::nullptr_t, FUNCTOR>>
#endif

    template<class T, class = PHANTOM_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(T, R, Params..., ThisType)>
    Functor(T _t)
    {
        using Tnoref = std::remove_reference_t<T>;
        struct _FunctorProxy
        {
            _FunctorProxy(Tnoref const& _l) : l(_l) {} // to ensure copyability
            _FunctorProxy(Tnoref&& _l) : l(std::move(_l)) {}
            inline R operator()(Params... a_Args) { return l(std::forward<Params>(a_Args)...); }
            Tnoref   l;
        };
        m_size = sizeof(_FunctorProxy);
        m_align = alignof(_FunctorProxy);
        auto* this__ = new (_alloc()) _FunctorProxy(std::forward<T>(_t));
        m_dgt = DelegateT(this__, &_FunctorProxy::operator());
        m_dtor = ::phantom::union_cast_nosizecheck<void*>(&_Functor_proxy_funcs<_FunctorProxy>::destructor);
        m_copyMove =
        ::phantom::union_cast_nosizecheck<void*>(&_Functor_proxy_funcs<_FunctorProxy>::copyOrMoveConstructor);
    }

    Functor(ThisType const& _other)
        : m_dtor(_other.m_dtor),
          m_copyMove{_other.m_copyMove},
          m_dgt(_other.m_dgt),
          m_size(_other.m_size),
          m_align(_other.m_align)

    {
        if (_lambda()) // not a simple delegate (might be a lambda or class with
                       // call operator())
        {
            union {
                _GenericCopyOrMovePtr mptr;
                void*                 ptr;
            } u{};
            u.ptr = m_copyMove;
            auto othis = _other.this_();
            m_dgt.getOpaque().setThis((othis->*u.mptr)(_alloc(), false));
        }
    }

    Functor(ThisType&& _other) noexcept
        : m_dtor(_other.m_dtor),
          m_copyMove(_other.m_copyMove),
          m_dgt(_other.m_dgt),
          m_size(_other.m_size),
          m_align(_other.m_align)
    {
        if (_lambda() && !_allocated())
        {
            union {
                _GenericCopyOrMovePtr mptr;
                void*                 ptr;
            } u{};
            u.ptr = m_copyMove;
            auto othis = _other.this_();
            m_dgt.getOpaque().setThis((othis->*u.mptr)(_alloc(), true));
            _other._dtor(othis);
        }
        _other.m_size = 0;
    }

    template<class T, class = PHANTOM_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(T, R, Params..., ThisType)>
    ThisType& operator=(T&& _t)
    {
        this->~ThisType();
        return *new (this) ThisType(std::forward<T>(_t));
    }

    template<typename T>
    ThisType& operator=(std::reference_wrapper<T> f) noexcept
    {
        return *this;
    }

    ThisType& operator=(ThisType const& _other)
    {
        this->~Functor();
        return *new (this) ThisType(_other);
    }

    ThisType& operator=(ThisType&& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(std::move(_other));
    }

    Functor(DelegateT const& _dgt) : m_dgt(_dgt) {}
    Functor(DelegateT&& _dgt) : m_dgt(_dgt) {}

    ThisType& operator=(DelegateT const& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(_other);
    }
    ThisType& operator=(DelegateT&& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(std::move(_other));
    }

    Functor(DynDelegateT const& _dgt) : m_dyndgt(_dgt), m_size(0x80000000) {}
    Functor(DynDelegateT&& _dgt) : m_dyndgt(_dgt), m_size(0x80000000) {}
    ThisType& operator=(DynDelegateT const& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(_other);
    }
    ThisType& operator=(DynDelegateT&& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(std::move(_other));
    }

    Functor(OpaqueDynDelegate const& _dgt) : m_dyndgt(_dgt), m_size(0x80000000) {}
    Functor(OpaqueDynDelegate&& _dgt) : m_dyndgt(_dgt), m_size(0x80000000) {}
    ThisType& operator=(OpaqueDynDelegate const& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(_other);
    }
    ThisType& operator=(OpaqueDynDelegate&& _other) noexcept
    {
        this->~Functor();
        return *new (this) ThisType(std::move(_other));
    }

    template<class T>
    Functor(T* a_pThis, R (T::*a_MemberFunc)(Params...)) : Functor(DelegateT(a_pThis, a_MemberFunc))
    {
    }

    template<class T>
    Functor(T const* a_pThis, R (T::*a_MemberFunc)(Params...) const) : Functor(DelegateT(a_pThis, a_PseudoMemberFunc))
    {
    }

    template<class T>
    Functor(T* a_pThis, R (*a_PseudoMemberFunc)(T*, Params...)) : Functor(DelegateT(a_pThis, a_PseudoMemberFunc))
    {
    }

    Functor(R (*a_In)(Params...)) : Functor(DelegateT(a_In)) {}

    ThisType& operator=(R (*a_In)(Params...))
    {
        this->~Functor();
        return *new (this) ThisType(a_In);
    }

    R operator()(Params... Args) const
    {
        if (_dyndgt())
            return m_dyndgt(std::forward<Params>(Args)...);
        else
            return m_dgt(std::forward<Params>(Args)...);
    }

    ~Functor()
    {
        auto this__ = this_();
        if (_lambda())
        {
            _dtor(this__);
            _free(this__);
        }
    }

    bool operator==(ThisType const& _other) const { return getID() == _other.getID(); }

    bool operator!=(ThisType const& _other) const { return getID() != _other.getID(); }

    bool operator==(std::nullptr_t) const { return empty(); }

    bool operator!=(std::nullptr_t) const { return !empty(); }

    void clear()
    {
        auto this__ = this_();
        if (_lambda())
        {
            _dtor(this__);
            _free(this__);
        }
        m_dgt = {};
        m_dtor = {};
        m_copyMove = {};
        m_size = 0;
    }

    operator bool() const { return (_dyndgt() && m_dyndgt) || m_dgt; }

    bool empty() const { return !((_dyndgt() && m_dyndgt) || m_dgt); }

    FunctorID getID() const { return _dyndgt() ? m_dyndgt.getID() : m_dgt.getID(); }

private:
    inline bool              _allocated() const { return m_size > SmallBufferSize || m_align > alignof(ThisType); }
    inline bool              _lambda() const { return (m_size & 0x7fffffff) != 0; }
    inline bool              _dyndgt() const { return (m_size & 0x80000000) != 0; }
    inline _Generic_Functor* this_() const { return reinterpret_cast<_Generic_Functor*>(m_dgt.getOpaque().getThis()); }
    inline void*             _alloc()
    {
        if (_allocated())
            return ::phantom::allocate(m_size, m_align);
        return m_smallBuffer;
    }
    inline void _free(void* _this)
    {
        if (_allocated())
            ::phantom::deallocate(_this);
    }
    inline void _dtor(_Generic_Functor* _t)
    {
        union {
            _GenericDtorPtr mptr;
            void*           ptr;
        } u{};
        u.ptr = m_dtor;
        (_t->*u.mptr)();
    }

private:
    union {
        struct
        {
            DelegateT m_dgt;
            void*     m_dtor;
            void*     m_copyMove;
        };
        DynDelegateT m_dyndgt;
    };
    uint32_t m_size{};
    uint32_t m_align{};
    char     m_smallBuffer[SmallBufferSize] = {};
};
static_assert(sizeof(Functor<int()>) == 64, "prout");

template<class T, class U, class R, class... Params>
auto makeFunctor(T* a_pThis, R (U::*a_Func)(Params...))
{
    return Functor<R(Params...)>(a_pThis, a_Func);
}

template<class T, class R, class... Params>
auto makeFunctor(R (*a_Func)(Params...))
{
    return Functor<R(Params...)>(a_Func);
}

} // namespace phantom
