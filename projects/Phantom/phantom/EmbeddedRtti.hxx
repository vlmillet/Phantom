#pragma once

// haunt {

#include "EmbeddedRtti.h"

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
#include <phantom/field>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("EmbeddedRtti")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(EmbeddedRtti)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(EmbeddedRtti const&)>()
            .method<EmbeddedRtti&(EmbeddedRtti const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .constructor<void(EmbeddedRtti&&)>()
            .method<EmbeddedRtti&(EmbeddedRtti&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<void(Object&) const>("asObject", &_::asObject)
            .field("instance", &_::instance)
            .field("metaClass", &_::metaClass)
            ;
        }
        PHANTOM_STRUCT(EmbeddedProxyRtti)
        {
            this_()
            .inherits<::phantom::EmbeddedRtti>()
            .constructor<void(), default_>()
            .constructor<void(EmbeddedProxyRtti const&)>()
            .method<EmbeddedProxyRtti&(EmbeddedProxyRtti const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .constructor<void(EmbeddedProxyRtti&&)>()
            .method<EmbeddedProxyRtti&(EmbeddedProxyRtti&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// missing symbol(s) reflection (phantom::DynamicDeleteFunc) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("dynamicDeleteFunc", &_::dynamicDeleteFunc)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("EmbeddedRtti")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
