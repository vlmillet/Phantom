#pragma once

// haunt {

#include "StringWithHash.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("StringWithHash")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(StringWithHash)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .constructor<void(), default_>()
            .constructor<void(StringHash)>()
            .constructor<void(String)>()
            .constructor<void(StringView)>()
            .constructor<void(const char*)>()
            .field("hash", &_::hash)
            .field("str", &_::str)
            .method<bool(StringWithHash const&) const>("operator<", &_::operator<)
            .method<StringHash() const>("operator StringHash", &_::operator notypedef<StringHash>)
            .method<String() const, explicit_>("operator String", &_::operator notypedef<String>)
            .method<StringView() const, explicit_>("operator StringView", &_::operator notypedef<StringView>)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringWithHash")
PHANTOM_END("phantom")
}
namespace std {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("StringWithHash")

        /// missing symbol(s) reflection (std::hash) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_S((phantom::StringWithHash), hash)
        {
            this_()
        .public_()
            // .method<phantom::size_t(::phantom::StringWithHash) const>("operator()", &_::operator())
            ;
        } */
    PHANTOM_END("StringWithHash")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
