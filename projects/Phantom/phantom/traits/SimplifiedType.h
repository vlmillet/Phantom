// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/// @cond ADVANCED

namespace phantom
{
template<typename T>
struct SimplifiedType
{
    typedef T type;
};

template<typename T>
struct SimplifiedType<T*>
{
    typedef void* type;
};

template<typename T>
struct SimplifiedType<T&>
{
    typedef int& type;
};

template<typename T>
struct SimplifiedType<T&&>
{
    typedef int& type;
};

template<typename T>
struct SimplifiedType<T const>
{
    typedef T type;
};

template<typename T>
struct SimplifiedType<T volatile>
{
    typedef T type;
};

template<typename T>
struct SimplifiedType<T const volatile>
{
    typedef T type;
};

template<typename T, size_t S>
struct SimplifiedType<T[S]>
{
    typedef void* type;
};

template<class R, class... Params>
struct SimplifiedType<R (*)(Params...)>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class R, class... Params>
struct SimplifiedType<R (*)(Params..., ...)>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type..., ...);
};

#if PHANTOM_HAS_STDCALL
template<class R, class... Params>
struct SimplifiedType<R(PHANTOM_CALLCONV(stdcall)*)(Params...)>
{
    typedef PHANTOM_TYPENAME SimplifiedType<R>::type(PHANTOM_CALLCONV(stdcall) * type)(
    PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};
#endif

#if PHANTOM_HAS_FASTCALL
template<class R, class... Params>
struct SimplifiedType<R(PHANTOM_CALLCONV(fastcall)*)(Params...)>
{
    typedef PHANTOM_TYPENAME SimplifiedType<R>::type(PHANTOM_CALLCONV(fastcall) * type)(
    PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};
#endif

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...)>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) const>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) volatile>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) const volatile>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...)&>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) const&>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) const volatile&>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) &&>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) const&&>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<class T, class R, class... Params>
struct SimplifiedType<R (T::*)(Params...) const volatile&&>
{
    typedef PHANTOM_TYPENAME
    SimplifiedType<R>::type (DummyClass::*type)(PHANTOM_TYPENAME SimplifiedType<Params>::type...);
};

template<typename T>
using SimplifiedTypeT = PHANTOM_TYPENAME SimplifiedType<T>::type;
} // namespace phantom

/// @endcond
