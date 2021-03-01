// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

namespace phantom
{
template<typename T>
struct RemoveConstReference
{
    typedef T type;
};
template<typename T>
struct RemoveConstReference<const T&>
{
    typedef T type;
};
} // namespace phantom
