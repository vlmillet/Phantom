#pragma once

// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include <type_traits>

namespace phantom
{
template<class T, class = void>
struct IsTypeDefined : std::false_type
{
};

template<class T>
struct IsTypeDefined<T, std::enable_if_t<(sizeof(T) > 0)> > : std::true_type
{
};
} // namespace phantom
