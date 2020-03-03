// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

namespace phantom
{
template<class T, class R>
struct FunctionTypeToMethodPointerType
{
    using type = R (T::*)();
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...)>
{
    using type = R (T::*)(P...);
};

template<class T, class R>
struct FunctionTypeToMethodPointerType<T, R(void)>
{
    using type = R (T::*)();
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) const>
{
    using type = R (T::*)(P...) const;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) const volatile>
{
    using type = R (T::*)(P...) const volatile;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) volatile>
{
    using type = R (T::*)(P...) volatile;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...)&>
{
    using type = R (T::*)(P...) &;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) const&>
{
    using type = R (T::*)(P...) const&;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) const volatile&>
{
    using type = R (T::*)(P...) const volatile&;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) volatile&>
{
    using type = R (T::*)(P...) volatile&;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) &&>
{
    using type = R (T::*)(P...) &&;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) const&&>
{
    using type = R (T::*)(P...) const&&;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) const volatile&&>
{
    using type = R (T::*)(P...) const volatile&&;
};

template<class T, class R, class... P>
struct FunctionTypeToMethodPointerType<T, R(P...) volatile&&>
{
    using type = R (T::*)(P...) volatile&&;
};
} // namespace phantom
