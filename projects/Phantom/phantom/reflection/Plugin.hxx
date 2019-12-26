#pragma once

// haunt {

#include "Plugin.h"

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

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Plugin")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Plugin)
        {
            using Plugins = typedef_< phantom::reflection::Plugins>;
            using StringView = typedef_< phantom::StringView>;
            using Strings = typedef_< phantom::Strings>;
            this_()
        
        .public_()
            .staticMethod<bool()>("HasLoadingInProgress", &_::HasLoadingInProgress)
            .staticMethod<bool(Module*)>("HasLoadingInProgress", &_::HasLoadingInProgress)
            .staticMethod<bool()>("HasUnloadingInProgress", &_::HasUnloadingInProgress)
            .staticMethod<Plugins const&()>("GetLoadingPluginStack", &_::GetLoadingPluginStack)
            .staticMethod<Plugins const&()>("GetUnloadingPluginStack", &_::GetUnloadingPluginStack)
        
        .public_()
            .constructor<void(StringView, StringView)>()
            .constructor<void(StringView)>()
            .method<StringView() const>("getFilePath", &_::getFilePath)
            .method<bool(Message*)>("load", &_::load)["nullptr"]
            .method<bool(Message*)>("unload", &_::unload)["nullptr"]
            .method<bool(Message*)>("loadAsDependency", &_::loadAsDependency)["nullptr"]
            .method<bool(Message*)>("unloadAsDependency", &_::unloadAsDependency)["nullptr"]
            .method<bool() const>("isLoaded", &_::isLoaded)
            .method<void()>("deleteOnDisk", &_::deleteOnDisk)
            .method<Module*() const>("getModule", &_::getModule)
            .method<StringView() const>("getName", &_::getName)
            .method<void(StringView)>("setName", &_::setName)
            .method<StringView() const>("getIncludePaths", &_::getIncludePaths)
            .method<void(StringView)>("setIncludePaths", &_::setIncludePaths)
            .method<StringView() const>("getDebugPath", &_::getDebugPath)
            .method<void(StringView)>("setDebugPath", &_::setDebugPath)
            .method<StringView() const>("getReleasePath", &_::getReleasePath)
            .method<void(StringView)>("setReleasePath", &_::setReleasePath)
            .method<Strings const&() const>("getDependencies", &_::getDependencies)
            .method<void(Strings const&)>("setDependencies", &_::setDependencies)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Plugin")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
