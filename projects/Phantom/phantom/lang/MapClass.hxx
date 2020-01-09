#pragma once

// haunt {

#include "MapClass.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("MapClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MapClass)
        {
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::ContainerClass>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
            .method<Type*() const>("getKeyType", &_::getKeyType)
            .method<Type*() const>("getMappedType", &_::getMappedType)
            .method<void(Type*)>("setKeyType", &_::setKeyType)
            .method<void(Type*)>("setMappedType", &_::setMappedType)
            .method<void(void*, void const*) const, virtual_>("eraseKey", &_::eraseKey)
            .method<void(void*, void const*) const, virtual_>("insert", &_::insert)
            .method<void(void*, void const*, void*) const, virtual_>("map", &_::map)
            .method<void(void const*, void const*, void*) const, virtual_>("find", &_::find)
            .method<void(void*, void const*, void*) const, virtual_>("find", &_::find)
        
        .protected_()
            .field("m_pKeyType", &_::m_pKeyType)
            .field("m_pMappedType", &_::m_pMappedType)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MapClass")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
