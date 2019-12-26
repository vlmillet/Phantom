#pragma once

// haunt {

#include "StringHash.h"

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
#include <phantom/function>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("StringHash")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<StringHash>("StringHash"); }
        PHANTOM_REGISTER(Functions) { this_().function<StringHash(StringView)>("makeStringHash", makeStringHash);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringHash")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
