#pragma once

// haunt {

// clang-format off

#include "Placeholder.h"

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallString.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Placeholder")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Placeholder)
        {
            using StringBuffer = typedef_< phantom::StringBuffer>;
            this_()
        .public_()
            .method<::phantom::lang::Symbol *() const, pure_virtual>("asSymbol", &_::asSymbol)
            .method<::phantom::lang::Placeholder *() const, pure_virtual>("asPlaceholder", &_::asPlaceholder)
            .method<::phantom::lang::PlaceholderType *() const, virtual_>("asPlaceholderType", &_::asPlaceholderType)
            .method<::phantom::lang::PlaceholderConstant *() const, virtual_>("asPlaceholderConstant", &_::asPlaceholderConstant)
            .method<::phantom::lang::PlaceholderClass *() const, virtual_>("asPlaceholderClass", &_::asPlaceholderClass)
            .method<::phantom::lang::PlaceholderTemplate *() const, virtual_>("asPlaceholderTemplate", &_::asPlaceholderTemplate)
            .method<hash64(LanguageElement*) const>("getRelativeHash", &_::getRelativeHash)
            .method<void(LanguageElement*, StringBuffer&) const>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<bool(LanguageElement*) const, virtual_>("accepts", &_::accepts)
            .method<::phantom::lang::Placeholder *(LanguageElement*, uint) const, pure_virtual>("clone", &_::clone)["0"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Placeholder")
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
