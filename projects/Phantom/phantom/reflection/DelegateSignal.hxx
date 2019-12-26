#pragma once

// haunt {

#include "DelegateSignal.h"

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
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("DelegateSignal")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(DelegateSignal)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Method>()
        .public_()
            .constructor<void(StringView, Signature*)>()
            .method<void(void*, OpaqueDelegate), pure_virtual>("connect", &_::connect)
            .method<void(void*, OpaqueDelegate), pure_virtual>("disconnect", &_::disconnect)
            .method<void(void*, void*, Method*)>("connect", &_::connect)
            .method<void(void*, void*, Method*)>("disconnect", &_::disconnect)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DelegateSignal")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
