#pragma once

// haunt {

// clang-format off

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
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("SourceFile")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SourceFile)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::SourceStream>()
        
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
            /// missing symbol(s) reflection (std::basic_istream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<std::basic_istream<char>*(), virtual_|override_>("createInputStream", &_::createInputStream)
            /// missing symbol(s) reflection (std::basic_istream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::basic_istream<char> *), virtual_|override_>("destroyInputStream", &_::destroyInputStream)
            .method<SourceFile*() const, virtual_|override_>("clone", &_::clone)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SourceFile")
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

