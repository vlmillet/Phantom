#pragma once

// haunt {

// clang-format off

#include "Placement.h"

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
#include <phantom/field>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Placement")

        PHANTOM_CLASS_T((class), (T), Placement)
        {
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .PHANTOM_T typedef_<SelfType>("SelfType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T method<void(T const&)>("assign", &_::assign)({"a_Val"})
            .PHANTOM_T method<void(T&&)>("assign", &_::assign)({"a_Val"})
            .PHANTOM_T method<void()>("destroy", &_::destroy)
            .PHANTOM_T method<T&()>("operator*", &_::operator*)
            .PHANTOM_T method<T const&() const>("operator*", &_::operator*)
            .PHANTOM_T method<T*()>("operator->", &_::operator->)
            .PHANTOM_T method<T const*() const>("operator->", &_::operator->)
            .PHANTOM_T method<const T*() const>("operator const T*", &_::operator notypedef<const T*>)
            .PHANTOM_T method<T*()>("operator T*", &_::operator notypedef<T*>)
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            ;
        }
        PHANTOM_CLASS_T((class), (T), RawPlacement)
        {
            this_()
        .public_()
            .PHANTOM_T method<void()>("destroy", &_::destroy)
            .PHANTOM_T method<T&()>("operator*", &_::operator*)
            .PHANTOM_T method<T const&() const>("operator*", &_::operator*)
            .PHANTOM_T method<T*()>("operator->", &_::operator->)
            .PHANTOM_T method<T const*() const>("operator->", &_::operator->)
            .PHANTOM_T method<const T*() const>("operator const T*", &_::operator notypedef<const T*>)
            .PHANTOM_T method<T*()>("operator T*", &_::operator notypedef<T*>)
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T field("m_data", &_::m_data)
            .PHANTOM_T field("m_constructed", &_::m_constructed)
            ;
        }
    PHANTOM_END("Placement")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
