#pragma once

#include "SmallMap.h"
#include "SmallMultimap.h"
#include "Variant.h"

namespace phantom
{
template<size_t StaticCapacity>
using SmallDictionary = SmallMap<String, Variant, StaticCapacity>;
template<size_t StaticCapacity>
using SmallMultiDictionary = SmallMultimap<String, Variant, StaticCapacity>;

using Dictionary = SmallDictionary<4>;
using MultiDictionary = SmallMultiDictionary<4>;
} // namespace phantom
