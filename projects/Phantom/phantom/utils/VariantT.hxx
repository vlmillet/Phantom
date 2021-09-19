#pragma once

// haunt {

// clang-format off

#include "VariantT.h"

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
#include <phantom/constructor>
#include <phantom/property>
#include <phantom/typedef>
#include <phantom/friend>

namespace phantom {
namespace _VariantT {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("VariantT")

        PHANTOM_STRUCT_TV((class), (T), (class, Ts), SelectFirstType)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_TS((class), (T), (T), SelectFirstType)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
    PHANTOM_END("VariantT")
PHANTOM_END("phantom.utils")
}
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("VariantT")

        PHANTOM_STRUCT_V((class, Ts), VariantT)
        {
            using FirstType = typedef_<PHANTOM_TYPENAME _::FirstType>;
            this_()
            .PHANTOM_T inherits</* ::phantom::_VariantT::VariantT_Recursive<VariantT<Ts...>, Ts...> */>()
            .PHANTOM_T typedef_<FirstType>("FirstType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T method<::phantom::lang::Type *() const>("type", &_::type)
            .PHANTOM_T method<void const*() const>("data", &_::data)
            .PHANTOM_T method<void*()>("data", &_::data)
            .PHANTOM_T method<void(Variant)>("setGeneric", &_::setGeneric)({"_var"})
            .PHANTOM_T method<Variant() const>("GetGeneric", &_::GetGeneric)
            .PHANTOM_T property<Variant>("Generic", &_::GetGeneric, &_::setGeneric)
            ;
        }
    PHANTOM_END("VariantT")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
