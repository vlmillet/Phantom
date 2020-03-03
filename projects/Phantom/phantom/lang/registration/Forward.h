// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include <phantom/lang/TypeOf.h>

template<class T>
T* _PHNTM_RemoveTemplateForwardADL(T**);

namespace phantom
{
namespace lang
{
template<class T, bool TIsClassAndIsDefined>
struct ForwardH : T
{
};

template<class T>
struct ForwardH<T, false>
{
};

template<class T>
struct Forward : ForwardH<T, std::is_class<T>::value && phantom::IsTypeDefined<T>::value>
{
}; // we inherits so that anything that can be applied to T can be in Forward<T>

template<class T>
struct RemoveForwardCaller
{
    template<class TT, class S = decltype(_PHNTM_RemoveTemplateForwardADL((TT**)nullptr))>
    static S  Call(TT** t);
    static T* Call(void*);
};

template<class T>
struct RemoveForwardTemplate
{
    using type = std::remove_pointer_t<decltype(RemoveForwardCaller<T>::Call((T**)nullptr))>;
};

template<class T>
using RemoveForwardTemplateT = PHANTOM_TYPENAME RemoveForwardTemplate<T>::type;

template<class T>
struct RemoveForward : RemoveForwardTemplate<T>
{
};

template<class T>
using RemoveForwardT = PHANTOM_TYPENAME RemoveForward<T>::type;

template<class T>
struct RemoveForward<Forward<T>>
{
    using type = RemoveForwardT<T>;
};

template<class T>
struct IsForward : std::integral_constant<bool, !std::is_same<T, RemoveForwardT<T>>::value>
{
};

#define _PHNTM_DEF_REMOVE_FWD_EXT(...)                                                                                 \
    template<class T>                                                                                                  \
    struct RemoveForward<T __VA_ARGS__>                                                                                \
    {                                                                                                                  \
        using type = RemoveForwardT<T> __VA_ARGS__;                                                                    \
    }

_PHNTM_DEF_REMOVE_FWD_EXT(&);
_PHNTM_DEF_REMOVE_FWD_EXT(&&);
_PHNTM_DEF_REMOVE_FWD_EXT(const);
_PHNTM_DEF_REMOVE_FWD_EXT(volatile);
_PHNTM_DEF_REMOVE_FWD_EXT(const volatile);
_PHNTM_DEF_REMOVE_FWD_EXT(*);

template<class T, size_t S>
struct RemoveForward<T[S]>
{
    using type = RemoveForwardT<T>[S];
};
template<class T, size_t S>
struct RemoveForward<T const[S]>
{
    using type = RemoveForwardT<T const>[S];
};
template<class T, size_t S>
struct RemoveForward<T const volatile[S]>
{
    using type = RemoveForwardT<T const volatile>[S];
};
template<class T, size_t S>
struct RemoveForward<T volatile[S]>
{
    using type = RemoveForwardT<T volatile>[S];
};

#define _PHNTM_DEF_REMOVE_FWD_SIGN(...)                                                                                \
    template<class R, class... Args>                                                                                   \
    struct RemoveForward<R(Args...) __VA_ARGS__>                                                                       \
    {                                                                                                                  \
        using type = RemoveForwardT<R>(RemoveForwardT<Args>...) __VA_ARGS__;                                           \
    };                                                                                                                 \
    template<class T, class R, class... Args>                                                                          \
    struct RemoveForward<R (T::*)(Args...) __VA_ARGS__>                                                                \
    {                                                                                                                  \
        using type = RemoveForwardT<R> (T::*)(RemoveForwardT<Args>...) __VA_ARGS__;                                    \
    };                                                                                                                 \
    template<class T, class R, class... Args>                                                                          \
    struct RemoveForward<R (T::*)(Args..., ...) __VA_ARGS__>                                                           \
    {                                                                                                                  \
        using type = RemoveForwardT<R> (T::*)(RemoveForwardT<Args>..., ...) __VA_ARGS__;                               \
    }

_PHNTM_DEF_REMOVE_FWD_SIGN();
_PHNTM_DEF_REMOVE_FWD_SIGN(const);
_PHNTM_DEF_REMOVE_FWD_SIGN(const volatile);
_PHNTM_DEF_REMOVE_FWD_SIGN(volatile);
_PHNTM_DEF_REMOVE_FWD_SIGN(&);
_PHNTM_DEF_REMOVE_FWD_SIGN(const&);
_PHNTM_DEF_REMOVE_FWD_SIGN(const volatile&);
_PHNTM_DEF_REMOVE_FWD_SIGN(volatile&);
_PHNTM_DEF_REMOVE_FWD_SIGN(&&);
_PHNTM_DEF_REMOVE_FWD_SIGN(const&&);
_PHNTM_DEF_REMOVE_FWD_SIGN(const volatile&&);
_PHNTM_DEF_REMOVE_FWD_SIGN(volatile&&);

PHANTOM_STATIC_ASSERT((std::is_same<void(int) const, RemoveForwardT<void(Forward<int>) const>>::value));
PHANTOM_STATIC_ASSERT(
(std::is_same<void(int const&) const, RemoveForwardT<void(Forward<Forward<int> const&>) const>>::value));
PHANTOM_STATIC_ASSERT(
(std::is_same<void(int const&) const, RemoveForwardT<Forward<void>(Forward<Forward<int const>&>) const>>::value));
class IsForwardTest;
PHANTOM_STATIC_ASSERT(IsForward<const Forward<IsForwardTest>&>::value);

} // namespace lang
namespace lang
{
template<typename t_Ty>
struct TypeOfUndefined<Forward<t_Ty>> : TypeOfUndefined<t_Ty>
{
};
} // namespace lang

namespace _TypeOf
{
template<class T>
struct StaticTypeOf<lang::Forward<T>>
{
    static auto Get()
    {
        return lang::TypeOfUndefined<T>::object();
    }
};
} // namespace _TypeOf

} // namespace phantom

#define PHANTOM_FORWARD(Type) using Type = phantom::lang::Forward<Type>
