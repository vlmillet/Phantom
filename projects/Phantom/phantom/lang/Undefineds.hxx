#pragma once

// haunt {

// clang-format off

#include "Undefineds.h"

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
#include <phantom/constructor>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Undefineds")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Undefineds)
        {
            this_()
        .public_()
            .constructor<void()>()
            .method<Type*()>("makePointer", &_::makePointer)
            .method<Type*()>("makeLValueReference", &_::makeLValueReference)
            .method<Type*()>("makeRValueReference", &_::makeRValueReference)
            .method<Type*()>("makeConst", &_::makeConst)
            .method<Type*()>("makeVolatile", &_::makeVolatile)
            .method<Type*()>("makeConstVolatile", &_::makeConstVolatile)
            .method<Type*(size_t)>("makeArray", &_::makeArray)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Undefineds")
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
