#pragma once

// haunt {

// clang-format off

#include "Delegate.h"

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
#include <phantom/constant>
#include <phantom/method>
#include <phantom/constructor>

namespace phantom {
namespace detail {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Delegate")

    PHANTOM_END("Delegate")
PHANTOM_END("phantom.utils")
}
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Delegate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(OpaqueDelegate)
        {
            this_()
        .public_()
            .constructor<void()>()
            .method<void()>("clear", &_::clear)
        
        .public_()
            .method<bool(const OpaqueDelegate&) const>("operator==", &_::operator==)
            .method<bool(const OpaqueDelegate&) const>("operator!=", &_::operator!=)
            .method<bool() const>("operator!", &_::operator!)
            .method<bool() const>("empty", &_::empty)
            .method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .method<int() const>("getThisOffset", &_::getThisOffset)
            /// missing symbol(s) reflection (phantom::MethodClosure) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<MethodClosure() const>("getMethod", &_::getMethod)
            .method<void*() const>("getThis", &_::getThis)
            .method<void*() const>("getFunction", &_::getFunction)
            .method<FunctorID() const>("getID", &_::getID)
        
        .public_()
            .method<OpaqueDelegate&(const OpaqueDelegate&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(const OpaqueDelegate&) const>("operator<", &_::operator<)
            .method<bool(const OpaqueDelegate&) const>("operator>", &_::operator>)
            .constructor<void(const OpaqueDelegate&)>()
        
        .protected_()
            .method<void(const OpaqueDelegate&)>("setOpaqueFrom", &_::setOpaqueFrom)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Delegate")
PHANTOM_END("phantom.utils")
namespace detail {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Delegate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Variables) { this_().constant("delegate_generic_class_mfptr_size", delegate_generic_class_mfptr_size); }
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT_T((int), (N), MFHacker)
        {
            this_()
            ;
        }
        PHANTOM_STRUCT_S((8), MFHacker)
        {
            this_()
            ;
        }
    PHANTOM_END("Delegate")
PHANTOM_END("phantom.utils")
}
namespace detail {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Delegate")

        PHANTOM_CLASS_T((class, class), (t_GenericMFP, t_StaticFP), ClosurePtr)
        {
            this_()
            .PHANTOM_T inherits<::phantom::OpaqueDelegate>()
        .public_()
            /// missing symbol(s) reflection (phantom::detail::DelegateGenericClass, ) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<DelegateGenericClass*() const>("getThis", &_::getThis)
            .PHANTOM_T method<t_GenericMFP() const>("getMFPtr", &_::getMFPtr)
            .PHANTOM_T method<t_StaticFP() const>("getStaticFP", &_::getStaticFP)
            .PHANTOM_T method<bool(t_StaticFP)>("contains", &_::contains)
            ;
        }
    PHANTOM_END("Delegate")
PHANTOM_END("phantom.utils")
}
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Delegate")

        PHANTOM_CLASS_T((class), (S), Delegate)
        {
            this_()
            ;
        }
        PHANTOM_CLASS_TVS((class), (R), (class, Params), (R(Params...)), Delegate)
        {
            this_()
            .PHANTOM_T method<R(Params...) const>("operator()", &_::operator())
            .PHANTOM_T method<FunctorID() const>("getID", &_::getID)
        
        .public_()
            /// missing symbol(s) reflection (phantom::Delegate::FuncPtrT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(phantom::Delegate<type-parameter-0-0 (type-parameter-0-1...)>::FuncPtrT)>("operator==", &_::operator==)
            /// missing symbol(s) reflection (phantom::Delegate::FuncPtrT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(phantom::Delegate<type-parameter-0-0 (type-parameter-0-1...)>::FuncPtrT)>("operator!=", &_::operator!=)
            .PHANTOM_T method<bool() const>("operator!", &_::operator!)
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<void()>("clear", &_::clear)
            .PHANTOM_T method<const OpaqueDelegate&() const>("getOpaque", &_::getOpaque)
            .PHANTOM_T method<void(const OpaqueDelegate&)>("setOpaque", &_::setOpaque)
            ;
        }
    PHANTOM_END("Delegate")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
