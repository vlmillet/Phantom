#pragma once

// haunt {

// clang-format off

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

#include <phantom/template-only-push>

#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace std {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("StringWithHash")

        /// missing symbol(s) reflection (std::hash) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_S((phantom::StringWithHash), hash)
        {
            this_()
            // .method<size_t(const ::phantom::StringWithHash &)>("operator()", &_::operator())
            ;
        } */
    PHANTOM_END("StringWithHash")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("StringWithHash")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(StringWithHash)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()
        .public_()
            .constructor<void(), default_>()
            .constructor<void(StringWithHash const&)>()
            .method<StringWithHash&(StringWithHash const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .constructor<void(StringView), explicit_>()
            .method<uint64_t() const, explicit_>("operator uint64_t", &_::operator notypedef<uint64_t>)
            .method<StringHash() const>("getHash", &_::getHash)
            .method<uint64_t() const>("getHashValue", &_::getHashValue)
            .method<StringView() const>("getString", &_::getString)
            .method<bool(StringWithHash const&) const>("operator<", &_::operator<)
            .method<bool(StringWithHash const&) const>("operator==", &_::operator==)
            .method<bool(StringWithHash const&) const>("operator!=", &_::operator!=)
            .method<bool() const>("isNull", &_::isNull)
            .method<bool() const>("isValid", &_::isValid)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringWithHash")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
