// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "HasMethod.h"

#define _PHNTM_DEFINE_HAS_METHOD_NO_BASE_L1(TraitName, name)                                       \
    _PHNTM_DEFINE_HAS_METHOD_NO_BASE_L0(TraitName, name)

#define _PHNTM_DEFINE_HAS_METHOD_NO_BASE_L0(TraitName, name)                                       \
    PHANTOM_DEFINE_HAS_METHOD_NO_BASE(TraitName, name)

// does not work with final classes
#define PHANTOM_DEFINE_HAS_METHOD_NO_BASE_ONLY(TraitName, name)                                    \
    template<typename T, class Signature>                                                          \
    struct TraitName;                                                                              \
                                                                                                   \
    template<typename T, class R, class... P>                                                      \
    struct TraitName<T, R(P...)>                                                                   \
    {                                                                                              \
        typedef char yes[1];                                                                       \
        typedef char no[2];                                                                        \
                                                                                                   \
        template<class U, R (U::*)(P...) = &U::name>                                               \
        struct MethodTraits;                                                                       \
                                                                                                   \
        template<typename U>                                                                       \
        static yes& _has(MethodTraits<U>*);                                                        \
                                                                                                   \
        template<typename>                                                                         \
        static no& _has(...);                                                                      \
                                                                                                   \
        static const bool value = sizeof(_has<T>(nullptr)) == sizeof(yes);                         \
    };

#define PHANTOM_DEFINE_HAS_METHOD_NO_BASE(TraitName, name)                                         \
    PHANTOM_DEFINE_HAS_METHOD(TraitName, name)                                                     \
    PHANTOM_DEFINE_HAS_METHOD_NO_BASE_ONLY(TraitName##_NoBase, name)
