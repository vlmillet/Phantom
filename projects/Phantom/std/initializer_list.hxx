#pragma once

#include <phantom/class>
#include <phantom/constructor>
#include <phantom/method>
#include <phantom/package>
#include <phantom/source>

PHANTOM_PACKAGE("std")
PHANTOM_SOURCE("initializer_list")

namespace std
{
/// initializer_list
PHANTOM_CLASS_T((class), (t_Ty), initializer_list)
{
    this_()
    .public_()
    .PHANTOM_T method<const t_Ty*() const>("begin", &_::begin)
    .PHANTOM_T method<const t_Ty*() const>("end", &_::end)
    .PHANTOM_T method<size_t() const>("size", &_::size);
}
} // namespace std

PHANTOM_END("initializer_list")
PHANTOM_END("std")
