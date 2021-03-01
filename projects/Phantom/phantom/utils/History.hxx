#pragma once

// haunt {

// clang-format off

#include "History.h"

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
#include <phantom/static_constant>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>

#include <phantom/template-only-push>

#include "SmallString.hxx"
#include "SmallVector.hxx"
#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("History")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(History)
        {
            using Entry = typedef_<_::Entry>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .constant("callStackSize", _::callStackSize)
            .struct_<Entry>()
                .constructor<void(), default_>()
                .constructor<void(const char*)>()
                .field("text", &_::Entry::text)
                .field("callStack", &_::Entry::callStack)
            .end()
            .method<void(StringView)>("record", &_::record)
            .method<void()>("record", &_::record)
            .method<void()>("pop", &_::pop)
            .method<void()>("dump", &_::dump)
            .field("entries", &_::entries)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("History")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
