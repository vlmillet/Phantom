// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

//	Delegate class highly inpired by "FastDelegate" using modern C++11 variadic templates

#pragma once

#include "FunctorID.h"

#include <haunt>
#include <memory.h>
#include <phantom/detail/MethodClosure.h>
#include <phantom/detail/macros.h>
#include <phantom/detail/typedefs.h>

namespace phantom
{
namespace detail
{
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
class __single_inheritance DelegateGenericClass;
class DelegateGenericClass
{
}; ///< @off
#else
class DelegateGenericClass
{
}; ///< @off
#endif
} // namespace detail
HAUNT_ON class OpaqueDelegate
{
    HAUNT_OFF protected : HAUNT_OFF typedef void (detail::DelegateGenericClass::*MFPtr)();
    HAUNT_OFF detail::DelegateGenericClass* m_pThis;
    HAUNT_OFF MFPtr                         m_pFunction;

public:
    OpaqueDelegate() : m_pThis(0), m_pFunction(0){};
    void clear()
    {
        m_pThis = 0;
        m_pFunction = 0;
    }

public:
    inline bool operator==(const OpaqueDelegate& other) const
    {
        return m_pThis == other.m_pThis && m_pFunction == other.m_pFunction;
    }
    inline bool operator!=(const OpaqueDelegate& other) const { return !operator==(other); }

    inline bool operator!() const { return m_pThis == 0 && m_pFunction == 0; }
    inline bool empty() const { return m_pThis == 0 && m_pFunction == 0; }

    operator bool() const { return m_pThis || m_pFunction; }

    int getThisOffset() const { return Closure(MethodClosure(m_pFunction)).offset; }

    MethodClosure getMethod() const { return MethodClosure(m_pFunction); }
    void*         getThis() const { return m_pFunction ? m_pThis : nullptr; }
    void*         getFunction() const { return m_pFunction ? nullptr : m_pThis; }
    FunctorID     getID() const { return FunctorID(m_pThis, MethodClosure(m_pFunction).getAddress()); }

public:
    OpaqueDelegate& operator=(const OpaqueDelegate& right)
    {
        setOpaqueFrom(right);
        return *this;
    }
    inline bool operator<(const OpaqueDelegate& right) const
    {
        if (m_pThis != right.m_pThis)
            return m_pThis < right.m_pThis;
        return memcmp(&m_pFunction, &right.m_pFunction, sizeof(m_pFunction)) < 0;
    }
    inline bool operator>(const OpaqueDelegate& right) const { return right.operator<(*this); }
    OpaqueDelegate(const OpaqueDelegate& right) : m_pThis(right.m_pThis), m_pFunction(right.m_pFunction) {}

protected:
    void setOpaqueFrom(const OpaqueDelegate& right)
    {
        m_pFunction = right.m_pFunction;
        m_pThis = right.m_pThis;
    }
};

namespace detail
{
template<class OutputClass, class InputClass>
inline OutputClass implicit_cast(InputClass input)
{
    return input;
}
//
//         template <class OutputClass, class InputClass>
//         union union_cast_hack {
//             OutputClass out;
//             InputClass in;
//         };
//
//         template <class OutputClass, class InputClass>
//         inline OutputClass union_cast(const InputClass input) {
//             union_cast_hack<OutputClass, InputClass> u;
//             PHANTOM_STATIC_ASSERT(sizeof(InputClass) == sizeof(u) && sizeof(InputClass) ==
//             sizeof(OutputClass), "cannot union_cast the given types"); u.in = input; return
//             u.out;
//         }

const int delegate_generic_class_mfptr_size = sizeof(void (DelegateGenericClass::*)());

template<int N>
struct MFHacker
{
    template<class t_Ty, class t_AnyFP, class t_MFPtr>
    inline static DelegateGenericClass* Convert(t_Ty* pthis, t_AnyFP function_to_bind, t_MFPtr& bound_func)
    {
        PHANTOM_STATIC_ASSERT(N < 0, "delegates not supported on this compiler");
        return 0;
    }
};

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

template<>
struct MFHacker<delegate_generic_class_mfptr_size>
{
    template<class t_Ty, class t_AnyFP, class t_MFPtr>
    inline static DelegateGenericClass* Convert(t_Ty* pthis, t_AnyFP function_to_bind, t_MFPtr& bound_func)
    {
        MethodClosure closure;
        closure.setClosurePointer(function_to_bind);
        bound_func = closure;
        return reinterpret_cast<DelegateGenericClass*>(pthis);
    }
};

template<>
struct MFHacker<delegate_generic_class_mfptr_size + sizeof(int)>
{
    template<class t_Ty, class t_AnyFP, class t_MFPtr>
    inline static DelegateGenericClass* Convert(t_Ty* pthis, t_AnyFP function_to_bind, t_MFPtr& bound_func)
    {
        union {
            t_AnyFP func;
            struct
            {
                t_MFPtr funcaddress;
                int     delta;
            } s;
        } u;
        PHANTOM_STATIC_ASSERT(sizeof(function_to_bind) == sizeof(u.s), "cannot use union_cast");
        u.func = function_to_bind;
        bound_func = u.s.funcaddress;
        return reinterpret_cast<DelegateGenericClass*>(reinterpret_cast<char*>(pthis) + u.s.delta);
    }
};

struct MicrosoftVirtualMFP
{
    void (DelegateGenericClass::*codeptr)();
    int delta;
    int vtable_index;
};

struct delegate_generic_virtual_class : virtual public DelegateGenericClass
{
    typedef delegate_generic_virtual_class* (delegate_generic_virtual_class::*ProbePtrType)();
    delegate_generic_virtual_class* this_() { return this; }
};

template<>
struct MFHacker<delegate_generic_class_mfptr_size + 2 * sizeof(int)>
{
    template<class t_Ty, class t_AnyFP, class t_MFPtr>
    inline static DelegateGenericClass* Convert(t_Ty* pthis, t_AnyFP function_to_bind, t_MFPtr& bound_func)
    {
        union {
            t_AnyFP func;
            DelegateGenericClass* (t_Ty::*hacked_mfp)();
            MicrosoftVirtualMFP s;
        } u;
        u.func = function_to_bind;
        bound_func = reinterpret_cast<t_MFPtr>(u.s.codeptr);
        union {
            delegate_generic_virtual_class::ProbePtrType virtfunc;
            MicrosoftVirtualMFP                          s;
        } u2;
        PHANTOM_STATIC_ASSERT(sizeof(function_to_bind) == sizeof(u.s) &&
                              sizeof(function_to_bind) == sizeof(u.hacked_mfp) && sizeof(u2.virtfunc) == sizeof(u2.s),
                              "cannot use union_cast");
        u2.virtfunc = &delegate_generic_virtual_class::this_;
        u.s.codeptr = u2.s.codeptr;
        return (pthis->*u.hacked_mfp)();
    }
};

template<>
struct MFHacker<delegate_generic_class_mfptr_size + sizeof(ptrdiff_t) + 2 * sizeof(int)>
{
    template<class t_Ty, class t_AnyFP, class t_MFPtr>
    inline static DelegateGenericClass* Convert(t_Ty* pthis, t_AnyFP function_to_bind, t_MFPtr& bound_func)
    {
        union {
            t_AnyFP func;
            struct
            {
                t_MFPtr funcaddress;
                int     delta;
                int     vtordisp;
                int     vtable_index;
            } s;
        } u;
        PHANTOM_STATIC_ASSERT(sizeof(t_AnyFP) == sizeof(u.s), "cannot use union_cast");
        u.func = function_to_bind;
        bound_func = u.s.funcaddress;
        int virtual_delta = 0;
        if (u.s.vtable_index)
        {
            const int* vtable =
            *reinterpret_cast<const int* const*>(reinterpret_cast<const char*>(pthis) + u.s.vtordisp);

            virtual_delta =
            u.s.vtordisp + *reinterpret_cast<const int*>(reinterpret_cast<const char*>(vtable) + u.s.vtable_index);
        }
        return reinterpret_cast<DelegateGenericClass*>(reinterpret_cast<char*>(pthis) + u.s.delta + virtual_delta);
    };
};

#else // PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

template<>
struct MFHacker<delegate_generic_class_mfptr_size>
{
    template<class t_Ty, class t_AnyFP, class t_MFPtr>
    inline static DelegateGenericClass* Convert(t_Ty* pthis, t_AnyFP function_to_bind, t_MFPtr& bound_func)
    {
        bound_func = reinterpret_cast<t_MFPtr>(function_to_bind);
        return reinterpret_cast<DelegateGenericClass*>(pthis);
    }
};

#endif

} // namespace detail

namespace detail
{
template<class t_GenericMFP, class t_StaticFP>
class ClosurePtr : public OpaqueDelegate
{
public:
    template<class t_Ty, class t_AnyMFPtr>
    inline void bindMF(t_Ty* pthis, t_AnyMFPtr function_to_bind)
    {
        m_pThis = MFHacker<sizeof(function_to_bind)>::Convert(pthis, function_to_bind, m_pFunction);
    }
    template<class t_Ty, class t_AnyMFPtr>
    inline void bindMF_const(const t_Ty* pthis, t_AnyMFPtr function_to_bind)
    {
        m_pThis = MFHacker<sizeof(function_to_bind)>::Convert(const_cast<t_Ty*>(pthis), function_to_bind, m_pFunction);
    }
    inline DelegateGenericClass* getThis() const { return m_pThis; }
    inline t_GenericMFP          getMFPtr() const { return reinterpret_cast<t_GenericMFP>(m_pFunction); }

    template<class t_DerivedClass>
    inline void copyFrom(t_DerivedClass*, const OpaqueDelegate& right)
    {
        setOpaqueFrom(right);
    }
    template<class t_DerivedClass, class t_ParentInvokerSig>
    inline void bindF(t_DerivedClass* pParent, t_ParentInvokerSig static_function_invoker, t_StaticFP function_to_bind)
    {
        if (function_to_bind == 0)
        { // cope with assignment to 0
            m_pFunction = 0;
        }
        else
        {
            bindMF(pParent, static_function_invoker);
        }

        PHANTOM_STATIC_ASSERT(sizeof(DelegateGenericClass*) == sizeof(function_to_bind),
                              "size differs between Data pointers and function pointers, cannot use delegates");
        m_pThis = ::phantom::union_cast<DelegateGenericClass*>(function_to_bind);
    }
    inline t_StaticFP getStaticFP() const
    {
        PHANTOM_STATIC_ASSERT(sizeof(t_StaticFP) == sizeof(this),
                              "size differs between Data pointers and function pointers, cannot use delegates");
        return ::phantom::union_cast<t_StaticFP>(this);
    }

    inline bool contains(t_StaticFP funcptr)
    {
        if (funcptr == 0)
            return empty();
        else
            return funcptr == reinterpret_cast<t_StaticFP>(getStaticFP());
    }
};

} // namespace detail

template<class S>
class Delegate
#if defined(__HAUNT__)
{
}
#endif
;

template<class R, class... Params>
class Delegate<R(Params...)>
{
private:
    typedef R (*FuncPtrT)(Params...);
    typedef R (detail::DelegateGenericClass::*member_function_pointer_t)(Params...);
    typedef detail::ClosurePtr<member_function_pointer_t, FuncPtrT> ClosurePtrType;
    ClosurePtrType                                                  m_Closure;
    typedef Delegate<R(Params...)>                                  SelfType;

public:
    Delegate() { clear(); }

    Delegate(const SelfType& other) { m_Closure.copyFrom(this, other.m_Closure); }

    Delegate(SelfType&& a_Temp) : Delegate((const SelfType&)a_Temp) {}

    Delegate(FuncPtrT function_to_bind) { bind(function_to_bind); }

    Delegate(OpaqueDelegate const& _od) { m_Closure.copyFrom(this, _od); }

    template<
    class T,
    class = std::enable_if_t<!std::is_lvalue_reference<T>::value && std::is_convertible<T, FuncPtrT>::value, void>>
    Delegate(T&& a_Functor) : Delegate(static_cast<FuncPtrT>(a_Functor))
    {
    }

    SelfType& operator=(const SelfType& other)
    {
        m_Closure.copyFrom(this, other.m_Closure);
        return *this;
    }

    SelfType& operator=(const OpaqueDelegate& _od)
    {
        m_Closure.copyFrom(this, _od);
        return *this;
    }

    SelfType& operator=(SelfType&& other)
    {
        m_Closure.copyFrom(this, other.m_Closure);
        return *this;
    }

    SelfType& operator=(R (*function_to_bind)(Params...))
    {
        bind(function_to_bind);
        return *this;
    }
    template<
    class T,
    class = std::enable_if_t<!std::is_lvalue_reference<T>::value && std::is_convertible<T, FuncPtrT>::value, void>>
    SelfType& operator=(T&& a_Functor)
    {
        return operator=(static_cast<FuncPtrT>(a_Functor));
    }

    bool operator==(const SelfType& other) const { return m_Closure == other.m_Closure; }
    bool operator!=(const SelfType& other) const { return m_Closure != other.m_Closure; }
    bool operator<(const SelfType& other) const { return m_Closure < other.m_Closure; }
    bool operator>(const SelfType& other) const { return other.m_Closure < m_Closure; }

    template<class t_Ty, class Y>
    Delegate(Y* pthis, R (t_Ty::*function_to_bind)(Params...))
    {
        m_Closure.bindMF(detail::implicit_cast<t_Ty*>(pthis), function_to_bind);
    }

    template<class t_Ty, class Y>
    Delegate(const Y* pthis, R (t_Ty::*function_to_bind)(Params...) const)
    {
        m_Closure.bindMF_const(detail::implicit_cast<const t_Ty*>(pthis), function_to_bind);
    }

    template<class Y, class F, class = std::enable_if_t<std::is_convertible<F, R (*)(Y*, Params...)>::value>>
    Delegate(Y* pthis, F&& function_to_bind)
    {
        m_Closure.bindMF(pthis, static_cast<R (*)(Y*, Params...)>(function_to_bind));
    }

    template<class t_Ty, class Y>
    inline void bind(Y* pthis, R (t_Ty::*function_to_bind)(Params...))
    {
        m_Closure.bindMF(detail::implicit_cast<t_Ty*>(pthis), function_to_bind);
    }
    template<class t_Ty, class Y>
    inline void bind(const Y* pthis, R (t_Ty::*function_to_bind)(Params...) const)
    {
        m_Closure.bindMF_const(detail::implicit_cast<const t_Ty*>(pthis), function_to_bind);
    }

    inline void bind(R (*function_to_bind)(Params...))
    {
        m_Closure.bindF(this, &SelfType::callStaticF, function_to_bind);
    }

    R operator()(Params... ps) const { return (m_Closure.getThis()->*(m_Closure.getMFPtr()))(((Params)ps)...); }

    FunctorID getID() const { return getOpaque().getID(); }

public:
    // necessary to allow ==0 to work despite the safe_bool idiom
    inline bool           operator==(FuncPtrT funcptr) { return m_Closure.contains(funcptr); }
    inline bool           operator!=(FuncPtrT funcptr) { return !m_Closure.contains(funcptr); }
    inline bool           operator!() const { return !m_Closure; }
                          operator bool() const { return !!m_Closure; }
    inline bool           empty() const { return !m_Closure; }
    void                  clear() { m_Closure.clear(); }
    const OpaqueDelegate& getOpaque() const { return m_Closure; }
    void                  setOpaque(const OpaqueDelegate& any) { m_Closure.copyFrom(this, any); }

private:
    R callStaticF(Params... ps) const { return (*(m_Closure.getStaticFP()))(((Params)ps)...); }
};

template<class t_Ty, class Y, class R, class... Params>
Delegate<R(Params...)> bind(Y* x, R (t_Ty::*func)(Params...))
{
    return Delegate<R(Params...)>(x, func);
}
template<class t_Ty, class Y, class R, class... Params>
Delegate<R(Params...)> bind(Y* x, R (t_Ty::*func)(Params...) const)
{
    return Delegate<R(Params...)>(x, func);
}
template<class t_Ty, class Y, class R, class... Params>
Delegate<R(Params...)> bind(Y* x, R (*func)(t_Ty*, Params...))
{
    return Delegate<R(Params...)>(x, func);
}
template<class t_Ty, class Y, class R, class... Params>
Delegate<R(Params...)> bind(Y const* x, R (*func)(t_Ty const*, Params...))
{
    return Delegate<R(Params...)>(x, func);
}
} // namespace phantom
