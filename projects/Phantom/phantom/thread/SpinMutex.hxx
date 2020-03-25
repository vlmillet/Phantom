#pragma once

// haunt {

// clang-format off

#include "SpinMutex.h"

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

#include <phantom/template-only-push>

#include "LockGuard.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.thread")
    PHANTOM_SOURCE("SpinMutex")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SpinMutex)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .method<void()>("lock", &_::lock)
            .method<void()>("unlock", &_::unlock)
            .method<LockGuard<SpinMutex>()>("autoLock", &_::autoLock)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SpinMutex")
PHANTOM_END("phantom.thread")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
