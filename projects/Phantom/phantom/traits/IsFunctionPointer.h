// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/traits/IsDataPointer.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct IsFunctionPointer
{
    static const bool value = std::is_pointer<t_Ty>::value && !IsDataPointer<t_Ty>::value;
};
} // namespace phantom

/// @endcond
