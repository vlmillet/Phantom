#pragma once

// haunt {

#include "PlaceholderType.h"

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

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("PlaceholderType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PlaceholderType)
        {
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Type, ::phantom::reflection::Placeholder>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<bool(LanguageElement*) const, virtual_|override_>("accepts", &_::accepts)
            .method<Symbol*() const, virtual_|override_>("asSymbol", &_::asSymbol)
            .method<Placeholder*() const, virtual_|override_>("asPlaceholder", &_::asPlaceholder)
            .method<Class*() const, virtual_|override_>("asClass", &_::asClass)
            .method<ClassType*() const, virtual_|override_>("asClassType", &_::asClassType)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<PlaceholderType*() const, virtual_|override_>("asPlaceholderType", &_::asPlaceholderType)
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<Placeholder*(uint) const, virtual_|override_>("clone", &_::clone)["0"]
        
        .protected_()
            .field("m_pAsClass", &_::m_pAsClass)
            .field("m_pAsClassType", &_::m_pAsClassType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderType")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
