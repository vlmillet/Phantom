// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "newImpl.h"

#include <phantom/mostDerivedOf.h>
#include <phantom/reflection/Class.h>
#include <phantom/reflection/TypeOf.h>

namespace phantom
{
namespace detail
{
PHANTOM_EXPORT_PHANTOM bool installed();

template<bool StaticChecks, class t_Ty>
void PlacementDeleteHH<StaticChecks, t_Ty, true>::apply(t_Ty* a_pInstance)
{
    auto pClass = a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass;
    auto pAddr = a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance;
    PHANTOM_ASSERT(mostDerivedOf(a_pInstance) == Object(pAddr, pClass));
    PHANTOM_ASSERT(a_pInstance == pAddr);
    pClass->unregisterInstance(pAddr);
    Constructor<t_Ty>::destroy(a_pInstance);
}

template<class t_Ty>
void DeleteDynHH<t_Ty, true, false>::apply(t_Ty* a_pInstance)
{
    PHANTOM_ASSERT(phantom::detail::installed());
    auto pClass = a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass;
    auto pAddr = a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance;
    pClass->unregisterInstance(pAddr);
    pClass->deleteInstance(pAddr);
}
} // namespace detail
} // namespace phantom

#define PHANTOM_NEW(...)                                                                           \
    ::phantom::detail::NewH<__VA_ARGS__>() * new (PHANTOM_ALLOCATE(__VA_ARGS__)) __VA_ARGS__

#define PHANTOM_DELETE(...) ::phantom::detail::DeleteH<true, __VA_ARGS__>()*

#define PHANTOM_DELETE_IGNORE_CHECKS(...) ::phantom::detail::DeleteH<false, __VA_ARGS__>()*

#define PHANTOM_NEW_N(N, ...)                                                                      \
    (::phantom::detail::NewNH<__VA_ARGS__>(N PHANTOM_MEMORY_STAT_APPEND_VALUES) *                  \
     ((__VA_ARGS__*)PHANTOM_ALLOCATE_N(N, __VA_ARGS__)))

#define PHANTOM_DELETE_N(N, ...) ::phantom::detail::DeleteNH<__VA_ARGS__>(N)*

#define PHANTOM_DELETE_DYN ::phantom::detail::DeleteDynH()*

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_STACK_NEW(...)                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(PHANTOM_STACK_NEW_, PHANTOM_PP_ARGCOUNT_1_2_X(__VA_ARGS__)), \
                       (__VA_ARGS__))
#else
#    define PHANTOM_STACK_NEW(...)                                                                 \
        PHANTOM_PP_CAT(PHANTOM_STACK_NEW_, PHANTOM_PP_ARGCOUNT_1_2_X(__VA_ARGS__))(__VA_ARGS__)
#endif

#define PHANTOM_STACK_NEW_1(var)                                                                   \
    ::phantom::detail::StackNewH<decltype(var)> PHANTOM_PP_CAT(_PHNTM_stacked, var)(&var)
#define PHANTOM_STACK_NEW_2(type, var)                                                             \
    type                               var;                                                        \
    ::phantom::detail::StackNewH<type> PHANTOM_PP_CAT(_PHNTM_stacked_, var)(                       \
    &var) // to avoid the most vexing C++ parsing of type var();
#define PHANTOM_STACK_NEW_X(type, var, ...)                                                        \
    type                               var(__VA_ARGS__);                                           \
    ::phantom::detail::StackNewH<type> PHANTOM_PP_CAT(_PHNTM_stacked, var)(&var)

#define PHANTOM_DEFERRED_NEW_EX(...) PHANTOM_NEW(__VA_ARGS__)

#define PHANTOM_DEFERRED_DELETE_EX(...) PHANTOM_DELETE(__VA_ARGS__)
