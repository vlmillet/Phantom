// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <cstdint>
#include <haunt>

HAUNT_STOP;

namespace phantom
{
template<class T>
struct Promote
{
    typedef T type;
};

template<>
struct Promote<bool>
{
    typedef int type;
};
template<>
struct Promote<char>
{
    typedef int type;
};
template<>
struct Promote<int8_t>
{
    typedef int type;
};
template<>
struct Promote<int16_t>
{
    typedef int type;
};
template<>
struct Promote<int32_t>
{
    typedef int32_t type;
};
template<>
struct Promote<int64_t>
{
    typedef int64_t type;
};
template<>
struct Promote<uint8_t>
{
    typedef int type;
};
template<>
struct Promote<uint16_t>
{
    typedef int type;
};
template<>
struct Promote<uint32_t>
{
    typedef uint32_t type;
};
template<>
struct Promote<uint64_t>
{
    typedef uint64_t type;
};
template<>
struct Promote<float>
{
    typedef double type;
};
template<>
struct Promote<double>
{
    typedef double type;
};
PHANTOM_IF_CHAR16_T(template<> struct Promote<char16_t> { typedef int type; };)
PHANTOM_IF_CHAR32_T(template<> struct Promote<char32_t> { typedef int type; };)
PHANTOM_IF_WCHAR_T(template<> struct Promote<wchar_t> { typedef int type; };)
} // namespace phantom
