// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
