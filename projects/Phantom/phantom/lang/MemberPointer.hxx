#pragma once

// haunt {

// clang-format off

#include "MemberPointer.h"

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
#include <phantom/using>
#include <phantom/friend>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("MemberPointer")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MemberPointer)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(TypeKind, ClassType*, size_t, size_t, Modifiers, uint)>()({"a_TypeId","a_pObjectType","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})["0"]["0"]
        
        .public_()
            .method<Type*() const, virtual_|override_>("asPOD", &_::asPOD)
            .method<MemberPointer*() const, virtual_|override_>("asMemberPointer", &_::asMemberPointer)
            .method<ClassType*() const>("getObjectType", &_::getObjectType)
            .method<void(void**, void*)>("assignment", &_::assignment)({"a_pArgs","a_pOutput"})
            .using_("Type::equal")
            .method<void(void**, void*)>("equal", &_::equal)({"a_pArgs","a_pOutput"})
            .method<void(void**, void*)>("notEqual", &_::notEqual)({"a_pArgs","a_pOutput"})
        
        .protected_()
            .field("m_pObjectType", &_::m_pObjectType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemberPointer")
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
