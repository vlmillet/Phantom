// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/traits/prerequisites.h>
#include <stddef.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct IsNullptrT : public std::false_type
{
};

template<>
struct IsNullptrT<decltype(nullptr)> : public std::true_type
{
};
} // namespace phantom

/// @endcond
