#pragma once

// haunt {

#include "FieldPointer.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("FieldPointer")

        PHANTOM_STRUCT_T((class, class), (T, V), FieldPointer)
        {
            using PointerType = typedef_<PHANTOM_TYPENAME _::PointerType>;
            this_()
            .PHANTOM_T typedef_<PointerType>("PointerType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(PointerType)>()
            .PHANTOM_T method<PointerType() const>("operator PointerType", &_::operator notypedef<PointerType>)
            .PHANTOM_T method<V&(T*, V&&) const>("operator()", &_::operator())
            .PHANTOM_T method<V&(T*, V const&) const>("operator()", &_::operator())
            ;
        }
    PHANTOM_END("FieldPointer")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
