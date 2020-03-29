#pragma once

// haunt {

// clang-format off

#include "SmallVectorBasedContainer.h"

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
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallVectorBasedContainer")

        PHANTOM_STRUCT_T((class), (t_Ty), Less)
        {
            using first_argument_type = typedef_<PHANTOM_TYPENAME _::first_argument_type>;
            using result_type = typedef_<PHANTOM_TYPENAME _::result_type>;
            using second_argument_type = typedef_<PHANTOM_TYPENAME _::second_argument_type>;
            this_()
            .PHANTOM_T typedef_<first_argument_type>("first_argument_type")
            .PHANTOM_T typedef_<second_argument_type>("second_argument_type")
            .PHANTOM_T typedef_<result_type>("result_type")
            .PHANTOM_T method<bool(const t_Ty&, const t_Ty&) const>("operator()", &_::operator())
            ;
        }
        PHANTOM_CLASS_T((class, size_t), (T, S), SmallVectorBasedContainer)
        {
            using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
            using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
            using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;
            this_()
        .public_()
            .PHANTOM_T typedef_<value_type>("value_type")
            .PHANTOM_T typedef_<iterator>("iterator")
            .PHANTOM_T typedef_<const_iterator>("const_iterator")
        
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(CustomAllocator const*), explicit_>()
            .PHANTOM_T method<iterator(const_iterator)>("erase", &_::erase)
            .PHANTOM_T method<iterator(const_iterator, const_iterator)>("erase", &_::erase)
            .PHANTOM_T method<const_iterator() const>("begin", &_::begin)
            .PHANTOM_T method<const_iterator() const>("end", &_::end)
            .PHANTOM_T method<iterator()>("begin", &_::begin)
            .PHANTOM_T method<iterator()>("end", &_::end)
            .PHANTOM_T method<T const*() const>("data", &_::data)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<T const&(size_t) const>("operator[]", &_::operator[])
        
        .protected_()
            ;
        }
    PHANTOM_END("SmallVectorBasedContainer")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
