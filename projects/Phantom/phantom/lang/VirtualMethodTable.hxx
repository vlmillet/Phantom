#pragma once

// haunt {

#include "VirtualMethodTable.h"

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
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("VirtualMethodTable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(VirtualMethodTable)
        {
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void()>()
            .constructor<void(size_t)>()
            .constructor<void(void**, size_t)>()
            .method<size_t() const>("getMethodCount", &_::getMethodCount)
            .method<size_t(Method*) const>("getIndexOf", &_::getIndexOf)
            .method<Method*(size_t) const>("getMethod", &_::getMethod)
            .method<Class*() const>("getOwnerClass", &_::getOwnerClass)
            .method<Class*() const>("getOriginalClass", &_::getOriginalClass)
            .method<size_t() const>("getOffset", &_::getOffset)
            .method<VirtualMethodTable*(size_t) const>("derive", &_::derive)["0"]
            .method<void(void*)>("construct", &_::construct)
            .method<VirtualMethodTable*() const>("getRootTable", &_::getRootTable)
            .method<Method*(size_t) const>("getRootMethod", &_::getRootMethod)
            .method<Method*(Method*) const>("getRootMethod", &_::getRootMethod)
            .method<VirtualMethodTable*() const, virtual_|override_>("asVirtualMethodTable", &_::asVirtualMethodTable)
            .method<size_t(Method*) const>("getMethodIndex", &_::getMethodIndex)
            .method<void**() const>("getClosurePointers", &_::getClosurePointers)
            .method<bool(Method*, bool)>("insertMethod", &_::insertMethod)
            .method<void(void*)>("extractNativeClosures", &_::extractNativeClosures)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("VirtualMethodTable")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
