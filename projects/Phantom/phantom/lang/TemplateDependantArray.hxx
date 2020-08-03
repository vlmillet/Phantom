#pragma once

// haunt {

// clang-format off

#include "TemplateDependantArray.h"

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
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateDependantArray")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantArray)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Array>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .constructor<void(Type*, Expression*, PlaceholderConstant*)>()
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)
            .method<::phantom::lang::TemplateDependantArray *() const, virtual_>("asTemplateDependantArray", &_::asTemplateDependantArray)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<PlaceholderConstant*() const>("getDeductibleConstant", &_::getDeductibleConstant)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*() const>("getItemCountExpression", &_::getItemCountExpression)
        
        .protected_()
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .field("m_pItemCountExpression", &_::m_pItemCountExpression)
            .field("m_pDeductibleConstant", &_::m_pDeductibleConstant)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantArray")
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
