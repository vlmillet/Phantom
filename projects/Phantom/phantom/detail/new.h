// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "newImpl.h"

#include <phantom/lang/Class.h>
#include <phantom/lang/TypeOf.h>
#include <phantom/traits/ImplicitConv.h>

namespace phantom
{
namespace detail
{
PHANTOM_EXPORT_PHANTOM bool installed();

template<bool StaticChecks, class t_Ty>
void PlacementDeleteHH<StaticChecks, t_Ty, true>::apply(t_Ty* a_pInstance)
{
    auto pClass = a_pInstance->rtti.metaClass;
    PHANTOM_ASSERT(a_pInstance == a_pInstance->rtti.instance);
    pClass->unregisterInstance(a_pInstance);
    Constructor<t_Ty>::destroy(a_pInstance);
}

} // namespace detail
} // namespace phantom

#define PHANTOM_NEW(...) phantom::detail::NewH<__VA_ARGS__>() * new (phantom::allocate(sizeof(__VA_ARGS__), PHANTOM_ALIGNOF(__VA_ARGS__))) __VA_ARGS__

#define PHANTOM_DELETE(...) phantom::detail::DeleteH<true, __VA_ARGS__>() *

#define PHANTOM_DELETE_IGNORE_CHECKS(...) ::phantom::detail::DeleteH<false, __VA_ARGS__>()*

#define PHANTOM_DELETE_DYN ::phantom::detail::DeleteDynH()*

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_STACK_NEW(...)                                                                                     \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(PHANTOM_STACK_NEW_, PHANTOM_PP_ARGCOUNT_1_2_X(__VA_ARGS__)), (__VA_ARGS__))
#else
#    define PHANTOM_STACK_NEW(...)                                                                                     \
        PHANTOM_PP_CAT(PHANTOM_STACK_NEW_, PHANTOM_PP_ARGCOUNT_1_2_X(__VA_ARGS__))(__VA_ARGS__)
#endif

#define PHANTOM_STACK_NEW_1(var) ::phantom::detail::StackNewH<decltype(var)> PHANTOM_PP_CAT(_PHNTM_stacked, var)(&var)
#define PHANTOM_STACK_NEW_2(type, var)                                                                                 \
    type                               var;                                                                            \
    ::phantom::detail::StackNewH<type> PHANTOM_PP_CAT(_PHNTM_stacked_, var)(                                           \
    &var) // to avoid the most vexing C++ parsing of type var();
#define PHANTOM_STACK_NEW_X(type, var, ...)                                                                            \
    type                               var(__VA_ARGS__);                                                               \
    ::phantom::detail::StackNewH<type> PHANTOM_PP_CAT(_PHNTM_stacked, var)(&var)

#define PHANTOM_DEFERRED_NEW_EX(...) PHANTOM_NEW(__VA_ARGS__)

#define PHANTOM_DEFERRED_DELETE_EX(...) PHANTOM_DELETE(__VA_ARGS__)
