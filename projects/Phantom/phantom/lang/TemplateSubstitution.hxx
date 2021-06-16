#pragma once

// haunt {

// clang-format off

#include "TemplateSubstitution.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("TemplateSubstitution")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(TemplateSubstitution)
        {
            this_()
        .public_()
            .constructor<void()>()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .constructor<void(TemplateSpecialization*)>()({"a_pInstantiation"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(Placeholder*, LanguageElement*)>("insert", &_::insert)({"a_pPlaceholder","a_pArgument"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(Placeholder*, LanguageElement*)>("substituteVariadic", &_::substituteVariadic)({"a_pPlaceholder","a_pArgument"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int*(size_t) const>("getPlaceholder", &_::getPlaceholder)({"i"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int*(size_t) const>("getArgument", &_::getArgument)({"i"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<TemplateSubstitution(Placeholder*, LanguageElements&) const>("splitVariadics", &_::splitVariadics)({"a_pPH","a_VariadicExpansion"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int*(Placeholder*) const>("getArgument", &_::getArgument)({"a_pPlaceholder"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(TemplateSpecialization*)>("setInstantiation", &_::setInstantiation)({"a_pSpec"})
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int*() const>("getInstantiation", &_::getInstantiation)
            .method<size_t() const>("size", &_::size)
            .method<TemplateSubstitution const*() const>("getEnclosingSubstitution", &_::getEnclosingSubstitution)
            .method<void(TemplateSubstitution const*)>("setEnclosingSubstitution", &_::setEnclosingSubstitution)({"a_pSubs"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("TemplateSubstitution")
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
