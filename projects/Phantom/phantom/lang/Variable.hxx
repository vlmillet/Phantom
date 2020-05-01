#pragma once

// haunt {

// clang-format off

#include "Variable.h"

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
    PHANTOM_SOURCE("Variable")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Variable)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Type*, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, void*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, StringView, void*, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<void*() const>("getAddress", &_::getAddress)
            .method<Type*() const>("getValueType", &_::getValueType)
            .method<void(void const*) const>("setValue", &_::setValue)
            .method<void(void*) const>("getValue", &_::getValue)
            .method<Variable*() const, virtual_|override_>("asVariable", &_::asVariable)
            .method<StaticField*() const, virtual_|override_>("asStaticField", &_::asStaticField)
            .method<void(Type*)>("setValueType", &_::setValueType)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Expression*)>("setInitializationExpression", &_::setInitializationExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*() const>("getInitializationExpression", &_::getInitializationExpression)
        
        .protected_()
            .method<void(void*)>("setAddress", &_::setAddress)
        
        .protected_()
            .method<void(), virtual_>("finalize", &_::finalize)
        
        .protected_()
            .field("m_pValueType", &_::m_pValueType)
            .field("m_pAddress", &_::m_pAddress)
            .field("m_pAnonymousSection", &_::m_pAnonymousSection)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_pInitializationExpression", &_::m_pInitializationExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Variable")
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
