#pragma once

// haunt {

// clang-format off

#include "RecursiveSemaphore.h"

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

#include <phantom/template-only-push>

#include "LockGuard.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.thread")
    PHANTOM_SOURCE("RecursiveSemaphore")

        PHANTOM_CLASS_T((class), (t_Semaphore), RecursiveSemaphore)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(int)>()["10000"]
            )
            .PHANTOM_T method<void()>("lock", &_::lock)
            .PHANTOM_T method<bool()>("tryLock", &_::tryLock)
            .PHANTOM_T method<void()>("unlock", &_::unlock)
            .PHANTOM_T method<LockGuard<RecursiveSemaphore<t_Semaphore> >()>("autoLock", &_::autoLock)
            ;
        }
    PHANTOM_END("RecursiveSemaphore")
PHANTOM_END("phantom.thread")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
