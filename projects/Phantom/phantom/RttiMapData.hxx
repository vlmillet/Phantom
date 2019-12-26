#pragma once

// haunt {

#include "RttiMapData.h"

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
#include <phantom/friend>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("RttiMapData")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(RttiMapData)
        {
            this_()
            .constructor<void(), default_>()
            /// missing symbol(s) reflection (phantom::DynamicDeleteFunc) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(::phantom::reflection::Class *, ::phantom::reflection::Class *, void const*, DynamicDeleteFunc)>()["nullptr"]
            .method<bool() const>("isMostDerived", &_::isMostDerived)
            .method<bool() const>("isNull", &_::isNull)
            .method<void() const>("delete_", &_::delete_)
            .method<void*(::phantom::reflection::Class *) const>("cast", &_::cast)
            .method<void(Object&) const>("asObject", &_::asObject)
            .method<reflection::Class*() const>("getAddressClass", &_::getAddressClass)
            .method<void*() const>("getObjectAddress", &_::getObjectAddress)
            .method<reflection::Class*() const>("getObjectClass", &_::getObjectClass)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("RttiMapData")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
