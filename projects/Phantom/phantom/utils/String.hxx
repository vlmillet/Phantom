#pragma once

// haunt {

// clang-format off

#include "String.h"

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

#include "SmallString.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("String")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<const char*(const String&)>("cstr", cstr);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("String")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
