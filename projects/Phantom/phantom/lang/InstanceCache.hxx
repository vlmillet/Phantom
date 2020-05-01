#pragma once

// haunt {

// clang-format off

#include "InstanceCache.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/thread/ThreadSafeArrayView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("InstanceCache")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(InstanceCache)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
        
        .public_()
            .constructor<void(Class*)>()
        
        .public_()
            .method<ThreadSafeArrayView<void*, SpinMutex>() const>("getInstances", &_::getInstances)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("InstanceCache")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
