// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include <phantom/constant>
#include <phantom/function>
#include <phantom/package>
#include <phantom/source>
#include <stdio.h>
#include <stdlib.h>

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include <windows.h>
#endif
/* *********************************************** */

extern "C"
{
    void* __cdecl Phantom_AssemblyVarArgCall(void*, size_t, char*, size_t, size_t)
    {
        PHANTOM_ASSERT(false, "waiting for assembly implementation");
        return nullptr;
    }
}

#if defined(_MSC_VER)
#    pragma warning(push, 0)
#elif defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wall"
#    pragma clang diagnostic ignored "-Wextra"
#endif

PHANTOM_PACKAGE("c")
PHANTOM_SOURCE("std")

PHANTOM_REGISTER(Variables)
{
    (this_()).constant("NULL", nullptr); // we don't want to keep old NULL macros as pure 0 int constant
}

PHANTOM_FUNCTION(void*, malloc, (size_t));
PHANTOM_FUNCTION(void, free, (void*));
PHANTOM_FUNCTION(void*, realloc, (void*, size_t));
PHANTOM_FUNCTION(void*, memmove, (void*, void const*, size_t));
PHANTOM_FUNCTION(void*, memcpy, (void*, void const*, size_t));
PHANTOM_FUNCTION(void*, memset, (void*, int, size_t));
PHANTOM_FUNCTION(int, printf, (const char*, ...));
PHANTOM_FUNCTION(int, rand, ());
PHANTOM_FUNCTION(float, sinf, (float));
PHANTOM_FUNCTION(float, cosf, (float));
PHANTOM_FUNCTION(float, tanf, (float));
PHANTOM_FUNCTION(float, asinf, (float));
PHANTOM_FUNCTION(float, acosf, (float));
PHANTOM_FUNCTION(float, atanf, (float));
PHANTOM_FUNCTION(float, fabs, (float));
PHANTOM_FUNCTION(float, powf, (float, float));
PHANTOM_FUNCTION(float, sqrtf, (float));
PHANTOM_FUNCTION(int, toupper, (int));
PHANTOM_FUNCTION(size_t, strlen, (char const*));
PHANTOM_FUNCTION(char*, strcpy, (char*, char const*));
PHANTOM_FUNCTION(char*, strncpy, (char*, char const*, size_t));
PHANTOM_FUNCTION(int, strncmp, (char const*, char const*, size_t));
PHANTOM_FUNCTION(const char*, strstr, (const char* str1, const char* str2));
PHANTOM_FUNCTION(char*, strstr, (char* str1, const char* str2));

#if PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
PHANTOM_FUNCTION(int, system, (const char* cmd));
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    pragma warning(disable : 4996)
PHANTOM_FUNCTION(char*, itoa, (int, char*, int));
#    pragma warning(default : 4996)
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
PHANTOM_FUNCTION(void, DebugBreak, ());
#else
void BuiltinTrapWrapper()
{
    __builtin_trap();
}
PHANTOM_FUNCTION(void, BuiltinTrapWrapper, ());
#endif

PHANTOM_END("std");
PHANTOM_END("c");
