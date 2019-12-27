// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/config.h>

#if defined(PHANTOM_CUSTOM_FORCED_INCLUDE)
#    include PHANTOM_CUSTOM_FORCED_INCLUDE
#endif

// FEATURES activation / deactivation

// internals (alpha) (don't touch)
#if !defined(PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_MODE)
#    define PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_MODE                                                                    \
        1 /// (DON'T TOUCH) means raii and database functions
          /// (initialize/terminate/restore/serialize...) extension will be solved at compile time
          /// with meta-programming to optimize runtime code => requires additional .hxx hierarchy
          /// to declare inherited classes (else raii extension will be dynamic, slower, more
          /// flexible and don't require additional .hxx file)
#endif
#if !defined(PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_SIGNATURE)
#    define PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_SIGNATURE                                                               \
        1 /// (DON'T TOUCH) Signatures in functions and member functions are solved at compile time
          /// instead of being parsed
#endif
#if !defined(PHANTOM_CUSTOM_ENABLE_SLOT_POOL)
#    define PHANTOM_CUSTOM_ENABLE_SLOT_POOL                                                                            \
        1 /// (DON'T TOUCH) add connection pool allocator on EmbeddedRtti Data which will
          /// automatically release slots on receiver destruction (PHANTOM_DYN_CONNECT can only be
          /// used after installation pass)
#endif

// public (bêta) (you can play with it but it is not recommanded at all)
#if !defined(PHANTOM_CUSTOM_USE_PRECISE_SIGNATURE)
#    define PHANTOM_CUSTOM_USE_PRECISE_SIGNATURE                                                                       \
        0 /// Use real template signatures instead of proxy/simplified ones (for ex: by default
          /// MyClass* (MyClass::*)(MyClass&) const will be simplified to void(DummyClass::*)(int&)
          /// for template argument passing). This increases heavily compile time as a lot of
          /// template instances will be created for each function signature which differs from
          /// another.
#endif
#if !defined(PHANTOM_CUSTOM_ENABLE_INSTANTIATION_SIGNALS)
#    define PHANTOM_CUSTOM_ENABLE_INSTANTIATION_SIGNALS                                                                \
        1 /// if enabled, on each object creation, a signal will be emitted from its meta-class to
          /// notify that an instance of it's type has been created, same for termination or
          /// restoration
#endif

// public (ok) (you can play with it if you want if you know what you are doing)
#if !defined(PHANTOM_CUSTOM_ENABLE_ALLOCATION_INFOS)
#    define PHANTOM_CUSTOM_ENABLE_ALLOCATION_INFOS                                                                     \
        1 /// Enable statitistics (file, line, count ...) on phantom class allocations (for stl
          /// class it wont generate any file/line information, but it's not a probleme since stl is
          /// safe)
#endif
#if !defined(PHANTOM_CUSTOM_ENABLE_SIGNAL_BLOCKING)
#    define PHANTOM_CUSTOM_ENABLE_SIGNAL_BLOCKING 0 /// enable o_block and o_unblock keywords, usable on signals
#endif
#if !defined(PHANTOM_CUSTOM_THREAD_SAFE)
#    define PHANTOM_CUSTOM_THREAD_SAFE 1 /// enable thread safe mutex based signal/slot connections
#endif
#if !defined(PHANTOM_CUSTOM_THREAD_SAFE_CONNECTIONS)
#    define PHANTOM_CUSTOM_THREAD_SAFE_CONNECTIONS                                                                     \
        PHANTOM_CUSTOM_THREAD_SAFE /// enable thread safe mutex based signal/slot connections
#endif

#if !defined(PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE)
#    define PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE 1
#endif

#if !defined(PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY)
#    define PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY 8192
#endif
