#pragma once

// haunt {

// clang-format off

#include "PlaceholderConstant.h"

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PlaceholderConstant")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PlaceholderConstant)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Constant, ::phantom::lang::Placeholder>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Type*, StringView, Modifiers, uint)>()["0"]["0"]
            .method<bool(LanguageElement*) const, virtual_|override_>("accepts", &_::accepts)
            .method<Symbol*() const, virtual_|override_>("asSymbol", &_::asSymbol)
            .method<Placeholder*() const, virtual_|override_>("asPlaceholder", &_::asPlaceholder)
            .method<PlaceholderConstant*() const, virtual_|override_>("asPlaceholderConstant", &_::asPlaceholderConstant)
            .method<void(void*) const, virtual_|override_>("getValue", &_::getValue)
            .method<bool(const void*) const, virtual_|override_>("hasValue", &_::hasValue)
            .method<bool() const, virtual_|override_>("isZero", &_::isZero)
            .method<void(StringBuffer&) const, virtual_|override_>("toString", &_::toString)
            .method<void(StringBuffer&) const, virtual_|override_>("toLiteral", &_::toLiteral)
            .method<void const*() const, virtual_|override_>("getAddress", &_::getAddress)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<Placeholder*(uint) const, virtual_|override_>("clone", &_::clone)["0"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderConstant")
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
