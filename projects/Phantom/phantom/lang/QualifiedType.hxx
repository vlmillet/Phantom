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
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(TypeKind, Type*, StringView, size_t, size_t, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<void*(Type*, void*) const, virtual_|override_>("cast", &_::cast)
            .method<void*(Type*, void*) const, virtual_|override_>("upcast", &_::upcast)
            .method<void*(Type*, void*) const, virtual_|override_>("downcast", &_::downcast)
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)
            .method<TypeRelation(Type*) const, virtual_|override_>("getRelationWith", &_::getRelationWith)
            .method<bool(void const*, void const*) const, virtual_|override_>("equal", &_::equal)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)
            .method<void(void*, void const*) const, virtual_|override_>("copyAssign", &_::copyAssign)
            .method<void(void*, void*) const, virtual_|override_>("moveAssign", &_::moveAssign)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)
            .method<void(void*, void*) const, virtual_|override_>("moveConstruct", &_::moveConstruct)
            .method<bool(Type*, void*, void const*) const, virtual_|override_>("convert", &_::convert)
            .method<void(LanguageElements&, Class*) const, virtual_|override_>("fetchElements", &_::fetchElements)["nullptr"]
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
            .method<bool(Type*) const, virtual_|override_>("isEquivalent", &_::isEquivalent)
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
