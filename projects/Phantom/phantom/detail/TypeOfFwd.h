// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "macros.h"

#include <haunt>

HAUNT_STOP;

#define PHANTOM_TYPEOF(...) (phantom::TypeOf<__VA_ARGS__>(_PHNTM_TypeOfVal(__VA_ARGS__)))
#define PHANTOM_STATIC_TYPEOF(...) (phantom::StaticTypeOf<__VA_ARGS__>(_PHNTM_TypeOfVal(__VA_ARGS__)))

template<bool>
struct __assertion_failure_is_class;
template<>
struct __assertion_failure_is_class<true>
{
};
template<bool>
struct __assertion_failure_is_enum;
template<>
struct __assertion_failure_is_enum<true>
{
};
template<bool>
struct __assertion_failure_is_union;
template<>
struct __assertion_failure_is_union<true>
{
};

#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL

#    define _PHNTM_TYPEOF_CHECK(meta, Meta, ...)                                                                       \
        ((::phantom::lang::Meta*)((void)__assertion_failure_##is_##meta<std::is_##meta<__VA_ARGS__>::value>(),         \
                                  (void)#__VA_ARGS__ " is not a '" #meta "'", PHANTOM_TYPEOF(__VA_ARGS__)))

#else

#    define _PHNTM_TYPEOF_CHECK(meta, Meta, ...) ((::phantom::lang::Meta*)(PHANTOM_TYPEOF(__VA_ARGS__)))

#endif

namespace phantom
{
namespace lang
{
template<class T>
struct TypeOfVal
{
    static auto val() { return static_cast<T**>(nullptr); }
};
template<class T>
struct TypeOfVal<T&>
{
    static auto& val() { return *static_cast<T***>(nullptr); }
};
template<class T>
struct TypeOfVal<T&&>
{
    static auto&& val() { return std::move(*static_cast<T***>(nullptr)); }
};
#define _PHNTM_TypeOfVal(...) phantom::lang::TypeOfVal<__VA_ARGS__>::val()

template<class>
struct TypeOfUndefined;
} // namespace lang

template<class T>
struct TypeOfIndirect
{
    TypeOfIndirect(T**) {}
};
template<class T>
struct TypeOfIndirect<T&>
{
    TypeOfIndirect(T**&) {}
};
template<class T>
struct TypeOfIndirect<T&&>
{
    TypeOfIndirect(T**&&) {}
};

template<class T, class Ret = decltype(_PHNTM_TypeOf(_PHNTM_TypeOfVal(T)))>
PHANTOM_FORCEINLINE auto TypeOf(T** t)
{
    return _PHNTM_TypeOf(t);
}

template<class T>
auto TypeOf(TypeOfIndirect<T>)
{
    return lang::TypeOfUndefined<T>::object();
}

namespace _TypeOf
{
template<class T>
struct StaticTypeOf
{
    static auto Get() { return _PHNTM_TypeOf(_PHNTM_TypeOfVal(T)); }
};
} // namespace _TypeOf

template<class T>
auto StaticTypeOf()
{
    return _TypeOf::StaticTypeOf<T>::Get();
}
} // namespace phantom
