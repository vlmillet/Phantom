#pragma once

// haunt {

// clang-format off

#include "Object.h"

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
#include <phantom/field>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Object")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Object)
        {
            this_()
            .inherits</* ::phantom::MemorySentry */>()
        .public_()
            /// missing symbol(s) reflection (phantom::RTTI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field<transient>("rtti", &_::rtti)
        
        .public_()
            .method<void*() const>("getMostDerived", &_::getMostDerived)
            .method<::phantom::lang::Class *() const>("getMetaClass", &_::getMetaClass)
            .method<void const*(::phantom::lang::Class *) const>("as", &_::as)({"a_pClass"})
            .method<void*(::phantom::lang::Class *)>("as", &_::as)({"a_pClass"})
            .method<void(), virtual_>("terminate", &_::terminate)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (T), IsObject)
        {
            this_()
            ;
        }
    PHANTOM_END("Object")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
