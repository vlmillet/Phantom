#pragma once

// haunt {

// clang-format off

#include "ValueMember.h"

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
    PHANTOM_SOURCE("ValueMember")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ValueMember)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Type*, StringView, uint, Modifiers, uint)>()["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<ValueMember*() const, virtual_|override_>("asValueMember", &_::asValueMember)
            .method<bool() const, virtual_>("isMovable", &_::isMovable)
            .method<bool() const, pure_virtual>("isWrittable", &_::isWrittable)
            .method<bool() const, pure_virtual>("isReadable", &_::isReadable)
            .method<void(void const*, void*) const, pure_virtual>("getValue", &_::getValue)
            .method<void(void*, void const*) const, pure_virtual>("setValue", &_::setValue)
            .method<void(void*, void*) const, virtual_>("setValueMoved", &_::setValueMoved)
            .method<::phantom::lang::Type *() const>("getValueType", &_::getValueType)
            .method<uint() const>("getFilterMask", &_::getFilterMask)
            .method<void(uint)>("setFilterMask", &_::setFilterMask)
            .method<void(void*, void const*) const, virtual_>("copyValue", &_::copyValue)
            .method<::phantom::lang::ClassType *() const>("getOwnerClassType", &_::getOwnerClassType)
            .method<::phantom::lang::Class *() const>("getOwnerClass", &_::getOwnerClass)
            .method<void*(void const*) const, virtual_>("getAddress", &_::getAddress)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Expression*)>("setDefaultExpression", &_::setDefaultExpression)
            .method<void(const char*)>("setDefaultExpression", &_::setDefaultExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*() const>("getDefaultExpression", &_::getDefaultExpression)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_pDefaultExpression", &_::m_pDefaultExpression)
            .field("m_uiFilterMask", &_::m_uiFilterMask)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ValueMember")
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
