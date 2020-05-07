#pragma once

// haunt {

// clang-format off

#include "Application.h"

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
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Delegate.hxx>
#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Application")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Application)
        {
            using Classes = typedef_< phantom::lang::Classes>;
            using LoadedLibraries = typedef_<_::LoadedLibraries>;
            using LoadedLibraryModules = typedef_<_::LoadedLibraryModules>;
            using Modules = typedef_< phantom::lang::Modules>;
            using Plugins = typedef_< phantom::lang::Plugins>;
            using Sources = typedef_< phantom::lang::Sources>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Strings = typedef_< phantom::Strings>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            using Types = typedef_< phantom::lang::Types>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<LoadedLibraryModules>("LoadedLibraryModules")
            .typedef_<LoadedLibraries>("LoadedLibraries")
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .typedef_<CppExpressionParser>("CppExpressionParser")
        
        .public_()
            .staticMethod<Application*()>("Get", &_::Get)
        
        .public_()
            .constructor<void()>()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_>("terminate", &_::terminate)
            .method<Source*() const>("getDefaultSource", &_::getDefaultSource)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(CppExpressionParser)>("setCppExpressionParser", &_::setCppExpressionParser)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<CppExpressionParser()>("getCppExpressionParser", &_::getCppExpressionParser)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(StringView, LanguageElement*) const>("cppExpression", &_::cppExpression)
            .method<bool(StringView, Symbols&, StringBuffer*)>("findCppSymbols", &_::findCppSymbols)["nullptr"]
            .method<bool(StringView, Symbols&, LanguageElement*, StringBuffer*)>("findCppSymbols", &_::findCppSymbols)["nullptr"]
            .method<Symbol*(StringView, LanguageElement*, StringBuffer*)>("findCppSymbol", &_::findCppSymbol)["nullptr"]
            .method<Type*(StringView, LanguageElement*, StringBuffer*)>("findCppType", &_::findCppType)["nullptr"]
            .method<Template*(StringView, LanguageElement*, StringBuffer*)>("findCppTemplate", &_::findCppTemplate)["nullptr"]
            .method<Template*(StringView, StringBuffer*)>("findCppTemplate", &_::findCppTemplate)["nullptr"]
            .method<Class*(StringView, LanguageElement*, StringBuffer*)>("findCppClass", &_::findCppClass)["nullptr"]
            .method<Symbol*(StringView, StringBuffer*)>("findCppSymbol", &_::findCppSymbol)["nullptr"]
            .method<Type*(StringView, StringBuffer*)>("findCppType", &_::findCppType)["nullptr"]
            .method<Class*(StringView, StringBuffer*)>("findCppClass", &_::findCppClass)["nullptr"]
            .method<Symbol*(StringView, LanguageElement*, StringBuffer*)>("findCppSymbolCached", &_::findCppSymbolCached)["nullptr"]
            .method<Type*(StringView, LanguageElement*, StringBuffer*)>("findCppTypeCached", &_::findCppTypeCached)["nullptr"]
            .method<Template*(StringView, LanguageElement*, StringBuffer*)>("findCppTemplateCached", &_::findCppTemplateCached)["nullptr"]
            .method<Template*(StringView, StringBuffer*)>("findCppTemplateCached", &_::findCppTemplateCached)["nullptr"]
            .method<Class*(StringView, LanguageElement*, StringBuffer*)>("findCppClassCached", &_::findCppClassCached)["nullptr"]
            .method<Symbol*(StringView, StringBuffer*)>("findCppSymbolCached", &_::findCppSymbolCached)["nullptr"]
            .method<Type*(StringView, StringBuffer*)>("findCppTypeCached", &_::findCppTypeCached)["nullptr"]
            .method<Class*(StringView, StringBuffer*)>("findCppClassCached", &_::findCppClassCached)["nullptr"]
            .method<Module*() const>("getMainModule", &_::getMainModule)
            .method<Module*(StringView)>("getOrCreateModule", &_::getOrCreateModule)
            .method<Modules() const>("getNativeModules", &_::getNativeModules)
            .method<Module*(StringView) const>("getSharedLibraryModule", &_::getSharedLibraryModule)
            .method<Module*(const byte*, uint, byte*)>("nativeModuleFromStackContext", &_::nativeModuleFromStackContext)["1"]["nullptr"]
            .method<Module*(const byte*)>("nativeModuleFromProgramCounter", &_::nativeModuleFromProgramCounter)
            .method<bool(StringView)>("setDefaultExportPath", &_::setDefaultExportPath)
            .method<StringView() const>("getDefaultExportPath", &_::getDefaultExportPath)
            .method<bool(StringView)>("setDefaultPluginPath", &_::setDefaultPluginPath)
            .method<StringView() const>("getDefaultPluginPath", &_::getDefaultPluginPath)
            .method<bool(StringView)>("addPluginPath", &_::addPluginPath)
            .method<bool(StringView)>("removePluginPath", &_::removePluginPath)
            .method<bool(StringView) const>("hasPluginPath", &_::hasPluginPath)
            .method<bool(StringView)>("setDefaultBinaryPath", &_::setDefaultBinaryPath)
            .method<StringView() const>("getDefaultBinaryPath", &_::getDefaultBinaryPath)
            .method<bool(StringView)>("addBinaryPath", &_::addBinaryPath)
            .method<bool(StringView)>("removeBinaryPath", &_::removeBinaryPath)
            .method<bool(StringView) const>("hasBinaryPath", &_::hasBinaryPath)
            .method<Type*(StringView) const>("getBuiltInType", &_::getBuiltInType)
            .method<Types const&() const>("getBuiltInTypes", &_::getBuiltInTypes)
            .method<Module*(StringView) const>("getModule", &_::getModule)
            .method<Modules const&() const>("getModules", &_::getModules)
            .method<Modules const&() const>("getStartupModules", &_::getStartupModules)
            .method<Module*(size_t) const>("getModuleByHandle", &_::getModuleByHandle)
            .method<Plugin*(StringView) const>("getPlugin", &_::getPlugin)
            .method<Plugins const&() const>("getPlugins", &_::getPlugins)
            .method<void(Plugin*)>("addPlugin", &_::addPlugin)
            .method<void(Plugin*)>("removePlugin", &_::removePlugin)
            .method<Module*(StringView)>("newModule", &_::newModule)
            .method<void(Module*)>("deleteModule", &_::deleteModule)
            .method<void(Module*)>("addModule", &_::addModule)
            .method<void(Module*)>("removeModule", &_::removeModule)
            .method<void(Sources&) const>("getSources", &_::getSources)
            .method<const Strings&() const>("getSourcePaths", &_::getSourcePaths)
            .method<StringView() const>("getDefaultSourcePath", &_::getDefaultSourcePath)
            .method<bool(StringView)>("setDefaultSourcePath", &_::setDefaultSourcePath)
            .method<bool(StringView)>("addSourcePath", &_::addSourcePath)
            .method<bool(StringView)>("removeSourcePath", &_::removeSourcePath)
            .method<PackageFolder*() const>("rootPackageFolder", &_::rootPackageFolder)
            .method<Source*(StringView) const>("getSource", &_::getSource)
            .method<PackageFolder*(StringView) const>("getPackageFolder", &_::getPackageFolder)
            .method<PackageFolder*(StringView)>("getOrCreatePackageFolder", &_::getOrCreatePackageFolder)
            .method<String(StringView) const>("findPluginPath", &_::findPluginPath)
            .method<void(StringView, Strings&) const>("findPlugins", &_::findPlugins)
            .method<Strings const&() const>("getPluginPaths", &_::getPluginPaths)
            .method<Type*(hash64) const>("findType", &_::findType)
            .method<Symbol*(StringView, LanguageElement*) const>("findSymbol", &_::findSymbol)["nullptr"]
            .method<Type*(StringView, LanguageElement*) const>("findType", &_::findType)["nullptr"]
            .method<void(Classes&, ::phantom::lang::Class *, bool) const>("findClasses", &_::findClasses)["nullptr"]["false"]
            .method<Undefineds*(StringView) const>("getUndefineds", &_::getUndefineds)
            .method<Undefineds*(StringView)>("undefineds", &_::undefineds)
            .method<Constant*() const>("getNullptr", &_::getNullptr)
            .method<Type*() const>("getAuto", &_::getAuto)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            /// missing symbol(s) reflection (phantom::MemoryContext) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<MemoryContext&()>("getMemoryContext", &_::getMemoryContext)
            .method<CustomAllocator const*() const, virtual_|override_>("getAllocator", &_::getAllocator)
        
        .public_()
            .field("pluginPathAdded", &_::pluginPathAdded)
            .field("pluginPathRemoved", &_::pluginPathRemoved)
            .field("binaryPathAdded", &_::binaryPathAdded)
            .field("binaryPathRemoved", &_::binaryPathRemoved)
            .field("moduleAdded", &_::moduleAdded)
            .field("moduleDiscarded", &_::moduleDiscarded)
            .field("moduleAboutToBeRemoved", &_::moduleAboutToBeRemoved)
            .field("moduleUnloaded", &_::moduleUnloaded)
            .field("pluginAdded", &_::pluginAdded)
            .field("pluginAboutToBeRemoved", &_::pluginAboutToBeRemoved)
            .field("pluginLoaded", &_::pluginLoaded)
            .field("pluginLoadingFailed", &_::pluginLoadingFailed)
            .field("pluginAboutToBeUnloaded", &_::pluginAboutToBeUnloaded)
            .field("sourceAdded", &_::sourceAdded)
            .field("sourceAboutToBeRemoved", &_::sourceAboutToBeRemoved)
            .field("packageAdded", &_::packageAdded)
            .field("packageAboutToBeRemoved", &_::packageAboutToBeRemoved)
            .field("sourceDeleted", &_::sourceDeleted)
        
        .protected_()
            .method<hash64() const, virtual_|override_>("computeHash", &_::computeHash)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Application")
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
