#pragma once

// haunt {

// clang-format off

#include "AnonymousStruct.h"

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
#include <phantom/static_method>
#include <phantom/constructor>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("AnonymousStruct")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(AnonymousStruct)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::AnonymousSection>()
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .method<AnonymousStruct*() const, virtual_|override_>("asAnonymousStruct", &_::asAnonymousStruct)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AnonymousStruct")
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
