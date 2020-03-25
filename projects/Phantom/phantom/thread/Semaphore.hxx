#pragma once

// haunt {

// clang-format off

#include "Semaphore.h"

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

namespace phantom {
PHANTOM_PACKAGE("phantom.thread")
    PHANTOM_SOURCE("Semaphore")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Semaphore)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .constructor<void(uint, int)>()["0"]["0"]
            .method<void()>("wait", &_::wait)
            .method<void(int)>("signal", &_::signal)["1"]
            .method<int() const>("getWaitingCountRelaxed", &_::getWaitingCountRelaxed)
            ;
        }
        PHANTOM_CLASS(LWSemaphore)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .constructor<void(int, int)>()["0"]["10000"]
            .method<bool()>("tryWait", &_::tryWait)
            .method<void()>("wait", &_::wait)
            .method<void(int)>("signal", &_::signal)["1"]
            .method<int() const>("getWaitingCountRelaxed", &_::getWaitingCountRelaxed)
            ;
        }
        PHANTOM_CLASS(SpinSemaphore)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .constructor<void(int, int)>()["0"]["0"]
            .method<bool()>("tryWait", &_::tryWait)
            .method<void()>("wait", &_::wait)
            .method<void(int)>("signal", &_::signal)["1"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Semaphore")
PHANTOM_END("phantom.thread")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
