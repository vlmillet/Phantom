// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "ImplicitConv.h"
#include "ModifiersAndFlags.h"

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<class T, class = decltype(new T(std::declval<T const&>()))>
std::true_type CopyConstructibleH(T&&);

template<class T>
std::false_type CopyConstructibleH(ImplicitConv<T&&>);

template<class T, class = decltype(std::declval<T&>() = std::declval<T const&>())>
std::true_type CopyAssignableH(T&& a);

template<class T>
std::false_type CopyAssignableH(ImplicitConv<T&&> a);

template<class T>
struct IsCopyConstructible : decltype(CopyConstructibleH<T>(std::declval<T>()))
{
};

template<class T, size_t S>
struct IsCopyConstructible<T[S]> : IsCopyConstructible<T>
{
};

template<class T>
struct IsCopyAssignable : decltype(CopyAssignableH<T>(std::declval<T>()))
{
};

template<class T, size_t S>
struct IsCopyAssignable<T[S]> : IsCopyAssignable<T>
{
};

#pragma warning(disable : 4348)

template<class T,
         T& (T::*)(T const&) = &T::operator=> std::true_type CopyAssignmentOperatorDeclared(T&& a);

template<class T>
std::false_type CopyAssignmentOperatorDeclared(ImplicitConv<T&&> a);

template<typename T>
struct HasCopyAssignmentOperatorDeclared
    : decltype(CopyAssignmentOperatorDeclared<T>(std::declval<T>()))
{
};

#pragma warning(default : 4348)

namespace detail
{
template<typename t_Ty, bool t_Assignable>
struct HasCopyAssignmentOperatorH : HasCopyAssignmentOperatorDeclared<t_Ty>
{
};
template<typename t_Ty>
struct HasCopyAssignmentOperatorH<t_Ty, false> : std::false_type
{
};
} // namespace detail

template<typename t_Ty>
struct HasCopyAssignmentOperator
    : public phantom::detail::HasCopyAssignmentOperatorH<t_Ty, IsCopyAssignable<t_Ty>::value>
{
};

template<typename t_Ty>
struct HasCopyDisabled
{
    static const bool value =
    (phantom::MetaFlags<t_Ty>::value & PHANTOM_R_FLAG_NO_COPY) == PHANTOM_R_FLAG_NO_COPY;
};

namespace detail
{
template<typename t_Ty>
using _IsTriviallyCopyConstructible = std::is_trivially_copy_constructible<t_Ty>;
template<typename t_Ty>
using _IsTriviallyCopyAssignable = std::is_trivially_copy_assignable<t_Ty>;

template<typename t_Ty, bool t_no_copy>
struct IsCopyableH : public std::false_type
{
};

template<typename t_Ty>
struct IsCopyableH<t_Ty, false>
{
    static const bool                value =
    IsCopyConstructible<t_Ty>::value AND IsCopyAssignable<t_Ty>::value AND NOT(
    std::is_abstract<t_Ty>::value);
};

template<typename t_Ty, bool t_no_copy>
struct IsTriviallyCopyConstructibleH : public std::false_type
{
};

template<typename t_Ty>
struct IsTriviallyCopyConstructibleH<t_Ty, false> : public _IsTriviallyCopyConstructible<t_Ty>
{
};

template<typename t_Ty, bool t_no_copy>
struct IsTriviallyCopyAssignableH : public std::false_type
{
};

template<typename t_Ty>
struct IsTriviallyCopyAssignableH<t_Ty, false> : public _IsTriviallyCopyAssignable<t_Ty>
{
};
} // namespace detail

template<typename t_Ty>
struct IsCopyable : public phantom::detail::IsCopyableH<t_Ty, HasCopyDisabled<t_Ty>::value>
{
};

template<typename t_Ty>
struct IsCopyConstructibleAndNotDisabled : public phantom::IsCopyConstructible<t_Ty>
{
};

template<typename t_Ty>
struct IsCopyAssignableAndNotDisabled : public phantom::IsCopyAssignable<t_Ty>
{
};

template<typename t_Ty>
struct IsTriviallyCopyConstructibleAndNotDisabled
    : public phantom::detail::IsTriviallyCopyConstructibleH<t_Ty, HasCopyDisabled<t_Ty>::value>
{
};

template<typename t_Ty>
struct IsTriviallyCopyAssignableAndNotDisabled
    : public phantom::detail::IsTriviallyCopyAssignableH<t_Ty, HasCopyDisabled<t_Ty>::value>
{
};
} // namespace phantom

#define PHANTOM_DISABLE_COPY_CONSTRUCTION(T) PHANTOM_DISABLE_TRAIT(IsCopyConstructible, T)
#define PHANTOM_DISABLE_COPY_CONSTRUCTION_T(Types, Params, T)                                      \
    PHANTOM_DISABLE_TRAIT(IsCopyConstructible, Types, Params, T)
#define PHANTOM_DISABLE_COPY_ASSIGNMENT(T) PHANTOM_DISABLE_TRAIT(IsCopyAssignable, T)
#define PHANTOM_DISABLE_COPY_ASSIGNMENT_T(Types, Params, T)                                        \
    PHANTOM_DISABLE_TRAIT(IsCopyAssignable, Types, Params, T)
#define PHANTOM_DISABLE_COPY(T)                                                                    \
    PHANTOM_DISABLE_COPY_CONSTRUCTION(T) PHANTOM_DISABLE_COPY_ASSIGNMENT(T)
#define PHANTOM_DISABLE_COPY_T(Types, Params, T)                                                   \
    PHANTOM_DISABLE_COPY_CONSTRUCTION_T(Types, Params, T)                                          \
    PHANTOM_DISABLE_COPY_ASSIGNMENT_T(Types, Params, T)

/// @endcond
