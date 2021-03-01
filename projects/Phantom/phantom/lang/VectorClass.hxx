#pragma once

// haunt {

// clang-format off

#include "VectorClass.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("VectorClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(VectorClass)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::SequentialContainerClass>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<const void*(void const*) const, virtual_>("data", &_::data)
            .method<void*(void*) const, virtual_>("data", &_::data)
            .method<void(void*, size_t) const, virtual_>("resize", &_::resize)
            .method<const void*(void const*, size_t) const, virtual_|override_>("referenceAt", &_::referenceAt)
            .method<void*(void*, size_t) const, virtual_|override_>("referenceAt", &_::referenceAt)
        
        .protected_()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VectorClass")
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
