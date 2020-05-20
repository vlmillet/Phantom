#pragma once
// haunt {

// clang-format off

#include "Alias.h"

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
    PHANTOM_SOURCE("Alias")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Alias)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Symbol*, StringView, Modifiers, uint)>()["\"\""]["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Type*() const, virtual_|override_>("toType", &_::toType)
            .method<Namespace*() const, virtual_|override_>("toNamespace", &_::toNamespace)
            .method<Alias*() const, virtual_|override_>("asAlias", &_::asAlias)
            .method<Symbol*() const>("getAliasedSymbol", &_::getAliasedSymbol)
            .method<void(Symbol*)>("setAliasedSymbol", &_::setAliasedSymbol)
            .method<void(Alias*)>("addAlias", &_::addAlias)
            .method<Alias*(StringView) const>("getAlias", &_::getAlias)
            .method<void(Symbols&, bool) const, virtual_>("fetchAccessibleSymbols", &_::fetchAccessibleSymbols)["true"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Alias")
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
