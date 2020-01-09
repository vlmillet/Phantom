// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/FunctionT.h>
#include <phantom/lang/MetaNewDelete.h>
#include <phantom/lang/MethodT.h>

#if PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_SIGNATURE
#    include <phantom/lang/SignatureH.h>
#    define TNMFP_signature_param_insert
#    define TNMFP_signature_arg_insert(modifiers, ...)                                                                 \
        SignatureH<t_ReturnType(__VA_ARGS__)>::CreateSignature((modifiers) & (PHANTOM_R_METHOD_QUAL_MASK)),
#else
#    define TNMFP_signature_param_insert StringView a_strSignature,
#    define TNMFP_signature_arg_insert(modifiers, ...) a_strSignature,
#endif

/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
namespace detail
{
template<typename T, class Sign>
struct MethodPointerSimplifier;

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...)>
{
    typedef R (T::*SourcePtr)(Ps...);
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
    /// assign operator= fallback
    template<typename TT>
    static SimplifiedTypeT<SourcePtr> Simplify(TT& (TT::*p)(const TT&))
    {
        SourcePtr ptr = &T::operator=;
        return (SimplifiedTypeT<SourcePtr>)ptr;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) const>
{
    typedef R (T::*SourcePtr)(Ps...) const;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) volatile>
{
    typedef R (T::*SourcePtr)(Ps...) volatile;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) const volatile>
{
    typedef R (T::*SourcePtr)(Ps...) const volatile;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) const volatile&>
{
    typedef R (T::*SourcePtr)(Ps...) const volatile&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) const&>
{
    typedef R (T::*SourcePtr)(Ps...) const&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) volatile&>
{
    typedef R (T::*SourcePtr)(Ps...) volatile&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...)&>
{
    typedef R (T::*SourcePtr)(Ps...) &;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) const volatile&&>
{
    typedef R (T::*SourcePtr)(Ps...) const volatile&&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) const&&>
{
    typedef R (T::*SourcePtr)(Ps...) const&&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) volatile&&>
{
    typedef R (T::*SourcePtr)(Ps...) volatile&&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

template<typename T, typename R, class... Ps>
struct MethodPointerSimplifier<T, R(Ps...) &&>
{
    typedef R (T::*SourcePtr)(Ps...) &&;
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};
} // namespace detail

} // namespace lang
} // namespace phantom

/// @endcond
