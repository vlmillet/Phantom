// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/traits/prerequisites.h>
#include <type_traits>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct SafeAlignOf : public std::alignment_of<t_Ty>
{
};

template<>
struct SafeAlignOf<void> : std::integral_constant<size_t, 0>
{
};
} // namespace phantom

/// @endcond
