#pragma once

// haunt {

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
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Namespace")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Namespace)
        {
            using Aliases = typedef_< phantom::reflection::Aliases>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using Namespaces = typedef_< phantom::reflection::Namespaces>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::reflection::Symbols>;
            this_()
            .inherits<::phantom::reflection::Symbol, ::phantom::reflection::Scope>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .public_()
            .staticMethod<Namespace*()>("Global", &_::Global)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<Scope*() const, virtual_|override_>("asScope", &_::asScope)
            .method<Namespace*() const>("asNamespace", &_::asNamespace)
            .method<Namespace*() const, virtual_|override_>("toNamespace", &_::toNamespace)
            .method<void(Symbol*), virtual_|override_>("addScopeElement", &_::addScopeElement)
            .method<void(Symbol*), virtual_|override_>("removeScopeElement", &_::removeScopeElement)
            .method<void(Namespace*)>("addNamespace", &_::addNamespace)
            .method<void(Namespace*)>("removeNamespace", &_::removeNamespace)
            .method<Namespace*() const>("getRootNamespace", &_::getRootNamespace)
            .method<Namespace*(StringView) const>("getNamespace", &_::getNamespace)
            .method<Namespace*(StringView, const char*) const>("getNamespaceCascade", &_::getNamespaceCascade)["\":\""]
            .method<Namespace*() const>("getParentNamespace", &_::getParentNamespace)
            .method<Namespace*(StringView, const char*)>("findOrCreateNamespace", &_::findOrCreateNamespace)["\":\""]
            .method<Alias*(StringView, Namespace*)>("addNamespaceAlias", &_::addNamespaceAlias)
            .method<void(StringView)>("removeNamespaceAlias", &_::removeNamespaceAlias)
            .method<Alias*(StringView) const>("getNamespaceAlias", &_::getNamespaceAlias)
            .method<Namespace*(StringView) const>("getNamespaceAliased", &_::getNamespaceAliased)
            .method<Namespaces const&() const>("getNamespaces", &_::getNamespaces)
            .method<Aliases const&() const>("getNamespaceAliases", &_::getNamespaceAliases)
            .method<String(char) const>("asPath", &_::asPath)["'/'"]
            .method<bool(Symbol*) const>("isSymbolHidden", &_::isSymbolHidden)
            .method<void(StringView, Symbols&) const, virtual_|override_>("getScopedSymbolsWithName", &_::getScopedSymbolsWithName)
            .method<void(Symbol*, Symbols&) const, virtual_|override_>("getElementDoubles", &_::getElementDoubles)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<void(StringBuffer&) const>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .public_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("namespaceAdded", &_::namespaceAdded)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("namespaceRemoved", &_::namespaceRemoved)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("namespaceAliasAdded", &_::namespaceAliasAdded)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("namespaceAliasRemoved", &_::namespaceAliasRemoved)
        
        .protected_()
            .method<void(Namespace*)>("setParentNamespace", &_::setParentNamespace)
            .method<void(LanguageElement*)>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*)>("onElementRemoved", &_::onElementRemoved)
        
        .protected_()
            .field("m_Namespaces", &_::m_Namespaces)
            .field("m_NamespaceAliases", &_::m_NamespaceAliases)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Namespace")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
