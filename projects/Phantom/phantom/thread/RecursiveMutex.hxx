#pragma once

// haunt {

// clang-format off

#include "RecursiveMutex.h"

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

#include <phantom/template-only-push>

#include "LockGuard.hxx"
#include "RecursiveSemaphore.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.thread")
    PHANTOM_SOURCE("RecursiveMutex")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<RecursiveMutex>("RecursiveMutex"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<RecursiveMutexGuard>("RecursiveMutexGuard"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("RecursiveMutex")
PHANTOM_END("phantom.thread")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
