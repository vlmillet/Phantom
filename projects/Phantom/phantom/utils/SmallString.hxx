#pragma once

#include "SmallString.h"

#include <phantom/lang/StringClassT.h>
#include <phantom/meta_type>

PHANTOM_META_TYPE_T((typename, size_t), (_Elem, S), phantom::SmallString, phantom::lang::StringClassT);

// haunt {

// clang-format off

#include "SmallString.h"

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
#include <phantom/static_constant>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include "SmallVector.hxx"
#include "StringView.hxx"
#include <std/initializer_list.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallString")

        PHANTOM_CLASS_T((class, phantom::uint), (T, StaticAllocSize), SmallString)
        {
            using CharT = typedef_<PHANTOM_TYPENAME _::CharT>;
            using ContainerType = typedef_<PHANTOM_TYPENAME _::ContainerType>;
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            using StringViewType = typedef_<PHANTOM_TYPENAME _::StringViewType>;
            using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
            using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
            using size_type = typedef_<PHANTOM_TYPENAME _::size_type>;
            using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;
            this_()
        
        .public_()
            .PHANTOM_T constant("npos", _::npos)
            .PHANTOM_T typedef_<StringViewType>("StringViewType")
            .PHANTOM_T typedef_<size_type>("size_type")
            .PHANTOM_T typedef_<SelfType>("SelfType")
            .PHANTOM_T typedef_<ContainerType>("ContainerType")
            .PHANTOM_T typedef_<value_type>("value_type")
            .PHANTOM_T typedef_<CharT>("CharT")
            .PHANTOM_T typedef_<const_iterator>("const_iterator")
            .PHANTOM_T typedef_<iterator>("iterator")
            /// missing symbol(s) reflection (std::reverse_iterator) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<const_reverse_iterator>("const_reverse_iterator")
            /// missing symbol(s) reflection (std::reverse_iterator) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T typedef_<reverse_iterator>("reverse_iterator")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(SelfType const&), default_>()({"a_Other"})
            .PHANTOM_T constructor<void(SelfType&&)>()({"a_Temp"})
            .PHANTOM_T constructor<void(::std::initializer_list<CharT>)>()({"a_IL"})
            .PHANTOM_T constructor<void(CharT), explicit_>()({"c"})
            .PHANTOM_T constructor<void(CharT const*, size_t)>()({"a_pCstr","a_Len"})
            .PHANTOM_T constructor<void(StringViewType)>()({"a_View"})
            .PHANTOM_T constructor<void(CharT const*)>()({"a_pCstr"})
            .PHANTOM_T method<SelfType&(SelfType const&), default_>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Other"})
            .PHANTOM_T method<SelfType&(SelfType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Other"})
            .PHANTOM_T method<SelfType&(CharT const*)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Other"})
            .PHANTOM_T method<SelfType&(CharT)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Ch"})
            .PHANTOM_T method<StringViewType() const>("operator StringViewType", &_::operator notypedef<StringViewType>)
            .PHANTOM_T method<CharT const*() const>("begin", &_::begin)
            .PHANTOM_T method<CharT const*() const>("end", &_::end)
            .PHANTOM_T method<CharT*()>("begin", &_::begin)
            .PHANTOM_T method<CharT*()>("end", &_::end)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<const_reverse_iterator() const>("rbegin", &_::rbegin)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<const_reverse_iterator() const>("rend", &_::rend)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<reverse_iterator()>("rbegin", &_::rbegin)
            /// missing symbol(s) reflection () -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<reverse_iterator()>("rend", &_::rend)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<size_t() const>("length", &_::length)
            .PHANTOM_T method<void(size_t)>("resize", &_::resize)({"s"})
            .PHANTOM_T method<void(size_t, CharT)>("resize", &_::resize)({"s","c"})
            .PHANTOM_T method<size_t() const>("capacity", &_::capacity)
            .PHANTOM_T method<void(size_t)>("reserve", &_::reserve)({"s"})
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<CharT const&(size_t) const>("operator[]", &_::operator[])({"i"})
            .PHANTOM_T method<CharT&(size_t)>("operator[]", &_::operator[])({"i"})
            .PHANTOM_T method<CharT const&() const>("back", &_::back)
            .PHANTOM_T method<CharT&()>("back", &_::back)
            .PHANTOM_T method<CharT const&() const>("front", &_::front)
            .PHANTOM_T method<CharT&()>("front", &_::front)
            .PHANTOM_T method<SelfType&(CharT const*)>("operator+=", &_::operator+=)({"a_pCstr"})
            .PHANTOM_T method<SelfType&(SelfType const&)>("operator+=", &_::operator+=)({"a_Other"})
            .PHANTOM_T method<SelfType&(CharT)>("operator+=", &_::operator+=)({"c"})
            .PHANTOM_T method<SelfType&(StringViewType)>("operator+=", &_::operator+=)({"v"})
            .PHANTOM_T method<SelfType(CharT const*) const>("operator+", &_::operator+)({"a_pCstr"})
            .PHANTOM_T method<SelfType(SelfType const&) const>("operator+", &_::operator+)({"a_Other"})
            .PHANTOM_T method<SelfType(CharT) const>("operator+", &_::operator+)({"c"})
            .PHANTOM_T method<SelfType(StringViewType) const>("operator+", &_::operator+)({"v"})
            .PHANTOM_T method<SelfType&(CharT const*)>("append", &_::append)({"a_pCstr"})
            .PHANTOM_T method<SelfType&(SelfType const&)>("append", &_::append)({"a_Other"})
            .PHANTOM_T method<SelfType&(CharT)>("append", &_::append)({"c"})
            .PHANTOM_T method<void(CharT)>("push_back", &_::push_back)({"c"})
            .PHANTOM_T method<SelfType&(CharT)>("assign", &_::assign)({"a_Ch"})
            .PHANTOM_T method<SelfType&(CharT const*)>("assign", &_::assign)({"a_pCstr"})
            .PHANTOM_T method<SelfType&(CharT const*, size_t)>("assign", &_::assign)({"a_pCstr","a_Len"})
            .PHANTOM_T method<SelfType&(CharT const*, size_t)>("append", &_::append)({"a_pCstr","a_Len"})
            .PHANTOM_T method<SelfType&(::std::initializer_list<CharT>)>("assign", &_::assign)({"a_IL"})
            .PHANTOM_T method<SelfType&(::std::initializer_list<CharT>)>("append", &_::append)({"a_IL"})
            .PHANTOM_T method<SelfType&(const SelfType&, size_t, size_t)>("append", &_::append)({"str","subpos","sublen"})
            .PHANTOM_T method<SelfType&(size_t, SelfType const&)>("insert", &_::insert)({"a_Pos","a_Str"})
            .PHANTOM_T method<SelfType&(size_t, CharT const*, size_t)>("insert", &_::insert)({"a_Pos","a_Cstr","a_Len"})
            .PHANTOM_T method<SelfType&(size_t, CharT)>("insert", &_::insert)({"a_Pos","a_C"})
            .PHANTOM_T method<iterator(const_iterator, ::std::initializer_list<CharT>)>("insert", &_::insert)({"a_Where","a_IL"})
            .PHANTOM_T method<size_t(size_t)>("erase", &_::erase)({"pos"})
            .PHANTOM_T method<iterator(iterator)>("erase", &_::erase)({"a_Where"})
            .PHANTOM_T method<iterator(iterator, iterator)>("erase", &_::erase)({"a_First","a_Last"})
            .PHANTOM_T method<void(SelfType&)>("swap", &_::swap)({"a_Other"})
            .PHANTOM_T method<void()>("pop_back", &_::pop_back)
            .PHANTOM_T method<CharT const*() const>("c_str", &_::c_str)
            .PHANTOM_T method<CharT const*() const>("data", &_::data)
            .PHANTOM_T method<CharT*()>("data", &_::data)
            .PHANTOM_T method<size_t(CharT) const>("find", &_::find)({"a_Ch"})
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find", &_::find)({"a_pWhat","a_Off"})["0"]
            .PHANTOM_T method<size_t(SelfType const&, size_t) const>("find", &_::find)({"a_What","a_Off"})["0"]
            .PHANTOM_T method<size_t(StringViewType, size_t) const>("find", &_::find)({"a_What","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_first_of", &_::find_first_of)({"ch","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_first_of", &_::find_first_of)({"chs","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_last_of", &_::find_last_of)({"ch","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_last_of", &_::find_last_of)({"chs","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_first_not_of", &_::find_first_not_of)({"ch","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_first_not_of", &_::find_first_not_of)({"chs","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_last_not_of", &_::find_last_not_of)({"ch","a_Off"})["npos"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_last_not_of", &_::find_last_not_of)({"chs","a_Off"})["npos"]
            .PHANTOM_T method<SelfType(size_t, size_t) const>("substr", &_::substr)({"a_Start","a_Offset"})["npos"]
            .PHANTOM_T method<StringViewType() const>("view", &_::view)
            .PHANTOM_T method<int(SelfType const&) const>("compare", &_::compare)({"a_Other"})
            .PHANTOM_T method<int(CharT const*) const>("compare", &_::compare)({"a_Cstr"})
            .PHANTOM_T method<int(size_t, size_t, CharT const*) const>("compare", &_::compare)({"start","len","a_Cstr"})
            .PHANTOM_T method<int(size_t, size_t, SelfType const&) const>("compare", &_::compare)({"start","len","a_Other"})
            .PHANTOM_T method<bool(SelfType const&) const>("operator<", &_::operator<)({"a_Other"})
            .PHANTOM_T method<bool(SelfType const&) const>("operator>", &_::operator>)({"a_Other"})
            .PHANTOM_T method<bool(SelfType const&) const>("operator==", &_::operator==)({"a_Other"})
            .PHANTOM_T method<bool(SelfType const&) const>("operator!=", &_::operator!=)({"a_Other"})
            .PHANTOM_T method<bool(CharT const*) const>("operator==", &_::operator==)({"a_Cstr"})
            .PHANTOM_T method<bool(CharT const*) const>("operator!=", &_::operator!=)({"a_Cstr"})
            .PHANTOM_T method<bool(StringViewType) const>("operator==", &_::operator==)({"a_Other"})
            .PHANTOM_T method<bool(StringViewType) const>("operator!=", &_::operator!=)({"a_Other"})
            ;
        }
    PHANTOM_END("SmallString")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
