#pragma once

// haunt {

#include "ObjectImpl.h"

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
#include <phantom/method>
#include <phantom/constructor>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Object")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Object)
        {
            this_()
        .public_()
            .constructor<void(), default_>()
            .constructor<void(void*, ::phantom::reflection::Class *)>()
            .method<bool() const>("isNull", &_::isNull)
            .method<void()>("destroy", &_::destroy)
            .method<bool(const Object&) const>("operator==", &_::operator==)
            .method<bool(const Object&) const>("operator!=", &_::operator!=)
            .method<void*() const>("getAddress", &_::getAddress)
            .method<reflection::Class*() const>("getClass", &_::getClass)
            .method<Object(::phantom::reflection::Class *) const>("as", &_::as)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Object")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
