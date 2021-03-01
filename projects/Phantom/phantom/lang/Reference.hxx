#pragma once

// haunt {

// clang-format off

#include "Reference.h"

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
    PHANTOM_SOURCE("Reference")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Reference)
        {
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ExtendedType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .protected_()
            .constructor<void(TypeKind, Type*, StringView)>()
        
        .public_()
            .method<bool() const, virtual_|override_>("isDefaultConstructible", &_::isDefaultConstructible)
            .method<Type*() const, virtual_|override_>("asClassAddressType", &_::asClassAddressType)
            .method<::phantom::lang::Reference *() const, virtual_>("asClassReference", &_::asClassReference)
            .method<Type*() const, virtual_|override_>("asConstClassAddressType", &_::asConstClassAddressType)
            .method<::phantom::lang::Reference *() const, virtual_>("asConstClassReference", &_::asConstClassReference)
            .method<Type*() const>("getReferencedType", &_::getReferencedType)
            .method<void(StringView, void*) const, virtual_|override_>("valueFromString", &_::valueFromString)
            .method<void(StringBuffer&, const void*) const, virtual_|override_>("valueToString", &_::valueToString)
            .method<Type*(Type*) const, pure_virtual|override_>("replicate", &_::replicate)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<void(void*, void const*) const, virtual_|override_>("copyAssign", &_::copyAssign)
            .method<void(void*, void*) const, virtual_|override_>("moveAssign", &_::moveAssign)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)
            .method<void(void*, void*) const, virtual_|override_>("moveConstruct", &_::moveConstruct)
            .method<bool(Type*, size_t&, PlaceholderMap&) const, pure_virtual|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, pure_virtual|override_>("isSame", &_::isSame)
            .method<void(void**, void*)>("address", &_::address)
            .method<void(void**, void*)>("memcpyStoreDelegate", &_::memcpyStoreDelegate)
            .method<bool() const, virtual_|override_>("isMoveAssignable", &_::isMoveAssignable)
            .method<bool() const, virtual_|override_>("isMoveConstructible", &_::isMoveConstructible)
            .method<bool() const, virtual_|override_>("isCopyAssignable", &_::isCopyAssignable)
            .method<bool() const, virtual_|override_>("isCopyConstructible", &_::isCopyConstructible)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Reference")
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
