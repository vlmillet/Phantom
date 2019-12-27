#pragma once

// haunt {

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

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("DataElement")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(DataElement)
        {
            using Fields = typedef_< phantom::reflection::Fields>;
            this_()
        .public_()
            .method<size_t() const, pure_virtual>("getAlignment", &_::getAlignment)
            .method<size_t() const, pure_virtual>("getSize", &_::getSize)
            .method<size_t() const, pure_virtual>("getOffset", &_::getOffset)
            .method<void(size_t), pure_virtual>("setOffset", &_::setOffset)
            .method<void(Fields&) const, pure_virtual>("fetchFields", &_::fetchFields)
            .method<Symbol*() const>("asSymbol", &_::asSymbol)
        
        .protected_()
            .constructor<void(Symbol*)>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("DataElement")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
