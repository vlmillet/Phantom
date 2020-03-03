// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

namespace phantom
{
template<typename t_Ty>
struct Dereferencer
{
    inline static t_Ty apply(t_Ty value)
    {
        return value;
    }
};

template<typename t_Ty>
struct Dereferencer<t_Ty&>
{
    inline static t_Ty& apply(t_Ty* value)
    {
        return *value;
    }
};
} // namespace phantom
