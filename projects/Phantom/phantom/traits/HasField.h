// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "HasMember.h"

#include <type_traits>

#define PHANTOM_DEFINE_HAS_FIELD(name)                                                             \
    PHANTOM_DEFINE_HAS_MEMBER(name)                                                                \
    template<class T, class V, bool HasMember>                                                     \
    struct ____HasField_##name : std::false_type                                                   \
    {                                                                                              \
    };                                                                                             \
                                                                                                   \
    template<class T, class V>                                                                     \
    struct ____HasField_##name<T, V, true>                                                         \
    {                                                                                              \
        typedef char yes;                                                                          \
        typedef long no;                                                                           \
        template<class Owner>                                                                      \
        static yes        apply(V(Owner::*));                                                      \
        static no         apply(...);                                                              \
        static const bool value = std::is_same<yes, decltype(apply(&T::name))>::value;             \
    };                                                                                             \
    template<class T, class V>                                                                     \
    struct HasField_##name : ____HasField_##name<T, V, HasMember_##name<T>::value>                 \
    {                                                                                              \
    };

#define PHANTOM_DEFINE_HAS_FIELD_NO_BASE_ONLY(name)                                                \
    template<class T, class V, bool HasField>                                                      \
    struct ____HasFieldNoBase_##name : std::false_type                                             \
    {                                                                                              \
    };                                                                                             \
                                                                                                   \
    template<class T, class V>                                                                     \
    struct ____HasFieldNoBase_##name<T, V, true>                                                   \
    {                                                                                              \
        template<class Owner>                                                                      \
        static Owner*     apply(V(Owner::*));                                                      \
        static void*      apply(...);                                                              \
        static const bool value =                                                                  \
        std::is_same<T, typename std::remove_pointer<decltype(apply(&T::name))>::type>::value;     \
    };                                                                                             \
    template<class T, class V>                                                                     \
    struct HasFieldNoBase_##name : ____HasFieldNoBase_##name<T, V, HasField_##name<T, V>::value>   \
    {                                                                                              \
    };

#define PHANTOM_DEFINE_HAS_FIELD_NO_BASE(name)                                                     \
    PHANTOM_DEFINE_HAS_FIELD(name)                                                                 \
    PHANTOM_DEFINE_HAS_FIELD_NO_BASE_ONLY(name)

#define PHANTOM_DEFINE_HAS_FIELD_L1(name) PHANTOM_DEFINE_HAS_FIELD_L0(name)

#define PHANTOM_DEFINE_HAS_FIELD_L0(name) PHANTOM_DEFINE_HAS_FIELD(name)

#define PHANTOM_DEFINE_HAS_FIELD_NO_BASE_MACRO_PARAM(name) _PHNTM_DEFINE_HAS_FIELD_NO_BASE_L0(name)

#define _PHNTM_DEFINE_HAS_FIELD_NO_BASE_L0(name) PHANTOM_DEFINE_HAS_FIELD_NO_BASE(name)
