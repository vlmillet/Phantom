#pragma once

// haunt {

#include "exceptions.h"

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
#include <phantom/constructor>

namespace phantom {
namespace exception {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("exceptions")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Exception)
        {
            this_()
            .inherits</* ::std::exception */>()
        .public_()
            .constructor<void()>()
            .constructor<void(const char* const&)>()
            ;
        }
        PHANTOM_CLASS(InvalidArgumentException)
        {
            this_()
            .inherits<::phantom::exception::Exception>()
        .public_()
            .constructor<void()>()
            .constructor<void(const char* const&)>()
            ;
        }
        PHANTOM_CLASS(UnsupportedMethodException)
        {
            this_()
            .inherits<::phantom::exception::Exception>()
        .public_()
            .constructor<void()>()
            .constructor<void(const char* const&)>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("exceptions")
PHANTOM_END("phantom")
}
namespace reflection {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("exceptions")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ParsingException)
        {
            this_()
            .inherits<::phantom::exception::Exception>()
        .public_()
            .constructor<void()>()
            .constructor<void(const char* const&)>()
            ;
        }
        PHANTOM_CLASS(RuntimeException)
        {
            this_()
            .inherits<::phantom::exception::Exception>()
        .public_()
            .constructor<void()>()
            .constructor<void(const char* const&)>()
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("exceptions")
PHANTOM_END("phantom")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
