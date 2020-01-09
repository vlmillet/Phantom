#pragma once

// haunt {

#include "AnonymousUnion.h"

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
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("AnonymousUnion")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(AnonymousUnion)
        {
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::AnonymousSection>()
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .method<AnonymousUnion*() const, virtual_|override_>("asAnonymousUnion", &_::asAnonymousUnion)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AnonymousUnion")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
