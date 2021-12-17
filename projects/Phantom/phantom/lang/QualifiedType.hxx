#pragma once

// haunt {

// clang-format off

#include "QualifiedType.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("QualifiedType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(QualifiedType)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ExtendedType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(TypeKind, Type*, StringView, size_t, size_t, Modifiers, uint)>()({"a_eTypeKind","a_pType","a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})["0"]["0"]
        
        .public_()
            .method<void*(Type*, void*) const, virtual_|override_>("cast", &_::cast)({"a_pTargetType","a_pSrc"})
            .method<void*(Type*, void*) const, virtual_|override_>("upcast", &_::upcast)({"a_pTargetType","a_pSrc"})
            .method<void*(Type*, void*) const, virtual_|override_>("downcast", &_::downcast)({"a_pTargetType","a_pSrc"})
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)({"a_pBuffer"})
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)({"a_pBuffer"})
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)({"a_pInstance"})
            .method<TypeRelation(Type*) const, virtual_|override_>("getRelationWith", &_::getRelationWith)({"a_pType"})
            .method<bool(void const*, void const*) const, virtual_|override_>("equal", &_::equal)({"a_pSrc0","a_pSrc1"})
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)({"a_str","dest"})
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)({"a_Buf","src"})
            .method<void(void*, void const*) const, virtual_|override_>("copyAssign", &_::copyAssign)({"a_pDest","a_pSrc"})
            .method<void(void*, void*) const, virtual_|override_>("moveAssign", &_::moveAssign)({"a_pDest","a_pSrc"})
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)({"a_pDest","a_pSrc"})
            .method<void(void*, void*) const, virtual_|override_>("moveConstruct", &_::moveConstruct)({"a_pDest","a_pSrc"})
            .method<bool(Type*, void*, void const*) const, virtual_|override_>("convert", &_::convert)({"a_pDstType","a_pDst","a_pSrc"})
            .method<void(LanguageElements&, Class*) const, virtual_|override_>("fetchElements", &_::fetchElements)({"out","a_pClass"})["nullptr"]
            .method<bool() const, virtual_|override_>("hasCopyDisabled", &_::hasCopyDisabled)
            .method<bool() const, virtual_|override_>("hasMoveDisabled", &_::hasMoveDisabled)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_|override_>("isCopyConstructible", &_::isCopyConstructible)
            .method<bool() const, virtual_|override_>("isCopyAssignable", &_::isCopyAssignable)
            .method<bool() const, virtual_|override_>("isTriviallyCopyAssignable", &_::isTriviallyCopyAssignable)
            .method<bool() const, virtual_|override_>("isTriviallyCopyConstructible", &_::isTriviallyCopyConstructible)
            .method<bool() const, virtual_|override_>("isMoveable", &_::isMoveable)
            .method<bool() const, virtual_|override_>("isMoveConstructible", &_::isMoveConstructible)
            .method<bool() const, virtual_|override_>("isMoveAssignable", &_::isMoveAssignable)
            .method<bool() const, virtual_|override_>("isTriviallyMoveAssignable", &_::isTriviallyMoveAssignable)
            .method<bool() const, virtual_|override_>("isTriviallyMoveConstructible", &_::isTriviallyMoveConstructible)
            .method<bool(Type*) const, virtual_|override_>("isEquivalent", &_::isEquivalent)({"a_pType"})
            .method<Type*() const, virtual_|override_>("unsignedType", &_::unsignedType)
            .method<Type*() const, virtual_|override_>("signedType", &_::signedType)
            .method<Type*() const, virtual_|override_>("longType", &_::longType)
            .method<bool() const, virtual_|override_>("isSignedInteger", &_::isSignedInteger)
            .method<bool() const, virtual_|override_>("isUnsignedInteger", &_::isUnsignedInteger)
            .method<Type*() const, virtual_|override_>("promote", &_::promote)
            .method<uint() const, virtual_|override_>("getDataPointerLevel", &_::getDataPointerLevel)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("QualifiedType")
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
