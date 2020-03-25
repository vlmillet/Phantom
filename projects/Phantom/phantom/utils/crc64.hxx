#pragma once

// haunt {

// clang-format off

#include "crc64.h"

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
#include <phantom/variable>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("crc64")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Variables) { this_().variable("crc64_tab", &crc64_tab); }
        PHANTOM_REGISTER(Functions) { this_().function<uint64_t(uint64_t, const unsigned char*, uint64_t)>("crc64", crc64);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("crc64")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
