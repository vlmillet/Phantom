#pragma once

// haunt {

// clang-format off

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

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Plugin")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Plugin)
        {
            using Plugins = typedef_< phantom::lang::Plugins>;
            using StringView = typedef_< phantom::StringView>;
            this_()
        
        .public_()
            .staticMethod<bool()>("HasLoadingInProgress", &_::HasLoadingInProgress)
            .staticMethod<bool(Module*)>("HasLoadingInProgress", &_::HasLoadingInProgress)({"a_pModule"})
            .staticMethod<bool()>("HasUnloadingInProgress", &_::HasUnloadingInProgress)
            .staticMethod<const Plugins&()>("GetLoadingPluginStack", &_::GetLoadingPluginStack)
            .staticMethod<const Plugins&()>("GetUnloadingPluginStack", &_::GetUnloadingPluginStack)
        
        .public_()
            .constructor<void(StringView, StringView)>()({"a_strName","a_strFilePath"})
            .constructor<void(StringView)>()({"a_strFilePath"})
            .method<StringView() const>("getFilePath", &_::getFilePath)
            .method<bool()>("load", &_::load)
            .method<bool()>("unload", &_::unload)
            .method<bool()>("loadAsDependency", &_::loadAsDependency)
            .method<bool()>("unloadAsDependency", &_::unloadAsDependency)
            .method<bool() const>("isLoaded", &_::isLoaded)
            .method<void()>("deleteOnDisk", &_::deleteOnDisk)
            .method<Module*() const>("getModule", &_::getModule)
            .method<StringView() const>("getName", &_::getName)
            .method<void(StringView)>("setName", &_::setName)({"a_Name"})
            .method<StringView() const>("getDebugPath", &_::getDebugPath)
            .method<void(StringView)>("setDebugPath", &_::setDebugPath)({"a_In"})
            .method<StringView() const>("getReleasePath", &_::getReleasePath)
            .method<void(StringView)>("setReleasePath", &_::setReleasePath)({"a_In"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Plugin")
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
