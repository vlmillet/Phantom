#pragma once

// haunt {

// clang-format off

#include "UserData.h"

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

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("UserData")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(UserData)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .constructor<void(), default_>()
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(void*, Delegate<void (void*)>)>()
            .constructor<void(UserData&&)>()
            .method<UserData&(UserData&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<void*() const>("getData", &_::getData)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("UserData")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
