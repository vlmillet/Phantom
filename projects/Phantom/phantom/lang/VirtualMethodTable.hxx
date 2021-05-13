#pragma once

// haunt {

// clang-format off

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
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void()>()
            .constructor<void(size_t)>()({"a_uiSize"})
            .constructor<void(void**, size_t)>()({"a_ppClosures","a_uiSize"})
            .constructor<void(VirtualMethodTable*)>()({"a_pBaseTable"})
            .constructor<void(VirtualMethodTable*, size_t)>()({"a_pBaseTable","a_uiSize"})
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<size_t() const>("getMethodCount", &_::getMethodCount)
            .method<size_t(Method*) const>("getIndexOf", &_::getIndexOf)({"a_pMethod"})
            .method<Method*(size_t) const>("getMethod", &_::getMethod)({"a_uiIndex"})
            .method<Class*() const>("getOwnerClass", &_::getOwnerClass)
            .method<Class*() const>("getOriginalClass", &_::getOriginalClass)
            .method<size_t() const>("getOffset", &_::getOffset)
            .method<VirtualMethodTable*(Class*, size_t) const>("derive", &_::derive)({"a_pOwner","a_uiSize"})["0"]
            .method<void(void*)>("construct", &_::construct)({"a_pInstance"})
            .method<VirtualMethodTable*() const>("getRootTable", &_::getRootTable)
            .method<Method*(size_t) const>("getRootMethod", &_::getRootMethod)({"a_uiIndex"})
            .method<Method*(Method*) const>("getRootMethod", &_::getRootMethod)({"a_pMethod"})
            .method<VirtualMethodTable*() const, virtual_|override_>("asVirtualMethodTable", &_::asVirtualMethodTable)
            .method<size_t(Method*) const>("getMethodIndex", &_::getMethodIndex)({"a_pMethod"})
            .method<void**() const>("getClosurePointers", &_::getClosurePointers)
            .method<bool(Method*, bool)>("insertMethod", &_::insertMethod)({"a_pMethod","a_bOnlyIfOverrides"})
            .method<void(void*)>("extractNativeVTable", &_::extractNativeVTable)({"a_pInstance"})
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

// clang-format on

// haunt }
