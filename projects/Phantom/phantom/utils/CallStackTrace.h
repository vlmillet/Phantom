#pragma once

#include <phantom/plugin.h>

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM CallStackTrace
{
    // retrieve current call stack
    static size_t Capture(void** backTrace, size_t backTraceSize);

    // convert callstack into readable String
    static void ToString(const void* const* backTrace, size_t backTraceSize, char* buffer, size_t bufferSize);

    // return symbol line
    static int GetSymbolLine(const void* _ptr);

    // return readable symbol name
    static size_t GetSymbolName(const void* pt, char* buffer, size_t buffersize);
};
} // namespace phantom
