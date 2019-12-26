// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
