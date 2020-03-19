#pragma once

// haunt {

// clang-format off

#include "fwd.h"

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
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "utils/SmallVector.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("fwd")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(NullMutex)
        {
            this_()
            .method<void()>("lock", &_::lock)
            .method<void()>("unlock", &_::unlock)
            .method<bool()>("try_lock", &_::try_lock)
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Variants>("Variants"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("fwd")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }

