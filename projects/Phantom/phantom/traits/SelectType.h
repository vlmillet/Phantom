// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

namespace phantom
{
template<bool C, typename T1, typename T2>
struct SelectType
{
    typedef T1 type;
};

template<typename T1, typename T2>
struct SelectType<false, T1, T2>
{
    typedef T2 type;
};

template<bool C, typename T1, typename T2>
using SelectTypeT = PHANTOM_TYPENAME SelectType<C, T1, T2>::type;
} // namespace phantom
