#pragma once

// haunt {

// clang-format off

#include "Source.h"

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
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Source")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Source)
        {
            using Import = typedef_<_::Import>;
            using Imports = typedef_<_::Imports>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Sources = typedef_< phantom::lang::Sources>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol, ::phantom::lang::Scope>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .struct_<Import>()
                .field("symbol", &_::Import::symbol)
                .field("isPublic", &_::Import::isPublic)
                .field("isStatic", &_::Import::isStatic)
                .field("alias", &_::Import::alias)
            .end()
            .typedef_<Imports>("Imports")
        
        .public_()
            .enum_<>().values({
                {"InvalidVersionNumber",_::InvalidVersionNumber}})
            .end()
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<Package*() const>("getPackage", &_::getPackage)
            .method<Source*() const>("getNativeArchive", &_::getNativeArchive)
            .method<SourceStream*() const>("getSourceStream", &_::getSourceStream)
            .method<FunctionType*(Type*, TypesView, Modifiers, uint)>("functionType", &_::functionType)["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctionPointer*(Type*, ABI, TypesView, Modifiers, uint)>("functionPointerType", &_::functionPointerType)["0"]["0"]
            /// missing symbol(s) reflection (phantom::lang::ABI) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctionPointer*(FunctionType*, ABI, Modifiers, uint)>("functionPointerType", &_::functionPointerType)["0"]["0"]
            .method<InitializerListType*(TypesView)>("initializerListType", &_::initializerListType)
            .method<MethodPointer*(ClassType*, Type*, TypesView, Modifiers, uint)>("methodPointerType", &_::methodPointerType)["0"]["0"]
            .method<FieldPointer*(ClassType*, Type*, Modifiers, uint)>("fieldPointerType", &_::fieldPointerType)["0"]["0"]
            .method<void(SourceStream*)>("setSourceStream", &_::setSourceStream)
            .method<Imports const&() const>("getImports", &_::getImports)
            .method<void(Symbols&) const>("getImported", &_::getImported)
            .method<Sources const&() const>("getImportings", &_::getImportings)
            .method<bool(Symbol*, bool, bool)>("addImport", &_::addImport)["false"]["false"]
            .method<bool(StringView, bool, bool)>("addImport", &_::addImport)["false"]["false"]
            .method<void(Symbol*)>("removeImport", &_::removeImport)
            .method<bool(Symbol*) const>("hasImport", &_::hasImport)
            .method<bool(Symbol*) const>("hasImported", &_::hasImported)
            .method<bool(Symbol*, Access, Modifiers, uint, SmallMap<Symbol*, Symbols>*) const>("canImport", &_::canImport)["Access::Public"]["0"]["0"]["nullptr"]
            .method<const Sources&() const>("getDependencies", &_::getDependencies)
            .method<bool(Source*) const>("hasDependency", &_::hasDependency)
            .method<bool(Source*) const>("hasDependencyCascade", &_::hasDependencyCascade)
            .method<void(Source*)>("addDependency", &_::addDependency)
            .method<bool(StringView)>("addDependency", &_::addDependency)
            .method<void(Source*)>("removeDependency", &_::removeDependency)
            .method<Sources const&() const>("getDependings", &_::getDependings)
            .method<void()>("clearImports", &_::clearImports)
            .method<void()>("clearDependencies", &_::clearDependencies)
            .method<Source*() const, virtual_|override_>("asSource", &_::asSource)
            .method<Scope*() const, virtual_|override_>("asScope", &_::asScope)
            .method<bool() const, virtual_|override_>("canBeUnloaded", &_::canBeUnloaded)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getUniqueName")
            .method<void(LanguageElement*)>("addElement", &_::addElement)
            .method<void(LanguageElement*)>("removeElement", &_::removeElement)
            .method<Source*() const, virtual_|override_>("getCodeLocationSource", &_::getCodeLocationSource)
        
        .protected_()
            .constructor<void(Package*, StringView, Modifiers, uint)>()["0"]
        
        .protected_()
            .method<void(Symbol*), virtual_|override_>("addScopeElement", &_::addScopeElement)
            .method<void(Symbol*), virtual_|override_>("removeScopeElement", &_::removeScopeElement)
            .method<void(LanguageElement*), virtual_|override_>("onElementAdded", &_::onElementAdded)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
        
        .public_()
            .field("sourceStreamChanged", &_::sourceStreamChanged)
            .field("buildSucceeded", &_::buildSucceeded)
        
        .protected_()
            .field("m_pSourceStream", &_::m_pSourceStream)
            .field("m_pNativeArchive", &_::m_pNativeArchive)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Source")
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
