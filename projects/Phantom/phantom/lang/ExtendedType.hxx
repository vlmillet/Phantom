#pragma once

// haunt {

#include "ExtendedType.h"

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
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ExtendedType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ExtendedType)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(Type*, TypeKind, StringView, size_t, size_t, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<Type*() const, virtual_|override_>("asPOD", &_::asPOD)
            .method<ExtendedType*() const, virtual_|override_>("asExtendedType", &_::asExtendedType)
            .method<Type*() const, virtual_|override_>("getUnderlyingType", &_::getUnderlyingType)
            .method<Type*() const, virtual_|override_>("removeEverything", &_::removeEverything)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
        
        .protected_()
            .field("m_pUnderlyingType", &_::m_pUnderlyingType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ExtendedType")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
