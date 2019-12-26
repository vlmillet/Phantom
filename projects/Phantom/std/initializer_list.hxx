#pragma once

#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/constructor>
#include <phantom/method>

PHANTOM_PACKAGE("std")
PHANTOM_SOURCE("initializer_list")

namespace std
{
    /// initializer_list
    PHANTOM_CLASS_T((class), (t_Ty), initializer_list)
    {
        this_()
            .PHANTOM_T method<const t_Ty*() const>("begin", &_::begin)
            .PHANTOM_T method<const t_Ty*() const>("end", &_::end)
            .PHANTOM_T method<size_t() const>("size", &_::size)
        ;
    }
}

PHANTOM_END("initializer_list")
PHANTOM_END("std")

