// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
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
