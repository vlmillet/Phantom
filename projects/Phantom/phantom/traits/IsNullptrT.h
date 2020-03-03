// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
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
