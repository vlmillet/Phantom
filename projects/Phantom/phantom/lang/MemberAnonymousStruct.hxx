#pragma once

// haunt {

// clang-format off

#include "MemberAnonymousStruct.h"

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
    PHANTOM_SOURCE("MemberAnonymousStruct")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MemberAnonymousStruct)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            this_()
            .inherits<::phantom::lang::MemberAnonymousSection>()
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()({"modifiers","a_uiFlags"})["0"]["0"]
            .method<MemberAnonymousStruct*() const, virtual_|override_>("asMemberAnonymousStruct", &_::asMemberAnonymousStruct)
        
        .protected_()
            .method<void(size_t&, size_t&) const, virtual_|override_>("computeSizeAndAlignment", &_::computeSizeAndAlignment)({"a_uiSize","a_uiAlignment"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemberAnonymousStruct")
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
