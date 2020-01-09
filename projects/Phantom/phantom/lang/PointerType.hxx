#pragma once

// haunt {

#include "PointerType.h"

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PointerType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PointerType)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ExtendedType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(Type*, StringView, size_t, size_t, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<Type*() const, virtual_|override_>("asPOD", &_::asPOD)
            .method<PointerType*() const, virtual_|override_>("asPointerType", &_::asPointerType)
            .method<void(void**, void*)>("less", &_::less)
            .method<void(void**, void*)>("greater", &_::greater)
            .method<void(void**, void*)>("lessEqual", &_::lessEqual)
            .method<void(void**, void*)>("greaterEqual", &_::greaterEqual)
            .method<void(void**, void*)>("equal", &_::equal)
            .method<void(void**, void*)>("notEqual", &_::notEqual)
            .using_("Type::equal")
            .method<void(void**, void*)>("assignment", &_::assignment)
        
        .protected_()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PointerType")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
