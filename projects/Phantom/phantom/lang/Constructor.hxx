#pragma once

// haunt {

// clang-format off

#include "Constructor.h"

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
    PHANTOM_SOURCE("Constructor")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Constructor)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Method>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void()>()
            .constructor<void(StringView, Signature*, Modifiers, uint)>()["0"]["0"]
            .method<void(void*, void**) const>("construct", &_::construct)
            .method<void(size_t, void*, void**)>("construct", &_::construct)
            .method<Constructor*() const, virtual_|override_>("asConstructor", &_::asConstructor)
        
        .protected_()
            .method<void**() const, virtual_>("getNativeVTablePointer", &_::getNativeVTablePointer)
            .method<size_t() const, virtual_>("getNativeVTableSize", &_::getNativeVTableSize)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Constructor")
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
