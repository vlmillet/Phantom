#pragma once

// haunt {

#include "FunctionPointer.h"

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
#include <phantom/class>
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("FunctionPointer")

        PHANTOM_CLASS_T((class), (Sign), FunctionPointer)
        {
            this_()
            ;
        }
        PHANTOM_STRUCT_TVS((class), (R), (class, Params), (R(Params...)), FunctionPointer)
        {
            using PointerType = typedef_<PHANTOM_TYPENAME _::PointerType>;
            this_()
            .PHANTOM_T typedef_<PointerType>("PointerType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(PointerType)>()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T constructor<void(int)>()
            .PHANTOM_T method<PointerType() const>("operator PointerType", &_::operator notypedef<PointerType>)
            .PHANTOM_T method<R(Params...) const>("operator()", &_::operator())
            ;
        }
    PHANTOM_END("FunctionPointer")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
