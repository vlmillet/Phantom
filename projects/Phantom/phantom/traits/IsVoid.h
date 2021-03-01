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
template<class T>
struct IsVoid : public std::false_type
{
};

template<>
struct IsVoid<void> : public std::true_type
{
};
template<>
struct IsVoid<const void> : public std::true_type
{
};
template<>
struct IsVoid<const volatile void> : public std::true_type
{
};
template<>
struct IsVoid<volatile void> : public std::true_type
{
};

} // namespace phantom

/// @endcond
