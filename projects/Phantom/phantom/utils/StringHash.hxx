#pragma once

// haunt {

#include "StringHash.h"

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
#include <phantom/function>
#include <phantom/method>
#include <phantom/constructor>

#include <phantom/template-only-push>

#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("StringHash")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<uint64_t(StringView)>("makeStringHash", makeStringHash);}
        PHANTOM_STRUCT(StringHash)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()
            .constructor<void(), default_>()
            .constructor<void(StringView), explicit_>()
            .constructor<void(uint64_t), explicit_>()
            .method<uint64_t() const, explicit_>("operator uint64_t", &_::operator notypedef<uint64_t>)
            .method<uint64_t() const>("get", &_::get)
            .method<StringView() const>("debugString", &_::debugString)
            .method<bool(StringHash const&) const>("operator<", &_::operator<)
            .method<bool(StringHash const&) const>("operator==", &_::operator==)
            .method<bool(StringHash const&) const>("operator!=", &_::operator!=)
            .method<bool() const>("IsNull", &_::IsNull)
            .method<bool() const>("IsValid", &_::IsValid)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringHash")
PHANTOM_END("phantom.utils")
}
namespace std {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("StringHash")

        /// missing symbol(s) reflection (std::hash) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_S((phantom::StringHash), hash)
        {
            this_()
            // .method<size_t(const ::phantom::StringHash &)>("operator()", &_::operator())
            ;
        } */
    PHANTOM_END("StringHash")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
