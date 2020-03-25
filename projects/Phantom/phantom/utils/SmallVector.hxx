#pragma once

#include "SmallVector.h"

#include <phantom/lang/VectorClassT.h>
#include <phantom/meta_type>

PHANTOM_META_TYPE_T((class, size_t, size_t), (T, StaticAllocSize, DynamicAllocInc), phantom::SmallVector,
                      phantom::lang::VectorClassT);

// haunt {

// clang-format off

#include "SmallVector.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include "ArrayView.hxx"
#include <std/initializer_list.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallVector")

        PHANTOM_CLASS_T((class, size_t, size_t), (T, StaticAllocSize, DynamicAllocInc), SmallVector)
        {
            using ArrayViewType = typedef_<PHANTOM_TYPENAME _::ArrayViewType>;
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
            using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
            using size_type = typedef_<PHANTOM_TYPENAME _::size_type>;
            using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;
            this_()
        
        .public_()
            .PHANTOM_T typedef_<SelfType>("SelfType")
            .PHANTOM_T typedef_<size_type>("size_type")
            .PHANTOM_T typedef_<value_type>("value_type")
            .PHANTOM_T typedef_<const_iterator>("const_iterator")
            .PHANTOM_T typedef_<iterator>("iterator")
            /// missing symbol(s) reflection (std::reverse_iterator) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<const_reverse_iterator>("const_reverse_iterator")
            /// missing symbol(s) reflection (std::reverse_iterator) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<reverse_iterator>("reverse_iterator")
            .PHANTOM_T typedef_<ArrayViewType>("ArrayViewType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(size_t), explicit_>()
            .PHANTOM_T constructor<void(CustomAllocator const*), explicit_>()
            .PHANTOM_T constructor<void(const ::std::initializer_list<value_type> &)>()
            .PHANTOM_T constructor<void(ArrayViewType)>()
            .PHANTOM_T constructor<void(const SelfType&)>()
            .PHANTOM_T constructor<void(iterator, iterator)>()
            .PHANTOM_T constructor<void(const_iterator, const_iterator)>()
            .PHANTOM_T constructor<void(SelfType&&)>()
            .PHANTOM_T method<SelfType&(const SelfType&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(SelfType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<value_type*()>("data", &_::data)
            .PHANTOM_T method<value_type const*() const>("data", &_::data)
            .PHANTOM_T method<const_iterator() const>("begin", &_::begin)
            .PHANTOM_T method<const_iterator() const>("end", &_::end)
            .PHANTOM_T method<iterator()>("begin", &_::begin)
            .PHANTOM_T method<iterator()>("end", &_::end)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<const_reverse_iterator() const>("rbegin", &_::rbegin)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<const_reverse_iterator() const>("rend", &_::rend)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<reverse_iterator()>("rbegin", &_::rbegin)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<reverse_iterator()>("rend", &_::rend)
            .PHANTOM_T method<value_type&(size_type)>("operator[]", &_::operator[])
            .PHANTOM_T method<value_type const&(size_type) const>("operator[]", &_::operator[])
            .PHANTOM_T method<size_type() const>("size", &_::size)
            .PHANTOM_T method<size_type() const>("capacity", &_::capacity)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<value_type const&() const>("front", &_::front)
            .PHANTOM_T method<value_type const&() const>("back", &_::back)
            .PHANTOM_T method<value_type&()>("front", &_::front)
            .PHANTOM_T method<value_type&()>("back", &_::back)
            .PHANTOM_T method<void()>("pop_back", &_::pop_back)
            .PHANTOM_T method<void(size_t)>("reserve", &_::reserve)
            .PHANTOM_T method<void(size_t)>("resize", &_::resize)
            .PHANTOM_T method<void(size_t, const value_type&)>("resize", &_::resize)
            .PHANTOM_T method<void(size_t, value_type&&)>("resize", &_::resize)
            .PHANTOM_T method<void(const value_type&)>("push_back", &_::push_back)
            .PHANTOM_T method<iterator(const_iterator)>("erase", &_::erase)
            .PHANTOM_T method<iterator(const_iterator)>("erase_unsorted", &_::erase_unsorted)
            .PHANTOM_T method<iterator(const_iterator, const_iterator)>("erase_unsorted", &_::erase_unsorted)
            .PHANTOM_T method<iterator(const_iterator, const_iterator)>("erase", &_::erase)
            .PHANTOM_T method<iterator(iterator, iterator, iterator)>("insert", &_::insert)
            .PHANTOM_T method<iterator(const_iterator, iterator, iterator)>("insert", &_::insert)
            .PHANTOM_T method<iterator(const_iterator, const_iterator, const_iterator)>("insert", &_::insert)
            .PHANTOM_T method<iterator(iterator, const_iterator, const_iterator)>("insert", &_::insert)
            .PHANTOM_T method<iterator(iterator, const value_type&)>("insert", &_::insert)
            .PHANTOM_T method<ArrayViewType() const>("operator ArrayViewType", &_::operator notypedef<ArrayViewType>)
            .PHANTOM_T method<iterator(iterator, value_type&&)>("insert", &_::insert)
            .PHANTOM_T method<void(value_type&&)>("push_back", &_::push_back)
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<void(SelfType&)>("swap", &_::swap)
            ;
        }
    PHANTOM_END("SmallVector")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallVector")

        /// missing symbol(s) reflection (phantom::IsCopyAssignable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, size_t, size_t), (T, S, D), (), IsCopyAssignable)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsCopyAssignable<T>::value)>>()
            ;
        } */
    PHANTOM_END("SmallVector")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallVector")

        /// missing symbol(s) reflection (phantom::IsMoveAssignable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, size_t, size_t), (T, S, D), (), IsMoveAssignable)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsMoveAssignable<T>::value)>>()
            ;
        } */
    PHANTOM_END("SmallVector")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallVector")

        /// missing symbol(s) reflection (phantom::IsCopyConstructible) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, size_t, size_t), (T, S, D), (), IsCopyConstructible)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsCopyConstructible<T>::value)>>()
            ;
        } */
    PHANTOM_END("SmallVector")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallVector")

        /// missing symbol(s) reflection (phantom::IsMoveConstructible) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, size_t, size_t), (T, S, D), (), IsMoveConstructible)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsMoveConstructible<T>::value)>>()
            ;
        } */
    PHANTOM_END("SmallVector")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
