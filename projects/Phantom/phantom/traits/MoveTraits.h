// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "ImplicitConv.h"
#include "ModifiersAndFlags.h"

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<class T, class = decltype(new T(std::declval<T&&>()))>
std::true_type MoveConstructibleH(T&&);

template<class T>
std::false_type MoveConstructibleH(ImplicitConv<T&&>);

template<class T, class = decltype(std::declval<T&>() = std::declval<T&&>())>
std::true_type MoveAssignableH(T&& a);

template<class T>
std::false_type MoveAssignableH(ImplicitConv<T&&> a);

template<class T>
struct IsMoveConstructible : decltype(MoveConstructibleH<T>(std::declval<T>()))
{
};

template<class T, size_t S>
struct IsMoveConstructible<T[S]> : IsMoveConstructible<T>
{
};

template<class T>
struct IsMoveAssignable : decltype(MoveAssignableH<T>(std::declval<T>()))
{
};

template<class T, size_t S>
struct IsMoveAssignable<T[S]> : IsMoveAssignable<T>
{
};

#pragma warning(disable : 4348)

template<class T,
         T& (T::*)(T&&) = &T::operator=> std::true_type MoveAssignmentOperatorDeclared(T&& a);

template<class T>
std::false_type MoveAssignmentOperatorDeclared(ImplicitConv<T&&> a);

template<typename T>
struct HasMoveAssignmentOperatorDeclared
    : decltype(MoveAssignmentOperatorDeclared<T>(std::declval<T>()))
{
};

#pragma warning(default : 4348)

namespace detail
{
template<typename t_Ty, bool t_Assignable>
struct HasMoveAssignmentOperatorH : HasMoveAssignmentOperatorDeclared<t_Ty>
{
};
template<typename t_Ty>
struct HasMoveAssignmentOperatorH<t_Ty, false> : std::false_type
{
};
} // namespace detail

template<typename t_Ty>
struct HasMoveAssignmentOperator
    : public phantom::detail::HasMoveAssignmentOperatorH<t_Ty, IsMoveAssignable<t_Ty>::value>
{
};

template<typename t_Ty>
struct HasMoveDisabled
{
    static const bool value =
    (phantom::MetaFlags<t_Ty>::value & PHANTOM_R_FLAG_NO_MOVE) == PHANTOM_R_FLAG_NO_MOVE;
};

namespace detail
{
template<typename t_Ty>
using _IsTriviallyMoveConstructible = std::is_trivially_copy_constructible<t_Ty>;
template<typename t_Ty>
using _IsTriviallyMoveAssignable = std::is_trivially_copy_assignable<t_Ty>;

template<typename t_Ty, bool t_no_copy>
struct IsMoveableH : public std::false_type
{
};

template<typename t_Ty>
struct IsMoveableH<t_Ty, false>
{
    static const bool                value =
    IsMoveConstructible<t_Ty>::value AND IsMoveAssignable<t_Ty>::value AND NOT(
    std::is_abstract<t_Ty>::value);
};

template<typename t_Ty, bool t_no_copy>
struct IsTriviallyMoveConstructibleH : public std::false_type
{
};

template<typename t_Ty>
struct IsTriviallyMoveConstructibleH<t_Ty, false> : public _IsTriviallyMoveConstructible<t_Ty>
{
};

template<typename t_Ty, bool t_no_copy>
struct IsTriviallyMoveAssignableH : public std::false_type
{
};

template<typename t_Ty>
struct IsTriviallyMoveAssignableH<t_Ty, false> : public _IsTriviallyMoveAssignable<t_Ty>
{
};
} // namespace detail

template<typename t_Ty>
struct IsMoveable : public phantom::detail::IsMoveableH<t_Ty, HasMoveDisabled<t_Ty>::value>
{
};

template<typename t_Ty>
struct IsMoveConstructibleAndNotDisabled : public phantom::IsMoveConstructible<t_Ty>
{
};

template<typename t_Ty>
struct IsMoveAssignableAndNotDisabled : public phantom::IsMoveAssignable<t_Ty>
{
};

template<typename t_Ty>
struct IsTriviallyMoveConstructibleAndNotDisabled
    : public phantom::detail::IsTriviallyMoveConstructibleH<t_Ty, HasMoveDisabled<t_Ty>::value>
{
};

template<typename t_Ty>
struct IsTriviallyMoveAssignableAndNotDisabled
    : public phantom::detail::IsTriviallyMoveAssignableH<t_Ty, HasMoveDisabled<t_Ty>::value>
{
};

} // namespace phantom

#define PHANTOM_DISABLE_MOVE_CONSTRUCTION(T) PHANTOM_DISABLE_TRAIT(IsMoveConstructible, T)
#define PHANTOM_DISABLE_MOVE_CONSTRUCTION_T(Types, Params, T)                                      \
    PHANTOM_DISABLE_TRAIT(IsMoveConstructible, Types, Params, T)
#define PHANTOM_DISABLE_MOVE_ASSIGNMENT(T) PHANTOM_DISABLE_TRAIT(IsMoveAssignable, T)
#define PHANTOM_DISABLE_MOVE_ASSIGNMENT_T(Types, Params, T)                                        \
    PHANTOM_DISABLE_TRAIT(IsMoveAssignable, Types, Params, T)
#define PHANTOM_DISABLE_MOVE(T)                                                                    \
    PHANTOM_DISABLE_MOVE_CONSTRUCTION(T) PHANTOM_DISABLE_MOVE_ASSIGNMENT(T)
#define PHANTOM_DISABLE_MOVE_T(Types, Params, T)                                                   \
    PHANTOM_DISABLE_MOVE_CONSTRUCTION_T(Types, Params, T)                                          \
    PHANTOM_DISABLE_MOVE_ASSIGNMENT_T(Types, Params, T)

/// @endcond
