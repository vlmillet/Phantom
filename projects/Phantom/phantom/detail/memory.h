// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/alignof>
#include <phantom/detail/fwd.h>

HAUNT_STOP;

#include <limits>
#include <phantom/CustomAllocator.h>
#include <stdlib.h>

namespace phantom
{
namespace memory
{
PHANTOM_EXPORT_PHANTOM void* allocBytes(size_t size, size_t align = 1, const char* file = "", int line = 0);
PHANTOM_EXPORT_PHANTOM void  deallocBytes(void* mem);
PHANTOM_EXPORT_PHANTOM void* reallocBytes(void* mem, size_t size, size_t align = 1, const char* file = "",
                                          int line = 0);

} // namespace memory
} // namespace phantom
