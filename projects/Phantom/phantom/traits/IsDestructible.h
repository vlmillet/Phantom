// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
using IsDestructible = std::is_destructible<t_Ty>;

template<typename T>
struct EmptyDerivedDtor : public T
{
};

template<typename T, bool is_final>
struct GetEmptyDerivedDtor
{
    using type = T;
};

template<typename T>
struct GetEmptyDerivedDtor<T, false>
{
    using type = EmptyDerivedDtor<T>;
};

namespace detail
{
template<typename T, bool is_class>
struct IsProtectedDestructibleH
    : public std::integral_constant<bool,
                                    !std::is_destructible<T>::value && std::is_destructible<EmptyDerivedDtor<T>>::value>
{
};
template<typename T>
struct IsProtectedDestructibleH<T, false> : public std::false_type
{
};

template<typename T, bool is_class>
struct IsPublicOrProtectedDestructibleH
    : public std::is_destructible<typename GetEmptyDerivedDtor<T, std::is_final<T>::value>::type>
{
};
template<typename T>
struct IsPublicOrProtectedDestructibleH<T, false> : public std::is_destructible<T>
{
};

} // namespace detail

template<typename T>
struct IsProtectedDestructible : public detail::IsProtectedDestructibleH<T, std::is_class<T>::value>

{
};

template<typename T>
struct IsPublicOrProtectedDestructible : public detail::IsPublicOrProtectedDestructibleH<T, std::is_class<T>::value>
{
};

} // namespace phantom

/// @endcond
