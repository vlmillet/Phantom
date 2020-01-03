#pragma once

// haunt {

#include "TemplateDependantElement.h"

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

#include <phantom/utils/ArrayView.hxx>
#include <phantom/utils/Optional.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("TemplateDependantElement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantElement)
        {
            using LanguageElements = typedef_< phantom::reflection::LanguageElements>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(LanguageElement*, StringView, OptionalArrayView<LanguageElement*>, OptionalArrayView<LanguageElement*>, Modifiers, uint)>()["NullOpt"]["NullOpt"]["0"]["0"]
            .method<Type*() const, virtual_|override_>("toType", &_::toType)
            .method<LanguageElement*() const>("getLeft", &_::getLeft)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<LanguageElements*() const>("getTemplateArguments", &_::getTemplateArguments)
            .method<LanguageElements*() const>("getFunctionArguments", &_::getFunctionArguments)
        
        .protected_()
            .field("m_pAsType", &_::m_pAsType)
            .field("m_pLeft", &_::m_pLeft)
            .field("m_pTemplateArguments", &_::m_pTemplateArguments)
            .field("m_pFunctionArguments", &_::m_pFunctionArguments)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantElement")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
