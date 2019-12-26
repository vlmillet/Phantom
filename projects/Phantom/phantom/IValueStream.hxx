#pragma once

// haunt {

#include "IValueStream.h"

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
#include <phantom/static_method>
#include <phantom/constructor>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("IValueStream")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(IValueStream)
        {
            this_()
        .public_()
            .staticMethod<const char*()>("last_error", &_::last_error)
            /// missing symbol(s) reflection (std::istream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(::std::istream &)>()
            .method<bool() const>("fail", &_::fail)
            .method<bool(Value&)>("operator>>", &_::operator>>)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("IValueStream")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
