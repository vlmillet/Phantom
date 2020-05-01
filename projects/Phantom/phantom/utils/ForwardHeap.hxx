#pragma once

// haunt {

// clang-format off

#include "ForwardHeap.h"

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
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("ForwardHeap")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ForwardHeap)
        {
            this_()
        .public_()
            .constructor<void(size_t)>()
            .method<ForwardHeap&(ForwardHeap const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .constructor<void(ForwardHeap const&)>()
            .method<ForwardHeap&(ForwardHeap&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .constructor<void(ForwardHeap&&)>()
        
        .public_()
            .method<void()>("Reset", &_::Reset)
        
        .public_()
            .method<void*(size_t, size_t)>("allocate", &_::allocate)
            .method<void(void*)>("deallocate", &_::deallocate)
            ;
        }
        PHANTOM_CLASS(ForwardHeapSequence)
        {
            this_()
        .public_()
            .constructor<void(size_t)>()
            .method<void*(size_t, size_t)>("allocate", &_::allocate)
            .method<void(void*)>("deallocate", &_::deallocate)
            .method<void()>("reset", &_::reset)
            .method<void(ForwardHeapSequence&)>("swap", &_::swap)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ForwardHeap")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
