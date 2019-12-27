// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <phantom/detail/phantom.h>
#include <phantom/traits/indices.h>

/// @cond ADVANCED

namespace phantom
{
namespace detail
{
template<typename t_Class, typename t_Signature>
struct ConstructorCallerNonFinal;

template<typename t_Ty, class... v_Params>
struct ConstructorCallerNonFinal<t_Ty, void(v_Params...)> : public t_Ty
{
public:
    PHANTOM_FORCEINLINE ConstructorCallerNonFinal(v_Params... args) : t_Ty(std::forward<v_Params>(args)...)
    {
    }

    template<size_t... Is>
    PHANTOM_FORCEINLINE static void apply(void* a_pAddress, void** a_pParams, Indices<Is...>)
    {
        new (a_pAddress) ConstructorCallerNonFinal(
        ((v_Params)(*static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is])))...);
    }

    PHANTOM_FORCEINLINE static void apply(void* a_pAddress, void** a_pParams)
    {
        apply(a_pAddress, a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    void closure(v_Params... args)
    {
        new (this) ConstructorCallerNonFinal(std::forward<v_Params>(args)...);
    }
};

template<typename t_Class, typename t_Signature, bool t_is_abstract, bool t_is_final>
struct ConstructorCallerH;

// abstract
template<typename t_Class, typename t_Signature, bool t_is_final>
struct ConstructorCallerH<t_Class, t_Signature, true, t_is_final>
{
    PHANTOM_FORCEINLINE static void apply(void*, void**)
    {
        PHANTOM_ASSERT(false, "calling abstract constructor");
    }

    void closure()
    {
        PHANTOM_ASSERT(false, "calling abstract constructor");
    }
};

// final abstract
template<typename t_Class, class... v_Params>
struct ConstructorCallerH<t_Class, void(v_Params...), true, true>
{
    template<size_t... Is>
    static void apply(void* a_pAddress, void** a_pParams, Indices<Is...>)
    {
        new (a_pAddress)
        t_Class(((v_Params)(*static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is])))...);
    }

    static void apply(void* a_pAddress, void** a_pParams)
    {
        apply(a_pAddress, a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    void closure(v_Params... args)
    {
        new (this) t_Class(std::forward<v_Params>(args)...);
    }
};

// final non abstract
template<typename t_Class, class... v_Params>
struct ConstructorCallerH<t_Class, void(v_Params...), false, true>
{
    template<size_t... Is>
    static void apply(void* a_pAddress, void** a_pParams, Indices<Is...>)
    {
        new (a_pAddress)
        t_Class(((v_Params)(*static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is])))...);
    }

    static void apply(void* a_pAddress, void** a_pParams)
    {
        apply(a_pAddress, a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    void closure(v_Params... args)
    {
        new (this) t_Class(std::forward<v_Params>(args)...);
    }
};

// default

template<typename t_Class, typename t_Signature>
struct ConstructorCallerH<t_Class, t_Signature, false, false> : public ConstructorCallerNonFinal<t_Class, t_Signature>
{
};

} // namespace detail

template<typename t_Class, typename t_Signature>
struct ConstructorCaller
    : public detail::ConstructorCallerH<t_Class, t_Signature, std::is_abstract<t_Class>::value,
                                        !std::is_class<t_Class>::value || std::is_final<t_Class>::value>
{
};

} // namespace phantom

/// @endcond
