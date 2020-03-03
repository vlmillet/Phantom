// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "SmallString.h"

namespace phantom
{
PHANTOM_FORCEINLINE const char* cstr(const String& str)
{
    return str.c_str();
}
} // namespace phantom
