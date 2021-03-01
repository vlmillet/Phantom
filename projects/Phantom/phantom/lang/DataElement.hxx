#pragma once

// haunt {

// clang-format off

#include "DataElement.h"

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

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("DataElement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(DataElement)
        {
            this_()
        .public_()
            .method<size_t() const, pure_virtual>("getAlignment", &_::getAlignment)
            .method<size_t() const, pure_virtual>("getSize", &_::getSize)
            .method<size_t() const, pure_virtual>("getOffset", &_::getOffset)
            .method<void(size_t), pure_virtual>("setOffset", &_::setOffset)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(Fields&) const, pure_virtual>("fetchFields", &_::fetchFields)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<int*() const>("asSymbol", &_::asSymbol)
        
        .protected_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .constructor<void(Symbol*)>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DataElement")
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
