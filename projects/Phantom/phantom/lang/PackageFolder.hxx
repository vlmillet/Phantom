#pragma once

// haunt {

// clang-format off

#include "PackageFolder.h"

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

#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PackageFolder")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PackageFolder)
        {
            using PackageFolders = typedef_< phantom::lang::PackageFolders>;
            using Packages = typedef_< phantom::lang::Packages>;
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
            .constructor<void()>()
            .constructor<void(StringView)>()
            .method<PackageFolder*(String)>("newPackageFolder", &_::newPackageFolder)
            .method<PackageFolder*() const, virtual_|override_>("asPackageFolder", &_::asPackageFolder)
            .method<PackageFolder*() const>("getParentFolder", &_::getParentFolder)
            .method<Packages const&() const>("getPackages", &_::getPackages)
            .method<Package*(Module*) const>("getPackage", &_::getPackage)
            .method<void(PackageFolders&) const>("getPackageFolders", &_::getPackageFolders)
            .method<PackageFolders const&() const>("getPackageFolders", &_::getPackageFolders)
            .method<PackageFolder*(StringView) const>("getPackageFolder", &_::getPackageFolder)
            .method<PackageFolder*(StringView) const>("getOrCreatePackageFolder", &_::getOrCreatePackageFolder)
            .method<Source*(StringView) const>("getSource", &_::getSource)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<CustomAllocator const*() const, virtual_|override_>("getAllocator", &_::getAllocator)
        
        .public_()
            .field("packageFolderAdded", &_::packageFolderAdded)
            .field("packageFolderAboutToBeRemoved", &_::packageFolderAboutToBeRemoved)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PackageFolder")
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
