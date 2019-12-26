// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#pragma once

#include <phantom/traits/IsVoid.h>
#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct SafeSizeOf : std::integral_constant<size_t, sizeof(t_Ty)>
{
};

template<>
struct SafeSizeOf<void> : std::integral_constant<size_t, 0>
{
};

} // namespace phantom

/// @endcond
