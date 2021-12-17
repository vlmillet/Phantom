#pragma once

// haunt {

// clang-format off

#include "TypeIndex.h"

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
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TypeIndex")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(TypeIndex)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(Type*)>()({"a_pType"})
            .constructor<void(TypeIndex const&)>()({"_other"})
            .method<TypeIndex&(TypeIndex const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"_other"})
            .method<bool(TypeIndex const&) const>("operator==", &_::operator==)({"_other"})
            .method<bool(TypeIndex const&) const>("operator!=", &_::operator!=)({"_other"})
            .method<bool(TypeIndex) const>("operator<", &_::operator<)({"_other"})
            .method<::phantom::lang::Type *() const>("operator ::phantom::lang::Type *", &_::operator notypedef<::phantom::lang::Type *>)
            .field("hash", &_::hash)
            .field("type", &_::type)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TypeIndex")
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
