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
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
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
            using SymbolsMap = typedef_<_::SymbolsMap>;
            this_()
            .inherits<::phantom::lang::Symbol, ::phantom::lang::Scope>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<SymbolsMap>("SymbolsMap")
        
        .public_()
            .staticMethod<::phantom::lang::Namespace *()>("Global", &_::Global)
        
        .public_()
            .constructor<void(Modifiers, uint)>()({"a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<Scope*() const, virtual_|override_>("asScope", &_::asScope)
            .method<Namespace*() const, virtual_|override_>("asNamespace", &_::asNamespace)
            .method<Namespace*() const, virtual_|override_>("toNamespace", &_::toNamespace)
            .method<void(Symbol*)>("addCustomSymbol", &_::addCustomSymbol)({"a_pSymbol"})
            .method<void(Symbol*)>("removeCustomSymbol", &_::removeCustomSymbol)({"a_pSymbol"})
            .method<Namespace*() const>("getRootNamespace", &_::getRootNamespace)
            .method<Namespace*(StringView) const>("getNamespace", &_::getNamespace)({"a_strName"})
            .method<Namespace*(StringView, const char*) const>("getNamespaceCascade", &_::getNamespaceCascade)({"qualifiedName","a_SeparatorList"})["\":\""]
            .method<Namespace*() const>("getParentNamespace", &_::getParentNamespace)
            .method<Namespace*(StringView, const char*)>("getOrCreateNamespace", &_::getOrCreateNamespace)({"a_strNamespaceName","separatorPattern"})["\":\""]
            .method<Scope*(StringView, const char*)>("getScopeOrCreateNamespace", &_::getScopeOrCreateNamespace)({"a_strScopeName","separatorPattern"})
            .method<Namespace*(StringView)>("newNamespace", &_::newNamespace)({"a_strNamespace"})
            .method<Alias*(StringView, Namespace*)>("addNamespaceAlias", &_::addNamespaceAlias)({"a_strAlias","a_pNamespace"})
            .method<void(StringView)>("removeNamespaceAlias", &_::removeNamespaceAlias)({"a_strAlias"})
            .method<Alias*(StringView) const>("getNamespaceAlias", &_::getNamespaceAlias)({"a_strName"})
            .method<Namespace*(StringView) const>("getNamespaceAliased", &_::getNamespaceAliased)({"a_strAlias"})
            .method<Namespaces const&() const>("getNamespaces", &_::getNamespaces)
            .method<Aliases const&() const>("getNamespaceAliases", &_::getNamespaceAliases)
            .method<SymbolsMap const&() const>("getSymbols", &_::getSymbols)
            .method<ArrayView<Symbol*>(hash64) const>("getSymbols", &_::getSymbols)({"_hash"})
            .method<String(char) const>("asPath", &_::asPath)({"separator"})["'/'"]
            .method<bool(Symbol*) const, virtual_|override_>("isSymbolHidden", &_::isSymbolHidden)({"a_pSymbol"})
            .method<void(StringView, Symbols&) const, virtual_|override_>("getScopedSymbolsWithName", &_::getScopedSymbolsWithName)({"a_Name","a_Symbols"})
            .method<void(Symbol*, Symbols&) const, virtual_|override_>("getElementDoubles", &_::getElementDoubles)({"a_pElement","out"})
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)({"a_pOther"})
            .using_("Symbol::getQualifiedName")
            .using_("Symbol::getQualifiedDecoratedName")
            .using_("Symbol::getRelativeName")
            .using_("Symbol::getRelativeDecoratedName")
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"a_pTo","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"a_pTo","a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
        
        .protected_()
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolAdded", &_::onScopeSymbolAdded)({"a_pSym"})
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolRemoving", &_::onScopeSymbolRemoving)({"a_pSym"})
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
