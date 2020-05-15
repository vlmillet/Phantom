#pragma once

// haunt {

// clang-format off

#include "SequentialContainerClass.h"

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
    PHANTOM_SOURCE("SequentialContainerClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SequentialContainerClass)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ContainerClass>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<SequentialContainerClass*() const, virtual_|override_>("asSequentialContainerClass", &_::asSequentialContainerClass)
            .method<void(void*, void const*) const, virtual_>("push_back", &_::push_back)
            /// missing symbol(s) reflection (phantom::MoveArg) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(void*, MoveArg) const, virtual_>("push_back", &_::push_back)
            .method<void(void*) const, virtual_>("pop_back", &_::pop_back)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SequentialContainerClass")
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
