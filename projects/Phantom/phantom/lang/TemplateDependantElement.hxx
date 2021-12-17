#pragma once

// haunt {

// clang-format off

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
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateDependantElement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(TemplateDependantElement)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(LanguageElement*, StringView, OptionalArrayView<LanguageElement*>, OptionalArrayView<LanguageElement*>, Modifiers, uint)>()({"a_pLeft","a_strName","a_pTemplateArguments","a_pFunctionArguments","modifiers","a_uiFlags"})["NullOpt"]["NullOpt"]["0"]["0"]
            .method<void()>("initialize", &_::initialize)
            .method<Type*() const, virtual_|override_>("toType", &_::toType)
            .method<LanguageElement*() const>("getLeft", &_::getLeft)
            .method<void(LanguageElement*, StringBuffer&) const>("getRelativeDecoration", &_::getRelativeDecoration)({"a_pTo","a_Buf"})
            .method<void(StringBuffer&) const>("getDecoration", &_::getDecoration)({"a_Buf"})
            .method<void(StringBuffer&) const>("getQualifiedDecoration", &_::getQualifiedDecoration)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"a_pTo","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"a_pTo","a_Buf"})
            .method<Optional<LanguageElements> const&() const>("getTemplateArguments", &_::getTemplateArguments)
            .method<Optional<LanguageElements> const&() const>("getFunctionArguments", &_::getFunctionArguments)
        
        .protected_()
            .field("m_pAsType", &_::m_pAsType)
            .field("m_pLeft", &_::m_pLeft)
            .field("m_pTemplateArguments", &_::m_pTemplateArguments)
            .field("m_pFunctionArguments", &_::m_pFunctionArguments)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateDependantElement")
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
