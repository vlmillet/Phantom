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
            .constructor<void(size_t)>()({"_heapSize"})
            .method<ForwardHeap&(ForwardHeap const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({""})
            .constructor<void(ForwardHeap const&)>()({""})
            .method<ForwardHeap&(ForwardHeap&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({""})
            .constructor<void(ForwardHeap&&)>()({"_temp"})
        
        .public_()
            .method<void()>("reset", &_::reset)
        
        .public_()
            .method<void*(size_t, size_t)>("allocate", &_::allocate)({"_size","_align"})
            .method<void(void*)>("deallocate", &_::deallocate)({"_ptr"})
            ;
        }
        PHANTOM_CLASS(ForwardHeapSequence)
        {
            this_()
        .public_()
            .constructor<void(size_t)>()({"a_HeapSize"})
            .method<void*(size_t, size_t)>("allocate", &_::allocate)({"_s","_a"})
            .method<void(void*)>("deallocate", &_::deallocate)({"_ptr"})
            .method<void()>("reset", &_::reset)
            .method<void(ForwardHeapSequence&)>("swap", &_::swap)({"a_Other"})
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
