// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#if defined(_MSC_VER)
#pragma warning(disable:4068)
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#endif

#ifndef __cplusplus
#    error "Phantom is a C++ Extension, you can't compile it with a C compiler"
#endif

#define __PHANTOM__

// avoid definition of windows min / max macros which conflicts with std ones
#ifndef NOMINMAX
#    define NOMINMAX 1
#endif

//  ==================================================================================================
//        OPERATING SYSTEM VARIABLES
//  ==================================================================================================

#define PHANTOM_OPERATING_SYSTEM_WINDOWS 1
#define PHANTOM_OPERATING_SYSTEM_WINDOWS_PHONE 2
#define PHANTOM_OPERATING_SYSTEM_LINUX 3
#define PHANTOM_OPERATING_SYSTEM_MACOSX 4
#define PHANTOM_OPERATING_SYSTEM_IOS 5
#define PHANTOM_OPERATING_SYSTEM_WINDOWS_CE 6
#define PHANTOM_OPERATING_SYSTEM_ANDROID 7
#define PHANTOM_OPERATING_SYSTEM_ORBIS 8
// ...

#define PHANTOM_OPERATING_SYSTEM_FAMILY_WINDOWS 1
#define PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX 2

//  ==================================================================================================
//        PLATFORMS VARIABLES
//  ==================================================================================================

#define PHANTOM_PLATFORM_WINDOWS_PC 0
#define PHANTOM_PLATFORM_WINDOWS_CE 1
#define PHANTOM_PLATFORM_LINUX_PC 2
#define PHANTOM_PLATFORM_ANDROID 3
#define PHANTOM_PLATFORM_IPHONE_IPAD 4
#define PHANTOM_PLATFORM_IOS_SIMULATOR 5
#define PHANTOM_PLATFORM_MACINTOSH 6
// ...

//  ==================================================================================================
//        PROCESSOR BRAND VARIABLES
//  ==================================================================================================

#define PHANTOM_PROCESSOR_INTEL
#define PHANTOM_PROCESSOR_AMD
// ...

//  ==================================================================================================
//        ARCHITECTURE VARIABLES
//  ==================================================================================================

#define PHANTOM_ARCHITECTURE_X86 0
#define PHANTOM_ARCHITECTURE_X86_64 1
#define PHANTOM_ARCHITECTURE_X64 PHANTOM_ARCHITECTURE_X86_64
#define PHANTOM_ARCHITECTURE_ARM 2
#define PHANTOM_ARCHITECTURE_ARM64 3
// ...

//  ==================================================================================================
//        COMPILER VARIABLES
//  ==================================================================================================

// Visual Studio

#define PHANTOM_COMPILER_VISUAL_STUDIO 1

#define PHANTOM_COMPILER_VISUAL_STUDIO_VERSION _MSC_VER // native predefined version number
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_15 1910
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2017 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_15
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_14 1900
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2015 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_14
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_12 1800
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2013 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_12
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_11 1700
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2012 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_11
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_10 1600
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2010 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_10
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_9 1500
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2008 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_9
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_8 1400
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2005 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_8
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_7 1300
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2003 PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_7
#define PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_6 1200

// Intel

#define PHANTOM_COMPILER_INTEL 2
#define PHANTOM_COMPILER_INTEL_VERSION __INTEL_COMPILER

// GCC
#define PHANTOM_COMPILER_GCC 3

#define PHANTOM_COMPILER_GCC_MAJOR_VERSION_1 1
#define PHANTOM_COMPILER_GCC_MAJOR_VERSION_2 2
#define PHANTOM_COMPILER_GCC_MAJOR_VERSION_3 3
#define PHANTOM_COMPILER_GCC_MAJOR_VERSION_4 4

#define PHANTOM_COMPILER_GCC_MAJOR_VERSION __GNUC__
#define PHANTOM_COMPILER_GCC_MINOR_VERSION __GNUC_MINOR__
#define PHANTOM_COMPILER_GCC_PATCH_VERSION __GNUC_PATCHLEVEL__
#define PHANTOM_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

// Metromerks Codewarrior

#define PHANTOM_COMPILER_CODEWARRIOR 4

// BORLAND C++

#define PHANTOM_COMPILER_BORLAND 5

// CLANG
#define PHANTOM_COMPILER_CLANG 6

//  ==================================================================================================
//        CURRENT PROCESSOR/ARCHITECTURE/PLATFORM/OS/COMPILER VARIABLE VALUES
//  ==================================================================================================

#if defined(__ORBIS__)

#    define PHANTOM_SIZE_OF_VOID_P 8
#    define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86_64
#    define PHANTOM_ARCHITECTURE_BITS 64

#    define PHANTOM_COMPILER_NAME "CLANG"
#    define PHANTOM_COMPILER PHANTOM_COMPILER_CLANG
#    define PHANTOM_COMPILER_VERSION
#    define PHANTOM_COMPILER_MAJOR_VERSION __clang__
#    define PHANTOM_COMPILER_ID_STRING "clang"

#    define PHANTOM_OPERATING_SYSTEM_FAMILY PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
#    define PHANTOM_OPERATING_SYSTEM PHANTOM_OPERATING_SYSTEM_ORBIS

#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) // Microsoft Windows

#    if defined(UNDER_CE) // CE
#        define PHANTOM_PLATFORM PHANTOM_PLATFORM_WINDOWS_CE
#        define PHANTOM_OPERATING_SYSTEM PHANTOM_OPERATING_SYSTEM_WINDOWS_CE
#        define PHANTOM_OPERATING_SYSTEM_FAMILY PHANTOM_OPERATING_SYSTEM_FAMILY_WINDOWS
#        define PHANTOM_OPERATING_SYSTEM_NAME "Windows CE"
#    else // 2000/NT/XP/VISTA/7
#        define PHANTOM_PLATFORM PHANTOM_PLATFORM_WINDOWS_PC
#        define PHANTOM_OPERATING_SYSTEM PHANTOM_OPERATING_SYSTEM_WINDOWS
#        define PHANTOM_OPERATING_SYSTEM_FAMILY PHANTOM_OPERATING_SYSTEM_FAMILY_WINDOWS
#        define PHANTOM_OPERATING_SYSTEM_NAME "Windows"
#    endif

#    if defined(__clang__) // CLANG

#        if defined(__i386__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__x86_64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86_64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        elif defined(__arm__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__arm64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        else
#            error unsupported CPU architecture
#        endif

#        define PHANTOM_COMPILER_NAME "CLANG"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_CLANG
#        define PHANTOM_COMPILER_VERSION
#        define PHANTOM_COMPILER_MAJOR_VERSION __clang__
#        define PHANTOM_COMPILER_ID_STRING "clangcl"

#    elif defined(__GNUG__) // GCC (MINGW32)

#        if defined(__i386__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__x86_64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86_64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        elif defined(__arm__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__arm64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        else
#            error unsupported CPU architecture
#        endif

#        define PHANTOM_COMPILER_NAME "GCC"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_GCC
#        define PHANTOM_COMPILER_VERSION PHANTOM_COMPILER_GCC_VERSION
#        define PHANTOM_COMPILER_MAJOR_VERSION __GNUC__
#        define PHANTOM_COMPILER_ID_STRING "gcc"

#    elif defined(_MSC_VER) // Visual Studio X

#        if defined(_M_IX86)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(_M_X64)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        elif defined(_M_ARM)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(_M_ARM64)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        else
#            error unsupported CPU architecture
#        endif

#        define PHANTOM_COMPILER_NAME "Visual Studio"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_VISUAL_STUDIO
#        define PHANTOM_COMPILER_VERSION _MSC_VER
#        define PHANTOM_COMPILER_MAJOR_VERSION PHANTOM_COMPILER_VERSION
#        if PHANTOM_COMPILER_MAJOR_VERSION == PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2010
#            define PHANTOM_COMPILER_ID_STRING "v110"
#        elif PHANTOM_COMPILER_MAJOR_VERSION == PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2012
#            define PHANTOM_COMPILER_ID_STRING "v111"
#        elif PHANTOM_COMPILER_MAJOR_VERSION == PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2013
#            define PHANTOM_COMPILER_ID_STRING "v112"
#        elif PHANTOM_COMPILER_MAJOR_VERSION == PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2015
#            define PHANTOM_COMPILER_ID_STRING "v140"
#        elif PHANTOM_COMPILER_MAJOR_VERSION == PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2017
#            define PHANTOM_COMPILER_ID_STRING "v141"
#        else
#            define PHANTOM_COMPILER_ID_STRING ""
#        endif
#    endif

#elif defined(__linux__) || defined(__linux) // Linux

#    define PHANTOM_PLATFORM PHANTOM_PLATFORM_LINUX_PC

#    define PHANTOM_OPERATING_SYSTEM_FAMILY PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
#    define PHANTOM_OPERATING_SYSTEM PHANTOM_OPERATING_SYSTEM_LINUX

#    if defined(__clang__) // CLANG

#        if defined(__i386__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__x86_64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86_64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        elif defined(__arm__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__arm64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        else
#            error unsupported CPU architecture
#        endif

#        define PHANTOM_COMPILER_NAME "CLANG"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_CLANG
#        define PHANTOM_COMPILER_VERSION __clang__
#        define PHANTOM_COMPILER_MAJOR_VERSION __clang__
#        define PHANTOM_COMPILER_ID_STRING "clang"

#    elif defined(__GNUC__) // GCC

#        if defined(__i386__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__x86_64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86_64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        elif defined(__arm__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__arm64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        else
#            error unsupported CPU architecture
#        endif

#        define PHANTOM_COMPILER_NAME "GCC"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_GCC
#        define PHANTOM_COMPILER_VERSION PHANTOM_COMPILER_GCC_VERSION
#        define PHANTOM_COMPILER_MAJOR_VERSION __GNUC__
#        define PHANTOM_COMPILER_ID_STRING "gcc"

#    endif

#elif defined(__APPLE__) // Apple Macintosh/IPhone/IPod/IPad

#    define PHANTOM_OPERATING_SYSTEM_FAMILY PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX

#    include "TargetConditionals.h"
#    if TARGET_OS_IPHONE
#        if TARGET_IPHONE_SIMULATOR
#            define PHANTOM_PLATFORM PHANTOM_PLATFORM_IOS_SIMULATOR
#        else
#            define PHANTOM_PLATFORM PHANTOM_PLATFORM_IPHONE_IPAD
#        endif
#        define PHANTOM_OPERATING_SYSTEM PHANTOM_OPERATING_SYSTEM_IOS
#    else
#        define PHANTOM_PLATFORM PHANTOM_PLATFORM_MACINTOSH
#        define PHANTOM_OPERATING_SYSTEM PHANTOM_OPERATING_SYSTEM_MACOSX
#    endif

#    if defined(__clang__) // CLANG

#        if defined(__i386__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__x86_64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_X86_64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        elif defined(__arm__)
#            define PHANTOM_SIZE_OF_VOID_P 4
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM
#            define PHANTOM_ARCHITECTURE_BITS 32
#        elif defined(__arm64__)
#            define PHANTOM_SIZE_OF_VOID_P 8
#            define PHANTOM_ARCHITECTURE PHANTOM_ARCHITECTURE_ARM64
#            define PHANTOM_ARCHITECTURE_BITS 64
#        else
#            error unsupported CPU architecture
#        endif

#        define PHANTOM_COMPILER_NAME "CLANG"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_CLANG
#        define PHANTOM_COMPILER_VERSION
#        define PHANTOM_COMPILER_MAJOR_VERSION __clang__
#        define PHANTOM_COMPILER_ID_STRING "appleclang"

#    elif defined(__GNUC__) // GCC

#        define PHANTOM_COMPILER_NAME "GCC"
#        define PHANTOM_COMPILER PHANTOM_COMPILER_GCC
#        define PHANTOM_COMPILER_VERSION PHANTOM_COMPILER_GCC_VERSION
#        define PHANTOM_COMPILER_MAJOR_VERSION __GNUC__
#        define PHANTOM_COMPILER_ID_STRING "gcc"

#    elif defined(__INTEL_COMPILER) // Intel C++

#        define PHANTOM_COMPILER PHANTOM_COMPILER_INTEL
#        define PHANTOM_COMPILER_VERSION PHANTOM_COMPILER_INTEL_VERSION
#    endif

#endif

// FEATURES

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    if PHANTOM_COMPILER_VISUAL_STUDIO_VERSION >= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2012
#        define PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE 1
#    else
#        define PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE 0
#    endif

#    if defined(_NATIVE_WCHAR_T_DEFINED)
#        define PHANTOM_HAS_BUILT_IN_WCHAR_T 1
#    else
#        define PHANTOM_HAS_BUILT_IN_WCHAR_T 0
#    endif

#    if _HAS_CHAR16_T_LANGUAGE_SUPPORT
#        define PHANTOM_HAS_BUILT_IN_CHAR16_T 1
#        define PHANTOM_HAS_BUILT_IN_CHAR32_T 1

#    else /* _HAS_CHAR16_T_LANGUAGE_SUPPORT */
#        define PHANTOM_HAS_BUILT_IN_CHAR16_T 0
#        define PHANTOM_HAS_BUILT_IN_CHAR32_T 0
#    endif

#    if defined(_CHAR_UNSIGNED)
#        define PHANTOM_HAS_DEFAULT_UNSIGNED_CHAR 1
#    else
#        define PHANTOM_HAS_DEFAULT_UNSIGNED_CHAR 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC

#    if __GXX_EXPERIMENTAL_CXX0X__ || PHANTOM_COMPILER_GCC_VERSION > 40601
#        define PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE 1
#    else
#        define PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE 0
#    endif

#    if defined(__WCHAR_TYPE__) || (__cpp_unicode_characters >= 200704)
#        define PHANTOM_HAS_BUILT_IN_WCHAR_T 1
#    else
#        define PHANTOM_HAS_BUILT_IN_WCHAR_T 0
#    endif

#    if __cpp_unicode_characters >= 200704
#        define PHANTOM_HAS_BUILT_IN_CHAR16_T 1
#        define PHANTOM_HAS_BUILT_IN_CHAR32_T 1
#    else
#        define PHANTOM_HAS_BUILT_IN_CHAR16_T 0
#        define PHANTOM_HAS_BUILT_IN_CHAR32_T 0
#    endif

#    if !defined(__GXX_RTTI)
#        error "'typeid' must be supported ; remove '-fno-rtti' from your compiler options"
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG

#    define PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE 1

#    if __is_identifier(wchar_t)
#        define PHANTOM_HAS_BUILT_IN_WCHAR_T 0
#    else
#        define PHANTOM_HAS_BUILT_IN_WCHAR_T 1
#    endif

#    if __is_identifier(char16_t)
#        define PHANTOM_HAS_BUILT_IN_CHAR16_T 0
#    else
#        define PHANTOM_HAS_BUILT_IN_CHAR16_T 1
#    endif

#    if __is_identifier(char32_t)
#        define PHANTOM_HAS_BUILT_IN_CHAR32_T 0
#    else
#        define PHANTOM_HAS_BUILT_IN_CHAR32_T 1
#    endif

#    if !__has_feature(cxx_rtti)
#        error "phantom cannot work without rtti, remove -fno-rtti in your compiler options"
#    endif

#endif

/// OPTIONAL FEATURES

/// R-VALUE REFERENCES

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    if PHANTOM_COMPILER_MAJOR_VERSION >= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2010
#        define PHANTOM_HAS_RVALUE_REFERENCES 1
#    else
#        define PHANTOM_HAS_RVALUE_REFERENCES 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    if __has_feature(cxx_rvalue_references) || defined(__ORBIS__)
#        define PHANTOM_HAS_RVALUE_REFERENCES 1
#    else
#        define PHANTOM_HAS_RVALUE_REFERENCES 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    if __cpp_rvalue_references >= 200610
#        define PHANTOM_HAS_RVALUE_REFERENCES 1
#    else
#        define PHANTOM_HAS_RVALUE_REFERENCES 0
#    endif

#endif

/// ALIAS TEMPLATES

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    if PHANTOM_COMPILER_MAJOR_VERSION >= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2013
#        define PHANTOM_HAS_ALIAS_TEMPLATES 1
#    else
#        define PHANTOM_HAS_ALIAS_TEMPLATES 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    if __has_feature(cxx_alias_templates) || defined(__ORBIS__)
#        define PHANTOM_HAS_ALIAS_TEMPLATES 1
#    else
#        define PHANTOM_HAS_ALIAS_TEMPLATES 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    if __cpp_alias_templates >= 200704
#        define PHANTOM_HAS_ALIAS_TEMPLATES 1
#    else
#        define PHANTOM_HAS_ALIAS_TEMPLATES 0
#    endif

#endif

/// VARIADIC TEMPLATES

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    if PHANTOM_COMPILER_MAJOR_VERSION >= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2013
#        define PHANTOM_HAS_VARIADIC_TEMPLATES 1
#    else
#        define PHANTOM_HAS_VARIADIC_TEMPLATES 0
#    endif

#    if PHANTOM_COMPILER_MAJOR_VERSION >= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2015
#        define PHANTOM_HAS_REF_QUALIFIERS 1
#    else
#        define PHANTOM_HAS_REF_QUALIFIERS 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    if __has_feature(cxx_variadic_templates)
#        define PHANTOM_HAS_VARIADIC_TEMPLATES 1
#    else
#        define PHANTOM_HAS_VARIADIC_TEMPLATES 0
#    endif

#    if __has_feature(cxx_reference_qualified_functions)
#        define PHANTOM_HAS_REF_QUALIFIERS 1
#    else
#        define PHANTOM_HAS_REF_QUALIFIERS 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    if __cpp_variadic_templates >= 200704
#        define PHANTOM_HAS_VARIADIC_TEMPLATES 1
#    else
#        define PHANTOM_HAS_VARIADIC_TEMPLATES 0
#    endif

#    if __cpp_reference_qualified_functions >= 200704
#        define PHANTOM_HAS_REF_QUALIFIERS 1
#    else
#        define PHANTOM_HAS_REF_QUALIFIERS 0
#    endif

#endif

/// NOEXCEPT

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    if PHANTOM_COMPILER_MAJOR_VERSION >= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2015
#        define PHANTOM_HAS_NOEXCEPT 1
#    else
#        define PHANTOM_HAS_NOEXCEPT 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    if __has_feature(cxx_noexcept)
#        define PHANTOM_HAS_NOEXCEPT 1
#    else
#        define PHANTOM_HAS_NOEXCEPT 0
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    if PHANTOM_COMPILER_GCC_VERSION >= 40600
#        define PHANTOM_HAS_NOEXCEPT 1
#    else
#        define PHANTOM_HAS_NOEXCEPT 0
#    endif

#endif

/// REQUIRED FEATURES

/// DECLTYPE

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    if PHANTOM_COMPILER_MAJOR_VERSION < PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2010
#        error "'decltype' must be supported by your compiler"
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    if !__has_feature(cxx_decltype)
#        error "'decltype' must be supported by your compiler"
#    endif

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    if __cpp_decltype < 200707
#        error "'decltype' must be supported by your compiler"
#    endif

#endif

/// CALLING CONVENTIONS / ABIs

#if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86
#    define PHANTOM_HAS_FASTCALL 1
#    define PHANTOM_HAS_STDCALL 1
#else
#    define PHANTOM_HAS_FASTCALL 0
#    define PHANTOM_HAS_STDCALL 0
#endif

/// DEBUG LEVELS

#define PHANTOM_DEBUG_LEVEL_FULL 2
#define PHANTOM_DEBUG_LEVEL_ASSERTS 1
#define PHANTOM_DEBUG_LEVEL_NONE 0

#if defined(_DEBUG)
#    define PHANTOM_DEBUG_LEVEL PHANTOM_DEBUG_LEVEL_FULL

#elif defined(NDEBUG)
#    define PHANTOM_DEBUG_LEVEL PHANTOM_DEBUG_LEVEL_NONE

#else
#    define PHANTOM_DEBUG_LEVEL PHANTOM_DEBUG_LEVEL_ASSERTS

#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_MAXIMUM_DECLARATION_COUNTER 800
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    define PHANTOM_MAXIMUM_DECLARATION_COUNTER 800
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    define PHANTOM_MAXIMUM_DECLARATION_COUNTER 800
#else
#    error TODO : define PHANTOM_MAXIMUM_DECLARATION_COUNTER for all supported compilers
#    define PHANTOM_MAXIMUM_DECLARATION_COUNTER 800
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_IF_COMPILER_VISUAL_STUDIO(...) __VA_ARGS__
#    define PHANTOM_IF_NOT_COMPILER_VISUAL_STUDIO(...)
#    if PHANTOM_COMPILER_VERSION <= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2013
#        define PHANTOM_IF_COMPILER_VISUAL_STUDIO_2013_OR_OLDER(...) __VA_ARGS__
#        define PHANTOM_IF_COMPILER_VISUAL_STUDIO_2015_OR_GREATER(...)
#    else
#        define PHANTOM_IF_COMPILER_VISUAL_STUDIO_2013_OR_OLDER(...)
#        define PHANTOM_IF_COMPILER_VISUAL_STUDIO_2015_OR_GREATER(...) __VA_ARGS__
#    endif
#else
#    define PHANTOM_IF_COMPILER_VISUAL_STUDIO(...)
#    define PHANTOM_IF_NOT_COMPILER_VISUAL_STUDIO(...) __VA_ARGS__
#    define PHANTOM_IF_COMPILER_VISUAL_STUDIO_2013_OR_OLDER(...)
#    define PHANTOM_IF_COMPILER_VISUAL_STUDIO_2015_OR_GREATER(...)
#endif

#if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86_64
#    define PHANTOM_IF_ARCHITECTURE_X64(...) __VA_ARGS__
#    define PHANTOM_IF_NOT_ARCHITECTURE_X64(...)
#else
#    define PHANTOM_IF_ARCHITECTURE_X64(...)
#    define PHANTOM_IF_NOT_ARCHITECTURE_X64(...) __VA_ARGS__
#endif

#if !PHANTOM_HAS_VARIADIC_TEMPLATES
#    error C++11 variadic templates are required, please use a more recent version of your compiler (MSVC >= 2015 or clang >= 3.7)
#endif

#if !PHANTOM_HAS_ALIAS_TEMPLATES
#    error C++11 alias templates are required, please use a more recent version of your compiler (MSVC >= 2015 or clang >= 3.7)
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS
#    define PHANTOM_SIZE_OF_LONG 8
#    define PHANTOM_SIZE_OF_LONGLONG 8
#    define PHANTOM_INT64_IS_LONGLONG 0
#    define PHANTOM_INT64_IS_LONG 1

#elif PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_SIZE_OF_LONG 4
#    define PHANTOM_SIZE_OF_LONGLONG 8
#    define PHANTOM_INT64_IS_LONGLONG 1
#    define PHANTOM_INT64_IS_LONG 0

#else
#    define PHANTOM_SIZE_OF_LONG 8
#    define PHANTOM_SIZE_OF_LONGLONG 8
#    define PHANTOM_INT64_IS_LONGLONG 0
#    define PHANTOM_INT64_IS_LONG 1

#endif

#define PHANTOM_LITTLE_ENDIAN 1
#define PHANTOM_BIG_ENDIAN 0

#if PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86 || PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X64
#    define PHANTOM_ENDIANESS PHANTOM_LITTLE_ENDIAN
#else
#    define PHANTOM_ENDIANESS PHANTOM_LITTLE_ENDIAN
#endif

// BIG ENDIAN NOT SUPPORTED
#define PHANTOM_ENDIANESS_SWAP 0

#if PHANTOM_ENDIANESS_SWAP

#    define PHANTOM_SWAP16(x) (((x & 0x00ff) << 8) | ((x & 0xff00) >> 8))

#    define PHANTOM_SWAP32(x)                                                                                          \
        (((x & 0x000000ff) << 24) | ((x & 0xff000000) >> 24) | ((x & 0x00ff0000) >> 8) | ((x & 0x0000ff00) << 8))

#    define PHANTOM_SWAP64(x)                                                                                          \
        (((x & 0x00000000000000ff) << 56) | ((x & 0xff00000000000000) >> 56) | ((x & 0x000000000000ff00) << 40) |      \
         ((x & 0x00ff000000000000) >> 40) | ((x & 0x0000000000ff0000) << 24) | ((x & 0x0000ff0000000000) >> 24) |      \
         ((x & 0x00000000ff000000) << 8) | ((x & 0x000000ff00000000) >> 8))
#else

#    define PHANTOM_SWAP16(x) (x)
#    define PHANTOM_SWAP32(x) (x)
#    define PHANTOM_SWAP64(x) (x)

#endif

#if 0
/// @brief this macro must be defined inside your static lib project configuration so that Phantom can sort your lang symbols when using static compilation.
#    define PHANTOM_STATIC_LIB_HANDLE
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR16_T
#    define PHANTOM_IF_CHAR16_T(...) __VA_ARGS__
#else
#    define PHANTOM_IF_CHAR16_T(...)
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR32_T
#    define PHANTOM_IF_CHAR32_T(...) __VA_ARGS__
#else
#    define PHANTOM_IF_CHAR32_T(...)
#endif

#if PHANTOM_HAS_BUILT_IN_WCHAR_T
#    define PHANTOM_IF_WCHAR_T(...) __VA_ARGS__
#else
#    define PHANTOM_IF_WCHAR_T(...)
#endif
