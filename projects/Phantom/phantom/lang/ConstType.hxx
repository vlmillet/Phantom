#pragma once

#include "ConstType.h"

// haunt {

// clang-format off

#include "ConstType.h"

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
    PHANTOM_SOURCE("ConstType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ConstType)
        {
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::QualifiedType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Type*)>()
            .method<Type*() const>("getConstedType", &_::getConstedType)
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)
            .method<ConstType*() const, virtual_|override_>("asConstClass", &_::asConstClass)
            .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(Type*) const, virtual_>("isConstConvertibleTo", &_::isConstConvertibleTo)
            .method<bool() const, virtual_|override_>("isMoveAssignable", &_::isMoveAssignable)
            .method<bool() const, virtual_|override_>("isCopyAssignable", &_::isCopyAssignable)
            .method<hash64() const, virtual_|override_>("computeLocalHash", &_::computeLocalHash)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ConstType")
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
