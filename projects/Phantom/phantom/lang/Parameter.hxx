#pragma once

// haunt {

// clang-format off

#include "Parameter.h"

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
    PHANTOM_SOURCE("Parameter")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Parameter)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::LocalVariable>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(Type*, StringView, Modifiers, uint)>()["0"]["0"]
            .method<void(StringView)>("setDefinitionName", &_::setDefinitionName)
            .method<bool() const>("hasDefaultArgument", &_::hasDefaultArgument)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*() const>("getDefaultArgumentExpression", &_::getDefaultArgumentExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(Expression*)>("setDefaultArgumentExpression", &_::setDefaultArgumentExpression)
            .method<StringView() const>("getNativeDefaultArgumentString", &_::getNativeDefaultArgumentString)
            .method<bool() const>("isEllipsis", &_::isEllipsis)
            .method<Parameter*() const, virtual_|override_>("asParameter", &_::asParameter)
            .method<Parameter*(LanguageElement*) const, virtual_|override_>("cloneImpl", &_::cloneImpl)
        
        .protected_()
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_pDefaultArgumentExpression", &_::m_pDefaultArgumentExpression)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Parameter")
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
