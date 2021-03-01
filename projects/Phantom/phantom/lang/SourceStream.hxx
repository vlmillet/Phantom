#pragma once

// haunt {

// clang-format off

#include "SourceStream.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("SourceStream")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(SourceStream)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
        .public_()
            .staticMethod<::phantom::lang::SourceStream *(StringView)>("CreateFromUrl", &_::CreateFromUrl)
        
        .public_()
            .method<::phantom::lang::SourceFile *(), virtual_>("asFile", &_::asFile)
            .method<bool() const, pure_virtual>("exists", &_::exists)
            /// missing symbol(s) reflection (std::basic_istream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::std::basic_istream<char> *(), pure_virtual>("createInputStream", &_::createInputStream)
            /// missing symbol(s) reflection (std::basic_istream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::basic_istream<char> *), pure_virtual>("destroyInputStream", &_::destroyInputStream)
            /// missing symbol(s) reflection (time_t) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<time_t() const, pure_virtual>("getLastChangeTime", &_::getLastChangeTime)
            .method<StringView() const>("getPath", &_::getPath)
            .method<StringView() const>("getProtocol", &_::getProtocol)
            .method<String() const>("getUrl", &_::getUrl)
            .method<::phantom::lang::SourceStream *() const, pure_virtual>("clone", &_::clone)
        
        .protected_()
            .constructor<void(StringView, StringView)>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("SourceStream")
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
