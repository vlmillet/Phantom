#pragma once

#include "SmallString.h"

#include <phantom/reflection/meta_type>
#include <phantom/reflection/StringClassT.h>

PHANTOM_R_META_TYPE_T((typename, size_t, size_t), (_Elem, S, D), phantom::SmallString, phantom::reflection::StringClassT);


// haunt {

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

        PHANTOM_CLASS_T((class, size_t, size_t), (T, StaticAllocSize, DynamicAllocInc), SmallString)
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
            .PHANTOM_T constructor<void(SelfType const&), default_>()
            .PHANTOM_T constructor<void(SelfType&&)>()
            .PHANTOM_T constructor<void(::std::initializer_list<CharT>)>()
            .PHANTOM_T constructor<void(CharT), explicit_>()
            .PHANTOM_T constructor<void(CharT const*, size_t)>()
            .PHANTOM_T constructor<void(StringViewType)>()
            .PHANTOM_T constructor<void(CharT const*)>()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T constructor<void(int), explicit_>()
            .PHANTOM_T method<SelfType&(SelfType const&), default_>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(SelfType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(CharT const*)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(CharT)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T method<SelfType&(int)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
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

// haunt }
