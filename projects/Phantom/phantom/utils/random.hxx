#pragma once

// haunt {

#include "random.h"

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

#include <phantom/template-only-push>

#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
namespace random {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("random")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void(char*, StringView)>("str", str)["\"%%%%%%%%-%%%%-%%%%-%%%%%%%%%%%%\""];}
        PHANTOM_REGISTER(Functions) { this_().function<uint64_t()>("integer", integer);}
        PHANTOM_REGISTER(Functions) { this_().function<double()>("normalized", normalized);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("random")
PHANTOM_END("phantom.utils")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
