// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/typedefs.h>
#include <phantom/lang/TypeOf.h>
#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct HasReflection
    : std::integral_constant<bool, !std::is_same<decltype(TypeOf((t_Ty**)nullptr)), lang::Type*>::value>
{
};

template<typename t_Ty>
struct HasReflection<t_Ty*> : std::integral_constant<bool, HasReflection<t_Ty>::value>
{
};

template<typename t_Ty>
struct HasReflection<t_Ty&> : std::integral_constant<bool, HasReflection<t_Ty>::value>
{
};

template<typename t_Ty>
struct HasReflection<t_Ty const> : std::integral_constant<bool, HasReflection<t_Ty>::value>
{
};

template<typename t_Ty>
struct HasReflection<t_Ty volatile> : std::integral_constant<bool, HasReflection<t_Ty>::value>
{
};

template<typename t_Ty>
struct HasReflection<t_Ty const volatile> : std::integral_constant<bool, HasReflection<t_Ty>::value>
{
};

template<typename t_Ty, size_t t_Size>
struct HasReflection<t_Ty[t_Size]> : std::integral_constant<bool, HasReflection<t_Ty>::value>
{
};

template<typename t_Ty>
struct RegProxyOf
{
    typedef t_Ty type;
};

_PHNTM_TRTS_SPEC__FOR_FUNDAMENTALS(HasReflection, bool, true)
} // namespace phantom

/// @endcond
