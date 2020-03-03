// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#define PHANTOM_TYPEOF(...) (phantom::TypeOf<__VA_ARGS__>(phantom::TypeOfTag<__VA_ARGS__>()))
#define PHANTOM_STATIC_TYPEOF(...)                                                                 \
    (phantom::StaticTypeOf<__VA_ARGS__>(phantom::TypeOfTag<__VA_ARGS__>()))

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

#    define _PHNTM_TYPEOF_CHECK(meta, Meta, ...)                                                   \
        ((::phantom::lang::Meta*)((void)__assertion_failure_##is_##meta<                     \
                                        std::is_##meta<__VA_ARGS__>::value>(),                     \
                                        (void)#__VA_ARGS__ " is not a '" #meta "'",                \
                                        PHANTOM_TYPEOF(__VA_ARGS__)))

#else

#    define _PHNTM_TYPEOF_CHECK(meta, Meta, ...)                                                   \
        ((::phantom::lang::Meta*)(PHANTOM_TYPEOF(__VA_ARGS__)))

#endif

namespace phantom
{
namespace lang
{
template<class>
struct TypeOfUndefined;
}

template<class T>
struct TypeOfTag
{
};

template<class T>
struct TypeOfIndirect
{
    TypeOfIndirect(TypeOfTag<T> )
    {
    }
};

template<class T, class Ret = decltype(_PHNTM_TypeOf(TypeOfTag<T>()))>
auto TypeOf(TypeOfTag<T> t)
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
    static auto Get()
    {
        return _PHNTM_TypeOf(TypeOfTag<T>());
    }
};
} // namespace _TypeOf

template<class T>
auto StaticTypeOf()
{
    return _TypeOf::StaticTypeOf<T>::Get();
}
} // namespace phantom
