// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
using HasVirtualDestructor = std::has_virtual_destructor<t_Ty>;
}

/// @endcond
