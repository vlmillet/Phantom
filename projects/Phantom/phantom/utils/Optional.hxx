#pragma once

// haunt {

// clang-format off

#include "Optional.h"

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
#include <phantom/enum>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Optional")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Enums) { this_().enum_<_NullOpt>().values({
            {"NullOpt",NullOpt}});
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS_T((class), (T), Optional)
        {
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            this_()
        .public_()
            .PHANTOM_T typedef_<SelfType>("SelfType")
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(T const&)>()
            .PHANTOM_T constructor<void(T&&)>()
            .PHANTOM_T constructor<void(const SelfType&)>()
            .PHANTOM_T constructor<void(SelfType&&)>()
            .PHANTOM_T constructor<void(_NullOpt)>()
            .PHANTOM_T method<SelfType&(const SelfType&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(SelfType&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(_NullOpt)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(T&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<SelfType&(T const&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .PHANTOM_T method<T*()>("operator->", &_::operator->)
            .PHANTOM_T method<T&()>("operator*", &_::operator*)
            .PHANTOM_T method<T const*() const>("operator->", &_::operator->)
            .PHANTOM_T method<T const&() const>("operator*", &_::operator*)
            .PHANTOM_T method<T const*() const>("operator T const*", &_::operator notypedef<T const*>)
            .PHANTOM_T method<T*()>("operator T*", &_::operator notypedef<T*>)
            .PHANTOM_T method<void()>("destroy", &_::destroy)
            .PHANTOM_T method<void()>("reset", &_::reset)
            .PHANTOM_T method<bool() const>("isInitialized", &_::isInitialized)
            .PHANTOM_T method<bool() const>("isNull", &_::isNull)
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            ;
        }
    PHANTOM_END("Optional")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
