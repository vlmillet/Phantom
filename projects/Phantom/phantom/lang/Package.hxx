#pragma once

// haunt {

// clang-format off

#include "Package.h"

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Package")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Package)
        {
            using PackageFolders = typedef_< phantom::lang::PackageFolders>;
            using Sources = typedef_< phantom::lang::Sources>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<bool(StringView)>("IsValidName", &_::IsValidName)
        
        .public_()
            .constructor<void(StringView)>()
            .method<void(), virtual_>("terminate", &_::terminate)
            .method<Package*() const, virtual_|override_>("asPackage", &_::asPackage)
            .method<Module*() const>("getModule", &_::getModule)
            .method<PackageFolder*() const>("getPackageFolder", &_::getPackageFolder)
            .method<Sources const&()>("getSources", &_::getSources)
            .method<Source*(StringView) const>("getSource", &_::getSource)
            .method<Source*(StringView, Visibility)>("getOrCreateSource", &_::getOrCreateSource)["Visibility::Public"]
            .method<Source*(StringView, Visibility)>("newSource", &_::newSource)["Visibility::Public"]
            .method<void(Source*)>("addSource", &_::addSource)
            .method<void(Source*)>("removeSource", &_::removeSource)
            .method<void(Source*)>("deleteSource", &_::deleteSource)
            .method<Namespace*() const>("getCounterpartNamespace", &_::getCounterpartNamespace)
            .using_("Symbol::getQualifiedDecoratedName")
            .using_("Symbol::getQualifiedName")
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<bool() const, virtual_|override_>("canBeUnloaded", &_::canBeUnloaded)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<String() const>("pathName", &_::pathName)
            .method<void(PackageFolders&) const>("getPackageFolders", &_::getPackageFolders)
            .method<CustomAllocator const*() const, virtual_|override_>("getAllocator", &_::getAllocator)
        
        .public_()
            .field("sourceAdded", &_::sourceAdded)
            .field("sourceAboutToBeRemoved", &_::sourceAboutToBeRemoved)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Package")
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
