#pragma once

#include "ConstType.h"

// haunt {

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("ConstType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ConstType)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
            .inherits<::phantom::reflection::QualifiedType>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .method<Type*() const>("getConstedType", &_::getConstedType)
            .method<Type*() const, virtual_|override_>("addConst", &_::addConst)
            .method<Type*() const, virtual_|override_>("addVolatile", &_::addVolatile)
            .method<Type*() const, virtual_>("addConstVolatile", &_::addConstVolatile)
            .method<Type*() const, virtual_|override_>("removeConstVolatile", &_::removeConstVolatile)
            .method<Type*() const, virtual_|override_>("removeConst", &_::removeConst)
            .method<Type*() const, virtual_|override_>("removeAllConst", &_::removeAllConst)
            .method<Type*(Type*) const, virtual_|override_>("replicate", &_::replicate)
            .method<ConstType*() const, virtual_|override_>("asConstType", &_::asConstType)
            .method<ConstType*() const, virtual_|override_>("asConstClass", &_::asConstClass)
            /// missing symbol(s) reflection (phantom::reflection::PlaceholderMap) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(Type*) const, virtual_>("isConstConvertibleTo", &_::isConstConvertibleTo)
            .method<Type*() const, virtual_|override_>("getUnderlyingType", &_::getUnderlyingType)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<Type*() const, virtual_|override_>("removeEverything", &_::removeEverything)
        
        .protected_()
            .method<ConstVolatileType*() const, virtual_|override_>("createConstVolatileType", &_::createConstVolatileType)
            .method<ConstType*() const, virtual_|override_>("createConstType", &_::createConstType)
            .method<VolatileType*() const, virtual_|override_>("createVolatileType", &_::createVolatileType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ConstType")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
