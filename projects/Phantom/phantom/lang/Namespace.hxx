#pragma once

// haunt {

// clang-format off

#include "Namespace.h"

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Namespace")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Namespace)
        {
            using Aliases = typedef_< phantom::lang::Aliases>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Namespaces = typedef_< phantom::lang::Namespaces>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol, ::phantom::lang::Scope>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .public_()
            .staticMethod<Namespace*()>("Global", &_::Global)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<Scope*() const, virtual_|override_>("asScope", &_::asScope)
            .method<Namespace*() const, virtual_|override_>("asNamespace", &_::asNamespace)
            .method<Namespace*() const, virtual_|override_>("toNamespace", &_::toNamespace)
            .method<Namespace*() const>("getRootNamespace", &_::getRootNamespace)
            .method<Namespace*(StringView) const>("getNamespace", &_::getNamespace)
            .method<Namespace*(StringView, const char*) const>("getNamespaceCascade", &_::getNamespaceCascade)["\":\""]
            .method<Namespace*() const>("getParentNamespace", &_::getParentNamespace)
            .method<Namespace*(StringView, const char*)>("getOrCreateNamespace", &_::getOrCreateNamespace)["\":\""]
            .method<Namespace*(StringView)>("newNamespace", &_::newNamespace)
            .method<Alias*(StringView, Namespace*)>("addNamespaceAlias", &_::addNamespaceAlias)
            .method<void(StringView)>("removeNamespaceAlias", &_::removeNamespaceAlias)
            .method<Alias*(StringView) const>("getNamespaceAlias", &_::getNamespaceAlias)
            .method<Namespace*(StringView) const>("getNamespaceAliased", &_::getNamespaceAliased)
            .method<Namespaces const&() const>("getNamespaces", &_::getNamespaces)
            .method<Aliases const&() const>("getNamespaceAliases", &_::getNamespaceAliases)
            .method<Symbols const&() const>("getSymbols", &_::getSymbols)
            .method<String(char) const>("asPath", &_::asPath)["'/'"]
            .method<bool(Symbol*) const, virtual_|override_>("isSymbolHidden", &_::isSymbolHidden)
            .method<void(StringView, Symbols&) const, virtual_|override_>("getScopedSymbolsWithName", &_::getScopedSymbolsWithName)
            .method<void(Symbol*, Symbols&) const, virtual_|override_>("getElementDoubles", &_::getElementDoubles)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .using_("Symbol::getQualifiedName")
            .using_("Symbol::getQualifiedDecoratedName")
            .using_("Symbol::getRelativeName")
            .using_("Symbol::getRelativeDecoratedName")
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .protected_()
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolAdded", &_::onScopeSymbolAdded)
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolRemoving", &_::onScopeSymbolRemoving)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Namespace")
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
