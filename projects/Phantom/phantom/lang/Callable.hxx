#pragma once

// haunt {

// clang-format off

#include "Callable.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Callable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Callable)
        {
            this_()
        .public_()
            .method<void(void**) const, pure_virtual>("call", &_::call)
            .method<void(void**, void*) const, pure_virtual>("call", &_::call)
            .method<void(void*, void**) const, pure_virtual>("invoke", &_::invoke)
            .method<void(void*, void**, void*) const, pure_virtual>("invoke", &_::invoke)
            .method<uint64_t() const, pure_virtual>("getUniqueID", &_::getUniqueID)
            .method<void(void**, SmallVector<Variant, 10>&) const, pure_virtual>("saveArgs", &_::saveArgs)
            .method<void(), virtual_>("onDisconnect", &_::onDisconnect)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Callable")
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
