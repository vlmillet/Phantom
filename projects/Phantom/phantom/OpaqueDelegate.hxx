#pragma once

// haunt {

#include "OpaqueDelegate.h"

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

namespace phantom {
namespace detail {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("OpaqueDelegate")

    PHANTOM_END("OpaqueDelegate")
PHANTOM_END("phantom")
}
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("OpaqueDelegate")

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
    PHANTOM_END("OpaqueDelegate")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
