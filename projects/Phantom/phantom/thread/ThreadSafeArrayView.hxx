#pragma once

// haunt {

// clang-format off

#include "ThreadSafeArrayView.h"

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
#include <phantom/constructor>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom.thread")
    PHANTOM_SOURCE("ThreadSafeArrayView")

        PHANTOM_STRUCT_T((class, class), (T, Mutex), ThreadSafeArrayView)
        {
            using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .PHANTOM_T typedef_<const_iterator>("const_iterator")
            .PHANTOM_T constructor<void(T const*, size_t, Mutex&)>()
            .PHANTOM_T constructor<void(ThreadSafeArrayView<T, Mutex>&&)>()
            .PHANTOM_T method<T const*() const>("data", &_::data)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<T const*() const>("begin", &_::begin)
            .PHANTOM_T method<T const*() const>("end", &_::end)
            ;
        }
    PHANTOM_END("ThreadSafeArrayView")
PHANTOM_END("phantom.thread")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }

