#pragma once

// haunt {

// clang-format off

#include "PlaceholderType.h"

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PlaceholderType")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PlaceholderType)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type, ::phantom::lang::Placeholder>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<bool(LanguageElement*) const, virtual_|override_>("accepts", &_::accepts)({"a_pLanguageElement"})
            .method<Symbol*() const, virtual_|override_>("asSymbol", &_::asSymbol)
            .method<Placeholder*() const, virtual_|override_>("asPlaceholder", &_::asPlaceholder)
            .method<Class*() const>("toClass", &_::toClass)
            .method<ClassType*() const>("toClassType", &_::toClassType)
            .method<PlaceholderType*() const, virtual_|override_>("asPlaceholderType", &_::asPlaceholderType)
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)({""})
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<Placeholder*(LanguageElement*, uint) const, virtual_|override_>("clone", &_::clone)({"a_pOwner","a_Flags"})["0"]
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"","a_Buf"})
            .method<bool() const, virtual_|override_>("isPack", &_::isPack)
        
        .protected_()
            .field("m_pAsClass", &_::m_pAsClass)
            .field("m_pAsClassType", &_::m_pAsClassType)
            .field("m_isPack", &_::m_isPack)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderType")
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
