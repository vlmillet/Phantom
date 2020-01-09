#pragma once

// haunt {

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
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("TypeIndex")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(TypeIndex)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(Type*)>()
            .constructor<void(TypeIndex const&)>()
            .method<TypeIndex&(TypeIndex const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(TypeIndex const&) const>("operator==", &_::operator==)
            .method<bool(TypeIndex const&) const>("operator!=", &_::operator!=)
            .method<bool(TypeIndex) const>("operator<", &_::operator<)
            .method<Type*() const>("operator Type*", &_::operator notypedef<Type*>)
            .field("hash", &_::hash)
            .field("type", &_::type)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TypeIndex")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
