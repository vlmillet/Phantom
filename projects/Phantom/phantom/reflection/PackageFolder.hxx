#pragma once

// haunt {

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("PackageFolder")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PackageFolder)
        {
            using PackageFolders = typedef_< phantom::reflection::PackageFolders>;
            using Packages = typedef_< phantom::reflection::Packages>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .staticMethod<PackageFolder*()>("Create", &_::Create)
            .constructor<void()>()
            .constructor<void(StringView)>()
            .method<void(PackageFolder*)>("addPackageFolder", &_::addPackageFolder)
        
        .public_()
            .method<PackageFolder*() const, virtual_|override_>("asPackageFolder", &_::asPackageFolder)
            .method<PackageFolder*() const>("getParentFolder", &_::getParentFolder)
            .method<Packages const&() const>("getPackages", &_::getPackages)
            .method<Package*(Module*) const>("getPackage", &_::getPackage)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int() const>("beginPackageFolders", &_::beginPackageFolders)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int() const>("endPackageFolders", &_::endPackageFolders)
            .method<void(PackageFolders&) const>("getPackageFolders", &_::getPackageFolders)
            .method<PackageFolders const&() const>("getPackageFolders", &_::getPackageFolders)
            .method<PackageFolder*(StringView) const>("getPackageFolder", &_::getPackageFolder)
            .method<PackageFolder*(StringView) const>("getOrCreatePackageFolder", &_::getOrCreatePackageFolder)
            .method<Source*(StringView) const>("getSource", &_::getSource)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .public_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("packageFolderAdded", &_::packageFolderAdded)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("packageFolderAboutToBeRemoved", &_::packageFolderAboutToBeRemoved)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PackageFolder")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
