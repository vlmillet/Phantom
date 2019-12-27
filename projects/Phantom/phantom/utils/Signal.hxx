#pragma once

// haunt {

#include "Signal.h"

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
#include <phantom/function>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Signal")

        PHANTOM_CLASS_T((class, class), (Sign, SignalMutexType), Signal)
        {
            this_()
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void*()>("Emitter", Emitter);}
        /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
        /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
        #endif // PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS_TVS((class), (SignalMutexType), (class, Parms), (void(Parms...), SignalMutexType), Signal)
        {
            this_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            .PHANTOM_T inherits<>()
            ;
        }
    PHANTOM_END("Signal")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
