// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/macros.h>

/// @cond ADVANCED

#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif

namespace phantom
{
namespace detail
{
template<typename t_Ty, bool t_Final>
struct CtorH
{
    PHANTOM_FORCEINLINE static void apply(void* a_pInstance)
    {
        new (a_pInstance) t_Ty();
    }
};
template<typename t_Ty>
struct CtorH<t_Ty, false> : public t_Ty
{
    PHANTOM_FORCEINLINE static void apply(void* a_pInstance)
    {
        new (a_pInstance) CtorH();
    }
};

template<typename t_Ty, bool t_Final>
struct DtorH
{
    PHANTOM_FORCEINLINE static void apply(void* a_pInstance)
    {
        reinterpret_cast<t_Ty*>(a_pInstance)->~t_Ty();
    }
};

template<typename t_Ty>
struct DtorH<t_Ty, false> : public t_Ty
{
    PHANTOM_FORCEINLINE static void apply(void* a_pInstance)
    {
        reinterpret_cast<DtorH<t_Ty, false>*>(a_pInstance)->~DtorH<t_Ty, false>();
    }
};
} // namespace detail

#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wabstract-final-class"
#endif

template<typename t_Ty>
struct Ctor final : public detail::CtorH<t_Ty, std::is_final<t_Ty>::value>
{
};

template<typename t_Ty>
struct Dtor final : public detail::DtorH<t_Ty, std::is_final<t_Ty>::value>
{
};

} // namespace phantom

#if defined(__clang__)
#    pragma clang diagnostic pop
#endif

#if defined(__clang__)
#    pragma clang diagnostic pop
#endif

/// @endcond
