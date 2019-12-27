#pragma once

// haunt {

#include "SourceFile.h"

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("SourceFile")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SourceFile)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::reflection::SourceStream>()
        
        .public_()
            .staticMethod<SourceFile*(StringView, bool)>("CreateOnDisk", &_::CreateOnDisk)
            .staticMethod<SourceFile*(Source*, StringView, bool)>("CreateOnDisk", &_::CreateOnDisk)
        
        .public_()
            .constructor<void(StringView)>()
            .method<bool()>("deleteFile", &_::deleteFile)
            .method<void(String&)>("read", &_::read)
            .method<void(StringView)>("write", &_::write)
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<time_t() const, virtual_|override_>("getLastChangeTime", &_::getLastChangeTime)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int *(), override_>("createInputStream", &_::createInputStream)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(int *), override_>("destroyInputStream", &_::destroyInputStream)
            .method<SourceFile*() const, virtual_|override_>("clone", &_::clone)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SourceFile")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
