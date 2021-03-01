#pragma once

// haunt {

// clang-format off

#include "ContainerClass.h"

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
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ContainerClass")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ContainerClass)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Class>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .protected_()
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<::phantom::lang::Type *() const, virtual_>("getKeyType", &_::getKeyType)
            .method<::phantom::lang::Type *() const, virtual_>("getMappedType", &_::getMappedType)
            .method<Type*() const>("getValueType", &_::getValueType)
            .method<void(Type*)>("setValueType", &_::setValueType)
            .method<ContainerClass*() const, virtual_|override_>("asContainerClass", &_::asContainerClass)
            .method<void(void*, void*) const, virtual_>("begin", &_::begin)
            .method<void(void const*, void*) const, virtual_>("begin", &_::begin)
            .method<void(void*, void*) const, virtual_>("end", &_::end)
            .method<void(void const*, void*) const, virtual_>("end", &_::end)
            .method<void*(void*) const, virtual_>("dereferenceIterator", &_::dereferenceIterator)
            .method<void(void*, size_t) const, virtual_>("advanceIterator", &_::advanceIterator)["1"]
            .method<bool(void*, void*) const, virtual_>("compareIterators", &_::compareIterators)
            .method<size_t(void const*) const, virtual_>("size", &_::size)
            .method<void(void*, void const*) const, virtual_>("erase", &_::erase)
            .method<void(void*) const, virtual_>("clear", &_::clear)
            .method<void(void*, void const*, void const*, void*) const, virtual_>("insert", &_::insert)
            .method<void(void*, void const*) const, virtual_>("eraseKey", &_::eraseKey)
            .method<void(void*, size_t) const, virtual_>("eraseAt", &_::eraseAt)
            .method<const void*(void const*, size_t) const, virtual_>("referenceAt", &_::referenceAt)
            .method<void*(void*, size_t) const, virtual_>("referenceAt", &_::referenceAt)
            .method<::phantom::lang::Type *() const, virtual_>("getIteratorType", &_::getIteratorType)
            .method<::phantom::lang::Type *() const, virtual_>("getConstIteratorType", &_::getConstIteratorType)
            .method<Property*() const>("getSizeProperty", &_::getSizeProperty)
        
        .protected_()
            .method<::phantom::lang::Property *(), virtual_>("createSizeProperty", &_::createSizeProperty)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            .field("m_pSizeProperty", &_::m_pSizeProperty)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ContainerClass")
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
