#pragma once

// haunt {

#include "Pool.h"

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
#include <phantom/typedef>
#include <phantom/friend>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Pool")

        PHANTOM_CLASS_T((class, size_t), (T, N), Pool)
        {
            using const_pointer = typedef_<PHANTOM_TYPENAME _::const_pointer>;
            using const_reference = typedef_<PHANTOM_TYPENAME _::const_reference>;
            using difference_type = typedef_<PHANTOM_TYPENAME _::difference_type>;
            using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
            using pointer = typedef_<PHANTOM_TYPENAME _::pointer>;
            using reference = typedef_<PHANTOM_TYPENAME _::reference>;
            using size_type = typedef_<PHANTOM_TYPENAME _::size_type>;
            using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;
            this_()
            .PHANTOM_T inherits<::phantom::BasicPool>()
        .public_()
            .PHANTOM_T typedef_<value_type>("value_type")
            .PHANTOM_T typedef_<pointer>("pointer")
            .PHANTOM_T typedef_<const_pointer>("const_pointer")
            .PHANTOM_T typedef_<reference>("reference")
            .PHANTOM_T typedef_<const_reference>("const_reference")
            .PHANTOM_T typedef_<size_type>("size_type")
            .PHANTOM_T typedef_<difference_type>("difference_type")
            /// missing symbol(s) reflection (std::true_type) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<propagate_on_container_move_assignment>("propagate_on_container_move_assignment")
            /// missing symbol(s) reflection (std::true_type) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<is_always_equal>("is_always_equal")
        
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(size_t)>()["N"]
            )
            .PHANTOM_T constructor<void(Pool<T, N> const&)>()
            .PHANTOM_T method<bool(Pool<T, N> const&) const>("operator==", &_::operator==)
            .PHANTOM_T method<T*()>("New", &_::New)
            .PHANTOM_T method<void(T*)>("Delete", &_::Delete)
            .PHANTOM_T method<void(iterator)>("Delete", &_::Delete)
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<size_t() const>("capacity", &_::capacity)
            .PHANTOM_T method<bool(const T*) const>("isAllocated", &_::isAllocated)
            .PHANTOM_T method<bool(iterator) const>("isAllocated", &_::isAllocated)
            .PHANTOM_T method<T*()>("allocate", &_::allocate)
            .PHANTOM_T method<T*(size_t)>("allocate", &_::allocate)
            .PHANTOM_T method<void(T*)>("deallocate", &_::deallocate)
            .PHANTOM_T method<void(T*, size_t)>("deallocate", &_::deallocate)
            .PHANTOM_T method<iterator()>("begin", &_::begin)
            .PHANTOM_T method<iterator()>("end", &_::end)
            .PHANTOM_T method<T&(size_t, size_t)>("getItem", &_::getItem)
            .PHANTOM_T method<const T&(size_t, size_t) const>("getItem", &_::getItem)
            .PHANTOM_T method<size_t() const>("getBlockCount", &_::getBlockCount)
            .PHANTOM_T method<size_t() const>("getItemsPerBlock", &_::getItemsPerBlock)
            .PHANTOM_T class_<iterator>()
            
            .public_()
                PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
                .PHANTOM_T constructor<void()>()
                )
                .PHANTOM_T constructor<void(Pool<T, N>*, size_t, size_t)>()
                .PHANTOM_T method<T&()>("operator*", &_::iterator::operator*)
                .PHANTOM_T method<T*()>("operator->", &_::iterator::operator->)
                .PHANTOM_T method<iterator(int)>("operator++", &_::iterator::operator++)
                .PHANTOM_T method<iterator&()>("operator++", &_::iterator::operator++)
                .PHANTOM_T method<bool(const iterator&) const>("operator==", &_::iterator::operator==)
                .PHANTOM_T method<bool(const iterator&) const>("operator!=", &_::iterator::operator!=)
            .end()
            ;
        }
    PHANTOM_END("Pool")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
