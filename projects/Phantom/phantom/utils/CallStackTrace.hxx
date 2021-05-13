#pragma once

// haunt {

// clang-format off

#include "CallStackTrace.h"

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
#include <phantom/static_method>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("CallStackTrace")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(CallStackTrace)
        {
            this_()
            .staticMethod<size_t(void**, size_t)>("Capture", &_::Capture)({"backTrace","backTraceSize"})
            .staticMethod<void(const void* const*, size_t, char*, size_t)>("ToString", &_::ToString)({"backTrace","backTraceSize","buffer","bufferSize"})
            .staticMethod<int(const void*)>("GetSymbolLine", &_::GetSymbolLine)({"_ptr"})
            .staticMethod<size_t(const void*, char*, size_t)>("GetSymbolName", &_::GetSymbolName)({"pt","buffer","buffersize"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CallStackTrace")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
