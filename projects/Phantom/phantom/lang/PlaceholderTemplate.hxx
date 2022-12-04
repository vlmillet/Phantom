#pragma once

// haunt {

// clang-format off

#include "PlaceholderTemplate.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("PlaceholderTemplate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(PlaceholderTemplate)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Template, ::phantom::lang::Placeholder>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .constructor<void(StringView, TemplateSignature*)>()({"a_strName","a_pSignature"})
            .method<bool(LanguageElement*) const, virtual_|override_>("accepts", &_::accepts)({"a_pLanguageElement"})
            .method<bool(Type*) const, virtual_>("isA", &_::isA)({""})
            .method<bool() const, virtual_|override_>("isPack", &_::isPack)
            .method<Symbol*() const, virtual_|override_>("asSymbol", &_::asSymbol)
            .method<Placeholder*() const, virtual_|override_>("asPlaceholder", &_::asPlaceholder)
            .method<PlaceholderTemplate*() const, virtual_|override_>("asPlaceholderTemplate", &_::asPlaceholderTemplate)
            .method<Placeholder*(LanguageElement*, uint) const, virtual_|override_>("clone", &_::clone)({"a_pOwner","a_Flags"})["0"]
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)({"a_Buf"})
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)({"a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)({"","a_Buf"})
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeName", &_::getRelativeName)({"","a_Buf"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("PlaceholderTemplate")
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
