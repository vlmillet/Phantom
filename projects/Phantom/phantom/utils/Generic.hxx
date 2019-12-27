#pragma once

// haunt {

#include "Generic.h"

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

namespace phantom {
namespace Generic {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Generic")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(Marker)
        {
            this_()
            ;
        }
        PHANTOM_STRUCT(Auto)
        {
            this_()
            .inherits<::phantom::Generic::Marker>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((class), (Decoration), Type)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(::phantom::reflection::Type *)>()
            .PHANTOM_T method<::phantom::reflection::Type *() const>("operator ::phantom::reflection::Type *", &_::operator notypedef<::phantom::reflection::Type *>)
            .PHANTOM_T method<reflection::Type*() const>("getType", &_::getType)
            .PHANTOM_T method<reflection::Class*() const>("getClass", &_::getClass)
            ;
        }
        PHANTOM_STRUCT_T((class), (T), Param)
        {
            this_()
            .PHANTOM_T inherits<::phantom::Generic::Type<T>>()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(::phantom::reflection::Type *)>()
            ;
        }
        PHANTOM_STRUCT_T((class), (TypeRef), ConstructorParams)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(void**)>()
            .PHANTOM_T method<void**() const>("operator void**", &_::operator notypedef<void**>)
            ;
        }
        PHANTOM_STRUCT_T((class, class), (TypePattern, ReturnType), Return)
        {
            this_()
            .PHANTOM_T inherits<::phantom::Generic::Marker>()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(ReturnType)>()
            .PHANTOM_T method<ReturnType() const>("operator ReturnType", &_::operator notypedef<ReturnType>)
            ;
        }
        PHANTOM_STRUCT_T((class, class), (TypePattern, ArgType), Arg)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(ArgType)>()
            .PHANTOM_T method<ArgType() const>("operator ArgType", &_::operator notypedef<ArgType>)
            .PHANTOM_T method<ArgType() const>("getValue", &_::getValue)
            ;
        }
        PHANTOM_STRUCT_T((size_t), (ParamIndex), ParamType)
        {
            this_()
            .PHANTOM_T inherits<::phantom::Generic::Marker>()
            ;
        }
    PHANTOM_END("Generic")
PHANTOM_END("phantom.utils")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
