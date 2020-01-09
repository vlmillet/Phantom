// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/preprocessor.h>

#if defined(__HAUNT__)

/// utils
#    define _HNT_CAT_I(x, y) x##y
#    define _HNT_CAT(x, y) _HNT_CAT_I(x, y)

/// code generation control

// INDIRECT annotations
#    define HAUNT_METADATA(k, v)                                                                                       \
        [[clang::annotate("haunt::meta:" #k "=" #v)]] void _HNT_CAT(_HAUNT_META, __COUNTER__)();
#    define HAUNT_METADATAS(seq) [[clang::annotate("haunt::metas:" #    seq)]] void _HNT_CAT(_HAUNT_META, __COUNTER__)();
#    define HAUNT_SLOTS : virtual void _HNT_CAT(_HAUNT_ACCESS_slots_, __COUNTER__)();                                  \
    public
#    define HAUNT_FLAGS(flags) [[clang::annotate("haunt::flags:" #    flags)]] _HNT_CAT(_HAUNT_FLAGS, __COUNTER__)();
#    define HAUNT_MODIFIERS(modifiers)                                                                                 \
        [[clang::annotate("haunt::modifiers:" #modifiers)]] _HNT_CAT(_HAUNT_MODIFIERS, __COUNTER__)();
#    define HAUNT_RAW(...) HAUNT_TEXT(#    __VA_ARGS__)
#    define HAUNT_UNIQUE_RAW(...) HAUNT_UNIQUE_TEXT(#    __VA_ARGS__)
#    define HAUNT_TEXT(text) [[clang::annotate(text)]] void _HNT_CAT(_HAUNT_RAW, __COUNTER__)();
#    define HAUNT_UNIQUE_TEXT(text) [[clang::annotate(text)]] void _HNT_CAT(_HAUNT_UNIQUE_RAW, __COUNTER__)();

// - source / package control
#    define HAUNT_DIR(name) [[clang::annotate(name)]] void _HNT_CAT(_HAUNT_DIR, __COUNTER__)();
#    define HAUNT_FILE(name) [[clang::annotate(name)]] void _HNT_CAT(_HAUNT_FILE, __COUNTER__)();
#    define HAUNT_PACKAGE(name) [[clang::annotate(name)]] void _HNT_CAT(_HAUNT_PACKAGE, __COUNTER__)();
#    define HAUNT_SOURCE(name) [[clang::annotate(name)]] void _HNT_CAT(_HAUNT_SOURCE, __COUNTER__)();
#    define HAUNT_END(name) [[clang::annotate("" name)]] void _HNT_CAT(_HAUNT_END, __COUNTER__)();
// - parsing control
#    define HAUNT_ON void _HNT_CAT(_HAUNT_ANNOTATE_on_, __COUNTER__)();
#    define HAUNT_OFF void _HNT_CAT(_HAUNT_ANNOTATE_off_, __COUNTER__)();
#    define HAUNT_SKIP HAUNT_OFF
#    define HAUNT_HIDDEN HAUNT_OFF
#    define HAUNT_STOP void _HNT_CAT(_HAUNT_STOP, __COUNTER__)();
#    define HAUNT_RESUME void _HNT_CAT(_HAUNT_RESUME, __COUNTER__)();
#    define HAUNT_PAUSE void _HNT_CAT(_HAUNT_PAUSE, __COUNTER__)();

// DIRECT annotations
#    define HAUNT_ANNOTATE(quotedString)                                                                               \
        [[clang::annotate(quotedString)]] // user one (end up in phantom::lang::Symbol::getAnnotations()
#    define HAUNT_SLOT [[clang::annotate("haunt::slot")]]
#    define HAUNT_STRUCTURE [[clang::annotate("haunt::structure")]]
#    define HAUNT_FILTERMASK(filter) [[clang::annotate("haunt::filtermask:" #    filter)]]
#    define HAUNT_NOTIFY(name) [[clang::annotate("haunt::notify:" #    name)]]
#    define HAUNT_GET(name) [[clang::annotate("haunt::get:" #    name)]]
#    define HAUNT_SET(name) [[clang::annotate("haunt::set:" #    name)]]
#    define HAUNT_FILTERMASK(arg) [[clang::annotate("haunt::save:" PHANTOM_PP_QUOTE(arg))]]
#    define HAUNT_TRANSIENT [[clang::annotate("haunt::transient")]]
#    define HAUNT_PROPERTY(type, name, ...)                                                                            \
        [[clang::annotate("haunt::property:" PHANTOM_PP_QUOTE(PHANTOM_PP_REMOVE_PARENS(                                \
        type)) "$" #name "$" #__VA_ARGS__)]] type PHANTOM_PP_CAT(_HAUNT_PROPERTY, __COUNTER__)();

#else

#    define HAUNT_ANNOTATE(...)
#    define HAUNT_ON
#    define HAUNT_OFF
#    define HAUNT_HIDDEN
#    define HAUNT_SKIP
#    define HAUNT_SLOT
#    define HAUNT_SLOTS
#    define HAUNT_METADATA(k, v)
#    define HAUNT_METADATAS(seq)
#    define HAUNT_STRUCTURE
#    define HAUNT_FILTERMASK(filter)
#    define HAUNT_NOTIFY(name)
#    define HAUNT_GET(name)
#    define HAUNT_SET(name)
#    define HAUNT_TRANSIENT
#    define HAUNT_STOP
#    define HAUNT_RESUME
#    define HAUNT_PAUSE
#    define HAUNT_DIR(name)
#    define HAUNT_FILE(name)
#    define HAUNT_PACKAGE(name)
#    define HAUNT_SOURCE(name)
#    define HAUNT_END(...)
#    define HAUNT_FLAGS(flags)
#    define HAUNT_MODIFIERS(modifiers)
#    define HAUNT_RAW(...)
#    define HAUNT_UNIQUE_RAW(...)
#    define HAUNT_TEXT(text)
#    define HAUNT_UNIQUE_TEXT(text)

#    define HAUNT_PROPERTY(type, name, ...)

#endif

#define x_min_level 0
#define x_default_level 3
#define x_max_level 5
