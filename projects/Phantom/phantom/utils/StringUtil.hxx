#pragma once

// haunt {

// clang-format off

#include "StringUtil.h"

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
#include <phantom/static_method>

#include <phantom/template-only-push>

#include "SmallString.hxx"
#include "SmallVector.hxx"
#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("StringUtil")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(StringUtil)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            using StringViews = typedef_< phantom::StringViews>;
            using Strings = typedef_< phantom::Strings>;
            this_()
            .staticMethod<void(String&, StringView, StringView)>("ReplaceAll", &_::ReplaceAll)
            .staticMethod<void(Strings&, StringView, const char*, bool)>("Split", &_::Split)["true"]
            .staticMethod<void(StringViews&, StringView, const char*, bool)>("Split", &_::Split)["true"]
            .staticMethod<StringView(StringView)>("RemoveExtraBlanks", &_::RemoveExtraBlanks)
            .staticMethod<bool(char)>("IsBlank", &_::IsBlank)
            .staticMethod<String(float, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(double, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(char, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(sint8, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(uint8, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(sint16, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(uint16, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(sint32, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(uint32, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(longlong, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(ulonglong, const char*)>("ToString", &_::ToString)["nullptr"]
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .staticMethod<String(sint64, const char*)>("ToString", &_::ToString)["nullptr"]
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .staticMethod<String(uint64, const char*)>("ToString", &_::ToString)["nullptr"]
            .staticMethod<String(long double, const char*)>("ToString", &_::ToString)["nullptr"]
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("StringUtil")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
