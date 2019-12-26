#pragma once

// haunt {

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

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("Placeholder")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Placeholder)
        {
            this_()
        .public_()
            .method<bool(Placeholder*) const>("isSame", &_::isSame)
            .method<Symbol*() const, pure_virtual>("asSymbol", &_::asSymbol)
            .method<Placeholder*() const, pure_virtual>("asPlaceholder", &_::asPlaceholder)
            .method<PlaceholderType*() const, virtual_>("asPlaceholderType", &_::asPlaceholderType)
            .method<PlaceholderConstant*() const, virtual_>("asPlaceholderConstant", &_::asPlaceholderConstant)
            .method<PlaceholderClass*() const, virtual_>("asPlaceholderClass", &_::asPlaceholderClass)
            .method<PlaceholderTemplate*() const, virtual_>("asPlaceholderTemplate", &_::asPlaceholderTemplate)
            .method<bool(LanguageElement*) const, virtual_>("accepts", &_::accepts)
            .method<Placeholder*(uint) const, pure_virtual>("clone", &_::clone)["0"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Placeholder")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
