#pragma once

// haunt {

// clang-format off

#include "Module.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Module")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Module)
        {
            using Classes = typedef_< phantom::lang::Classes>;
            using Dependencies = typedef_<_::Dependencies>;
            using DependencyPred = typedef_<_::DependencyPred>;
            using FuncT = typedef_<_::FuncT>;
            using Functions = typedef_< phantom::lang::Functions>;
            using Modules = typedef_< phantom::lang::Modules>;
            using Packages = typedef_< phantom::lang::Packages>;
            using Sources = typedef_< phantom::lang::Sources>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::lang::Types>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<Dependencies>("Dependencies")
        
        .public_()
            .constructor<void(StringView, uint)>()({"a_strName","a_uiFlags"})["0"]
            .constructor<void(size_t, size_t, StringView, StringView, StringView, uint)>()({"a_NativeHandle","a_NativeImageSize","a_strName","a_LibFullName","a_DeclarationCppFullName","a_uiFlags"})["0"]
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Module*() const, virtual_|override_>("asModule", &_::asModule)
            .method<bool() const>("isPlugin", &_::isPlugin)
            .method<bool() const>("isInstalled", &_::isInstalled)
            .method<Plugin*() const>("getPlugin", &_::getPlugin)
            .method<StringView() const>("getLibraryFullName", &_::getLibraryFullName)
            .method<StringView() const>("getDeclarationCppFullName", &_::getDeclarationCppFullName)
            .method<byte*() const>("getMemoryStart", &_::getMemoryStart)
            .method<size_t() const>("getHandle", &_::getHandle)
            .method<void*() const>("getBaseAddress", &_::getBaseAddress)
            .method<size_t() const>("getImageSize", &_::getImageSize)
            .method<void(void*)>("setBaseAddress", &_::setBaseAddress)({"a_pBaseAddress"})
            .method<void(size_t)>("setImageSize", &_::setImageSize)({"a_ImageSize"})
            .method<Packages const&() const>("getPackages", &_::getPackages)
            .method<bool() const, virtual_|override_>("canBeUnloaded", &_::canBeUnloaded)
            .method<Package*() const>("getDefaultPackage", &_::getDefaultPackage)
            .method<Package*(StringView) const>("getPackage", &_::getPackage)({"a_strName"})
            .method<Package*(StringView)>("getOrCreatePackage", &_::getOrCreatePackage)({"a_strName"})
            .method<Package*(StringView)>("newPackage", &_::newPackage)({"a_strName"})
            .method<ArrayView<Module*>() const>("getDependencies", &_::getDependencies)
            .method<ArrayView<Module*>() const>("getDependents", &_::getDependents)
            .method<void(Module*)>("addDependency", &_::addDependency)({"a_pModule"})
            .method<void(Module*)>("removeDependency", &_::removeDependency)({"a_pModule"})
            .method<bool(Module*) const>("hasDependency", &_::hasDependency)({"a_pModule"})
            .method<bool(Module*) const>("hasDependencyCascade", &_::hasDependencyCascade)({"a_pModule"})
            .struct_<DependencyPred>()
                .method<bool(Module*, Module*) const>("operator()", &_::DependencyPred::operator())({"a_pM0","a_pM1"})
            .end()
            .method<int() const>("getDependencyLevel", &_::getDependencyLevel)
            .method<void(Sources&) const>("getSources", &_::getSources)({"a_Sources"})
            .method<void(Modules&) const>("fetchDependencies", &_::fetchDependencies)({"a_Modules"})
            .method<void(Classes&, ::phantom::lang::Class *, bool) const>("findClasses", &_::findClasses)({"a_Classes","a_pBaseClass","a_bDefaultInstanciable"})["nullptr"]["false"]
            .method<void(Functions&, StringView, const Types*, Type*) const>("findFunctions", &_::findFunctions)({"a_Functions","a_Name","a_pParameterTypes","a_pReturnType"})["\"\""]["nullptr"]["nullptr"]
            .method<Source*() const>("getAnonymousSource", &_::getAnonymousSource)
            /// missing symbol(s) reflection (phantom::MemoryContext) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<MemoryContext&()>("getMemoryContext", &_::getMemoryContext)
            .method<Type*(hash64) const>("findType", &_::findType)({"a_Hash"})
            .method<Type*(hash64) const>("findUsableType", &_::findUsableType)({"a_Hash"})
            .typedef_<FuncT>("FuncT")
            .method<void(FuncT)>("setOnLoadFunc", &_::setOnLoadFunc)({"func"})
            .method<void(FuncT)>("setOnUnloadFunc", &_::setOnUnloadFunc)({"func"})
            .method<FuncT() const>("getOnLoadFunc", &_::getOnLoadFunc)
            .method<FuncT() const>("getOnUnloadFunc", &_::getOnUnloadFunc)
            .method<bool() const>("isMarkedOutdated", &_::isMarkedOutdated)
            .method<void()>("markOutdated", &_::markOutdated)
            .method<void()>("markUpToDate", &_::markUpToDate)
            .method<CustomAllocator const*() const, virtual_|override_>("getAllocator", &_::getAllocator)
            .method<void()>("setInstalled", &_::setInstalled)
        
        .public_()
            .field("packageAdded", &_::packageAdded)
            .field("packageAboutToBeRemoved", &_::packageAboutToBeRemoved)
            .field("changed", &_::changed)
            .field("installed", &_::installed)
        
        .protected_()
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
            ;
        }
        PHANTOM_STRUCT(ModuleReverseDependencySorter)
        {
            this_()
            .method<bool(Module*, Module*) const>("operator()", &_::operator())({"m0","m1"})
            ;
        }
        PHANTOM_STRUCT(ModuleDependencySorter)
        {
            this_()
            .method<bool(Module*, Module*) const>("operator()", &_::operator())({"m0","m1"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (T), Module::DeleteMetaH)
        {
            this_()
            ;
        }
    PHANTOM_END("Module")
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
