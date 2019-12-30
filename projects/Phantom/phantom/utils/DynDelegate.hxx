#pragma once

// haunt {

#include "DynDelegate.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/friend>

#include <phantom/template-only-push>

#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("DynDelegate")

        PHANTOM_CLASS_T((class), (Sign), DynDelegate)
        {
            this_()
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(OpaqueDynDelegate)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()
        
        .public_()
            .constructor<void(), default_>()
            .constructor<void(void*, ::phantom::reflection::Class *, ::phantom::reflection::Method *)>()
            .constructor<void(::phantom::reflection::Function *)>()
            .constructor<void(void*, ::phantom::reflection::Class *, StringView)>()
            .method<void*() const>("getThis", &_::getThis)
            .method<reflection::Function*() const>("getFunction", &_::getFunction)
            .method<reflection::Method*() const>("getMethod", &_::getMethod)
            .method<reflection::Subroutine*() const>("getSubroutine", &_::getSubroutine)
            .method<bool() const>("isEmpty", &_::isEmpty)
            .method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .method<bool(const OpaqueDynDelegate&) const>("operator==", &_::operator==)
            .method<bool(const OpaqueDynDelegate&) const>("operator!=", &_::operator!=)
            /// missing symbol(s) reflection (phantom::FunctorID) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<FunctorID() const>("getID", &_::getID)
            .method<void(void**) const>("call", &_::call)
            .method<void(void**, void*) const>("call", &_::call)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DynDelegate")
PHANTOM_END("phantom.utils")
namespace _DynDelegate {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("DynDelegate")

        PHANTOM_STRUCT_TV((class), (R), (class, Params), Caller)
        {
            this_()
            .PHANTOM_T staticMethod<R(OpaqueDynDelegate const&, Params...)>("Call", &_::Call)
            ;
        }
        PHANTOM_STRUCT_VS((class, Params), (void, Params...), Caller)
        {
            this_()
            .PHANTOM_T staticMethod<void(OpaqueDynDelegate const&, Params...)>("Call", &_::Call)
            ;
        }
    PHANTOM_END("DynDelegate")
PHANTOM_END("phantom.utils")
}
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("DynDelegate")

        PHANTOM_CLASS_TVS((class), (R), (class, Params), (R(Params...)), DynDelegate)
        {
            using StringView = typedef_< phantom::StringView>;
            using ThisType = typedef_<PHANTOM_TYPENAME _::ThisType>;
            this_()
            .PHANTOM_T inherits<::phantom::OpaqueDynDelegate>()
        .public_()
            .PHANTOM_T typedef_<ThisType>("ThisType")
        
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(::phantom::reflection::Function *)>()
            .PHANTOM_T constructor<void(void*, ::phantom::reflection::Class *, ::phantom::reflection::Method *)>()
            .PHANTOM_T constructor<void(void*, ::phantom::reflection::Class *, StringView)>()
            .PHANTOM_T constructor<void(OpaqueDynDelegate), explicit_>()
            .PHANTOM_T method<R(Params...) const>("operator()", &_::operator())
            ;
        }
    PHANTOM_END("DynDelegate")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
