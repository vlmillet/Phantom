#pragma once

// clang-format off

#include <limits>
#include <phantom/source>
#include <phantom/class>
#include <phantom/static_method>
#include <phantom/typedef>

#if defined(_MSC_VER)
#    pragma warning(push, 0)
#elif defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wall"
#    pragma clang diagnostic ignored "-Wextra"
#endif

#undef min
#undef max

PHANTOM_SOURCE("std.limits");

namespace std
{
PHANTOM_CLASS_T((class), (T), numeric_limits)
{
PHANTOM_PUBLIC:
    PHANTOM_STATIC_METHOD(T, min, ());
    PHANTOM_STATIC_METHOD(T, max, ());
    PHANTOM_STATIC_METHOD(T, lowest, ());
    PHANTOM_STATIC_METHOD(T, epsilon, ());
    PHANTOM_STATIC_METHOD(T, round_error, ());
    PHANTOM_STATIC_METHOD(T, denorm_min, ());
    PHANTOM_STATIC_METHOD(T, infinity, ());
    PHANTOM_STATIC_METHOD(T, quiet_NaN, ());
    PHANTOM_STATIC_METHOD(T, signaling_NaN, ());
}
} // namespace std

PHANTOM_END("std.limits");

#if defined(_MSC_VER)
#    pragma warning(pop)
#elif defined(__clang__)
#    pragma clang diagnostic pop
#endif
