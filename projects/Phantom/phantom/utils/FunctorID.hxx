#pragma once

// haunt {

#include "FunctorID.h"

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

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("FunctorID")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(FunctorID)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(size_t, size_t)>()
            .constructor<void(void*, void*)>()
            .method<bool(FunctorID const&) const>("operator==", &_::operator==)
            .method<bool(FunctorID const&) const>("operator!=", &_::operator!=)
            .method<bool(FunctorID) const>("operator<", &_::operator<)
            .method<bool() const>("isNull", &_::isNull)
            .method<bool() const>("operator bool", &_::operator notypedef<bool>)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FunctorID")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
