#pragma once

// haunt {

// clang-format off

#include "CustomAllocator.h"

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
#include <phantom/static_method>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "utils/Delegate.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("CustomAllocator")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(CustomAllocator)
        {
            using AllocFunc = typedef_<_::AllocFunc>;
            using DeallocFunc = typedef_<_::DeallocFunc>;
            using ReallocFunc = typedef_<_::ReallocFunc>;
            this_()
            .staticMethod<void(CustomAllocator)>("Push", &_::Push)
            .staticMethod<void()>("Pop", &_::Pop)
            .staticMethod<CustomAllocator const&()>("CurrentOrDefault", &_::CurrentOrDefault)
            .staticMethod<CustomAllocator const&()>("Current", &_::Current)
            .staticMethod<CustomAllocator const&()>("Default", &_::Default)
            .staticMethod<void()>("Init", &_::Init)
            .staticMethod<void()>("Release", &_::Release)
            .typedef_<AllocFunc>("AllocFunc")
            .typedef_<ReallocFunc>("ReallocFunc")
            .typedef_<DeallocFunc>("DeallocFunc")
            .field("allocFunc", &_::allocFunc)
            .field("deallocFunc", &_::deallocFunc)
            .field("reallocFunc", &_::reallocFunc)
            .method<bool(CustomAllocator const&) const>("operator==", &_::operator==)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CustomAllocator")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
