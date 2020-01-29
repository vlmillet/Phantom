#pragma once

// haunt {

#include "InitializerListType.h"

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
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("InitializerListType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(InitializerListType)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .method<TypesView() const>("getTypes", &_::getTypes)
            .method<bool(TypesView) const>("matches", &_::matches)
            .method<InitializerListType*() const, virtual_|override_>("asInitializerListType", &_::asInitializerListType)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<Pointer*() const, virtual_|override_>("createPointer", &_::createPointer)
            .method<LValueReference*() const, virtual_|override_>("createLValueReference", &_::createLValueReference)
            .method<RValueReference*() const, virtual_|override_>("createRValueReference", &_::createRValueReference)
            .method<ConstVolatileType*() const, virtual_|override_>("createConstVolatileType", &_::createConstVolatileType)
            .method<VolatileType*() const, virtual_|override_>("createVolatileType", &_::createVolatileType)
        
        .protected_()
            .field("m_Types", &_::m_Types)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("InitializerListType")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
