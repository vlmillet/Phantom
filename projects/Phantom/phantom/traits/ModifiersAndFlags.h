#pragma once

#include <haunt>
#include <phantom/typedefs.h>
#include <type_traits>

namespace phantom
{
/// @cond ADVANCED
HAUNT_PAUSE;

template<int modifiers = 0, uint Flags = 0>
struct ModifiersAndFlags
    : public std::integral_constant<ulonglong, (ulonglong(modifiers) << 32) | ulonglong(Flags)>
{
};

template<typename t_Ty>
struct MetaModifiersAndFlags : public std::integral_constant<ulonglong, 0>
{
};

template<typename t_Ty>
struct MetaFlags
    : public std::integral_constant<uint, MetaModifiersAndFlags<t_Ty>::value & 0xffffffff>
{
};

template<typename t_Ty>
struct MetaModifiers
    : public std::integral_constant<int, (MetaModifiersAndFlags<t_Ty>::value >> 32) & 0xffffffff>
{
};

HAUNT_RESUME;
/// @endcond
} // namespace phantom
