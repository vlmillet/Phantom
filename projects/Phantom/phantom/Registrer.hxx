#pragma once

// haunt {

#include "Registrer.h"

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
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Registrer")

        #if PHANTOM_NOT_TEMPLATE
        /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        // PHANTOM_REGISTER(Typedefs) { this_().typedef_<Registrer>("Registrer"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Registrer")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
