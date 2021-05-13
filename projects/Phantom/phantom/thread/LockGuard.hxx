#pragma once

// haunt {

// clang-format off

#include "LockGuard.h"

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
#include <phantom/constructor>

namespace phantom {
PHANTOM_PACKAGE("phantom.thread")
    PHANTOM_SOURCE("LockGuard")

        PHANTOM_STRUCT_T((class), (MutexT), LockGuard)
        {
            this_()
            .PHANTOM_T constructor<void(MutexT&)>()({"_m"})
            ;
        }
    PHANTOM_END("LockGuard")
PHANTOM_END("phantom.thread")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
