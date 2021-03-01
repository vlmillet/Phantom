#pragma once

// haunt {

// clang-format off

#include "FieldPointer.h"

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
#include <phantom/field>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("FieldPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(FieldPointer)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::MemberPointer>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(ClassType*, Type*, Modifiers, uint)>()["0"]["0"]
        
        .protected_()
            .constructor<void(ClassType*, Type*, size_t, size_t, Modifiers, uint)>()
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<FieldPointer*() const, virtual_|override_>("asFieldPointer", &_::asFieldPointer)
            .method<size_t(void*) const, virtual_>("getOffset", &_::getOffset)
            .method<void(void*, void*, void const*) const, virtual_>("setValue", &_::setValue)
            .method<void(void*, void const*, void*) const, virtual_>("getValue", &_::getValue)
            .method<void*(void*, void const*) const, virtual_>("getAddress", &_::getAddress)
            .method<::phantom::lang::Type *() const>("getValueType", &_::getValueType)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("FieldPointer")
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
