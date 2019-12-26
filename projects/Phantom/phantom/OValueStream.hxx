#pragma once

// haunt {

#include "OValueStream.h"

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
    PHANTOM_SOURCE("OValueStream")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(OValueStream)
        {
            this_()
        .public_()
            /// missing symbol(s) reflection (std::ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(::std::ostream &, int)>()["4"]
            .method<OValueStream&(const Value&)>("operator<<", &_::operator<<)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("OValueStream")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
