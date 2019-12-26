// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <stdint.h>

/// @cond ADVANCED
namespace phantom
{
template<int size>
struct IntTypeBySize;

template<>
struct IntTypeBySize<1>
{
    typedef int8_t type;
};
template<>
struct IntTypeBySize<2>
{
    typedef int16_t type;
};
template<>
struct IntTypeBySize<4>
{
    typedef int32_t type;
};
template<>
struct IntTypeBySize<8>
{
    typedef int64_t type;
};
template<>
struct IntTypeBySize<16>
{
    struct type
    {
        int64_t hi;
        int64_t lo;
    };
};
} // namespace phantom
