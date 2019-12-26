#pragma once

#include "SmallVector.h"
#include "Variant.h"

namespace phantom
{
template<size_t StaticCapacity>
using SmallCollection = SmallVector<Variant, StaticCapacity>;

using Collection = SmallCollection<4>;
} // namespace phantom
