#pragma once

// haunt {

// clang-format off

#include "Pair.h"

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
#include <phantom/constructor>
#include <phantom/field>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Pair")

        PHANTOM_STRUCT_T((class, class), (t_First, t_Second), Pair)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void()>()
            )
            .PHANTOM_T constructor<void(t_First, t_Second)>()
            .PHANTOM_T field("first", &_::first)
            .PHANTOM_T field("second", &_::second)
            ;
        }
    PHANTOM_END("Pair")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Pair")

        /// missing symbol(s) reflection (phantom::IsCopyAssignable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, class), (K, V), (), IsCopyAssignable)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsCopyAssignable<K>::value || !IsCopyAssignable<V>::value)>>()
            ;
        } */
    PHANTOM_END("Pair")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Pair")

        /// missing symbol(s) reflection (phantom::IsMoveAssignable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, class), (K, V), (), IsMoveAssignable)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsMoveAssignable<K>::value || !IsMoveAssignable<V>::value)>>()
            ;
        } */
    PHANTOM_END("Pair")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Pair")

        /// missing symbol(s) reflection (phantom::IsCopyConstructible) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, class), (K, V), (), IsCopyConstructible)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsCopyConstructible<K>::value || !IsCopyConstructible<V>::value)>>()
            ;
        } */
    PHANTOM_END("Pair")
PHANTOM_END("phantom.utils")
}
namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Pair")

        /// missing symbol(s) reflection (phantom::IsMoveConstructible) -> use the 'haunt.bind' to bind symbols with your custom haunt files
        /* PHANTOM_STRUCT_TS((class, class), (K, V), (), IsMoveConstructible)
        {
            this_()
            // .PHANTOM_T inherits<::std::integral_constant<bool, !(!IsMoveConstructible<K>::value || !IsMoveConstructible<V>::value)>>()
            ;
        } */
    PHANTOM_END("Pair")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }


