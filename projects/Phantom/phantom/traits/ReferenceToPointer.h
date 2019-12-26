// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct ReferenceToPointer
{
    typedef t_Ty type;
};

template<typename t_Ty>
struct ReferenceToPointer<t_Ty&>
{
    typedef t_Ty* type;
};
} // namespace phantom

/// @endcond
