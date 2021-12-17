#pragma once

// haunt {

// clang-format off

#include "AliasTemplate.h"

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
#include <phantom/function>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang.registration")
    PHANTOM_SOURCE("AliasTemplate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void(TemplateSignature*, StringView)>("SolveAliasTemplateDefaultArguments", SolveAliasTemplateDefaultArguments)({"a_pTS","a_Defaults"});}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AliasTemplate")
PHANTOM_END("phantom.lang.registration")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
