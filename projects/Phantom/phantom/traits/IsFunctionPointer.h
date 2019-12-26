// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/traits/IsDataPointer.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct IsFunctionPointer
{
    static const bool value = std::is_pointer<t_Ty>::value AND !IsDataPointer<t_Ty>::value;
};
} // namespace phantom

/// @endcond
