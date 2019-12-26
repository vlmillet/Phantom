// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
