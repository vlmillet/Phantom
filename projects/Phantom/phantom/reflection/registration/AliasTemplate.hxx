#pragma once

// haunt {

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

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection.registration")
    PHANTOM_SOURCE("AliasTemplate")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void(TemplateSignature*, StringView)>("SolveAliasTemplateDefaultArguments", SolveAliasTemplateDefaultArguments);}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("AliasTemplate")
PHANTOM_END("phantom.reflection.registration")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
