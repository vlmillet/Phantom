#pragma once

// haunt {

#include "OptionalArrayView.h"

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
#include <phantom/alias_template>

#include <phantom/template-only-push>

#include "ArrayView.hxx"
#include "Optional.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("OptionalArrayView")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_ALIAS_TEMPLATE((class), (T), OptionalArrayView, Optional<ArrayView<T> >);
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("OptionalArrayView")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
