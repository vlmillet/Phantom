#pragma once

// haunt {

// clang-format off

#include "StringView.h"

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
#include <phantom/static_constant>
#include <phantom/function>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "SmallString.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("StringView")

        PHANTOM_STRUCT_T((class), (CharT), NullTerminatedBasicStringView)
        {
            this_()
            .PHANTOM_T constant("null", _::null)
            .PHANTOM_T constructor<void(BasicStringView<CharT>)>()({"a_Str"})
            .PHANTOM_T method<const char*()>("operator const char*", &_::operator notypedef<const char*>)
            .PHANTOM_T field("nullTerminated", &_::nullTerminated)
            .PHANTOM_T field("notNullTerminated", &_::notNullTerminated)
            ;
        }
        PHANTOM_CLASS_T((class), (CharT), BasicStringView)
        {
            using NullTerminatedType = typedef_<PHANTOM_TYPENAME _::NullTerminatedType>;
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            this_()
        .public_()
            .PHANTOM_T constant("npos", _::npos)
            .PHANTOM_T typedef_<SelfType>("SelfType")
            .PHANTOM_T typedef_<NullTerminatedType>("NullTerminatedType")
            .PHANTOM_T constructor<void(const CharT*)>()({"a_Str"})
            .PHANTOM_T constructor<void(const CharT*, const CharT*)>()({"a_First","a_Last"})
            .PHANTOM_T constructor<void(const CharT*, size_t)>()({"a_Str","a_Len"})
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<SelfType(size_t, size_t)>("substr", &_::substr)({"a_Start","a_Off"})["npos"]
            .PHANTOM_T method<CharT()>("dropBack", &_::dropBack)
            .PHANTOM_T method<void(size_t)>("dropBack", &_::dropBack)({"N"})
            .PHANTOM_T method<CharT()>("dropFront", &_::dropFront)
            .PHANTOM_T method<void(size_t)>("dropFront", &_::dropFront)({"N"})
            .PHANTOM_T method<bool(SelfType) const>("startsWith", &_::startsWith)({"a_Str"})
            .PHANTOM_T method<const CharT&(size_t) const>("operator[]", &_::operator[])({"Idx"})
            .PHANTOM_T method<const CharT*() const>("data", &_::data)
            .PHANTOM_T method<const CharT*() const>("begin", &_::begin)
            .PHANTOM_T method<const CharT*() const>("end", &_::end)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<NullTerminatedType() const>("nullTerminated", &_::nullTerminated)
            .PHANTOM_T method<CharT() const>("front", &_::front)
            .PHANTOM_T method<CharT() const>("back", &_::back)
            .PHANTOM_T method<size_t(CharT) const>("find", &_::find)({"a_Ch"})
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find", &_::find)({"a_pWhat","a_Off"})["0"]
            .PHANTOM_T method<size_t(SelfType, size_t) const>("find", &_::find)({"a_What","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_first_of", &_::find_first_of)({"ch","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_first_of", &_::find_first_of)({"chs","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_last_of", &_::find_last_of)({"ch","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_last_of", &_::find_last_of)({"chs","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_first_not_of", &_::find_first_not_of)({"ch","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_first_not_of", &_::find_first_not_of)({"chs","a_Off"})["0"]
            .PHANTOM_T method<size_t(CharT, size_t) const>("find_last_not_of", &_::find_last_not_of)({"ch","a_Off"})["npos"]
            .PHANTOM_T method<size_t(CharT const*, size_t) const>("find_last_not_of", &_::find_last_not_of)({"chs","a_Off"})["npos"]
            .PHANTOM_T method<int(SelfType) const>("compare", &_::compare)({"_other"})
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<bool(BasicStringView<char>, BasicStringView<char>)>("operator==", operator==)({"a_Left","a_Right"});}
        PHANTOM_REGISTER(Functions) { this_().function<bool(BasicStringView<char>, BasicStringView<char>)>("operator!=", operator!=)({"a_Left","a_Right"});}
        PHANTOM_REGISTER(Functions) { this_().function<bool(BasicStringView<char>, BasicStringView<char>)>("operator<", operator<)({"a_Left","a_Right"});}
        PHANTOM_REGISTER(Functions) { this_().function<bool(BasicStringView<char>, BasicStringView<char>)>("operator>", operator>)({"a_Left","a_Right"});}
        PHANTOM_REGISTER(Functions) { this_().function<bool(BasicStringView<char>, BasicStringView<char>)>("operator<=", operator<=)({"a_Left","a_Right"});}
        PHANTOM_REGISTER(Functions) { this_().function<bool(BasicStringView<char>, BasicStringView<char>)>("operator>=", operator>=)({"a_Left","a_Right"});}
        PHANTOM_REGISTER(Functions) { this_().function<const char*(StringView)>("cstr", cstr)({"str"});}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringView")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
