// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "remove_all_pointers.h"

#include <type_traits>

namespace phantom
{
template<typename T>
struct remove_everything
{
    typedef typename std::remove_all_extents<typename std::remove_cv<
    typename remove_all_pointers<typename std::remove_reference<T>::type>::type>::type>::type type;
};
} // namespace phantom
