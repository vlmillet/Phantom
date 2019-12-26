#pragma once

// haunt {

#include "BasicPool.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>

#include <phantom/template-only-push>

#include "SmallVector.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("BasicPool")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(BasicPool)
        {
            using FreeItem = typedef_<_::FreeItem>;
            this_()
        .public_()
            .constructor<void(size_t, size_t)>()
            .method<uint8_t*()>("allocate", &_::allocate)
            .method<void(uint8_t*)>("deallocate", &_::deallocate)
            .method<void()>("clear", &_::clear)
            .method<size_t() const>("size", &_::size)
            .method<size_t() const>("capacity", &_::capacity)
            .method<size_t() const>("getItemSize", &_::getItemSize)
            .method<size_t() const>("getItemsPerBlock", &_::getItemsPerBlock)
            .method<bool(const uint8_t*) const>("isAllocated", &_::isAllocated)
            .method<size_t() const>("getBlockCount", &_::getBlockCount)
            .method<uint8_t*(size_t, size_t)>("getItem", &_::getItem)
            .method<const uint8_t*(size_t, size_t) const>("getItem", &_::getItem)
            .method<bool(size_t, size_t) const>("isAllocated", &_::isAllocated)
            .method<void(size_t)>("setItemsPerBlock", &_::setItemsPerBlock)
        
        .protected_()
            .method<void()>("addBlock", &_::addBlock)
            .method<void(size_t, size_t)>("deallocate", &_::deallocate)
            .method<bool(const uint8_t*, size_t&, size_t&) const>("getItemIdx", &_::getItemIdx)
            .method<void(const uint8_t*, bool)>("setAllocBit", &_::setAllocBit)
            .method<void(size_t, size_t, bool)>("setAllocBit", &_::setAllocBit)
            .method<size_t() const>("getAllocationBytesPerBlock", &_::getAllocationBytesPerBlock)
            .struct_<FreeItem>()
                .field("m_next", &_::FreeItem::m_next)
            .end()
            .field("m_blocks", &_::m_blocks)
            .field("m_allocatedItems", &_::m_allocatedItems)
            .field("m_itemSize", &_::m_itemSize)
            .field("m_itemsPerBlock", &_::m_itemsPerBlock)
            .field("m_nextFreeItem", &_::m_nextFreeItem)
            .field("m_freeItemCount", &_::m_freeItemCount)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("BasicPool")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
