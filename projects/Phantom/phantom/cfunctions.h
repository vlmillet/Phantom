// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

// COMPILER SPECIFIC C FUNCTIONS

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO // VISUAL STUDIO C functions
#    if defined(_UNICODE)
#        define PHANTOM_SPRINTF swprintf
#        define PHANTOM_VSPRINTF vswprintf
#        define PHANTOM_STRCMP wcscmp
#        define PHANTOM_STRLEN wcslen
#    else
#        define PHANTOM_SPRINTF sprintf
#        define PHANTOM_VSPRINTF vsprintf
#        define PHANTOM_STRCMP strcmp
#        define PHANTOM_STRLEN strlen
#    endif

#    define PHANTOM_ITOA(value, buf, size, radix) _itoa_s(value, buf, size, radix)
#    define PHANTOM_SPRINTF_S sprintf_s

#    if PHANTOM_ARCHITECTURE_BITS >= 64
#        define PHANTOM_FSEEK _fseeki64
#    else
#        define PHANTOM_FSEEK fseek
#    endif

#elif (PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG) // GCC C functions
#    if defined(_UNICODE)
#        define PHANTOM_SPRINTF swprintf
#        define PHANTOM_VSPRINTF vswprintf
#        define PHANTOM_STRCMP wcscmp
#        define PHANTOM_STRLEN wcslen
#    else
#        define PHANTOM_SPRINTF sprintf
#        define PHANTOM_VSPRINTF vsprintf
#        define PHANTOM_STRCMP strcmp
#        define PHANTOM_STRLEN strlen
#    endif

#    define PHANTOM_SPRINTF_S snprintf

#    define PHANTOM_ITOA(value, buf, size, radix) itoa(value, buf, radix)

#    define PHANTOM_FSEEK fseek

#endif
