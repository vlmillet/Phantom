// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "CallStackTrace.h"

// clang-format off
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    pragma warning(push, 0)
#    include "windows.h"
#    include "imagehlp.h"
#    pragma comment(lib, "imagehlp.lib")
#    pragma comment(lib, "Shlwapi.lib")
#    pragma warning(pop)
#endif
// clang-format on

static const char* TRUNCATED_STACK_NAME = "Truncated";
static const char* UNKNOWN_STACK_NAME = "Unknown";

namespace phantom
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

class CallStackInitializer
{
public:
    CallStackInitializer()
    {
        HANDLE hProcess = GetCurrentProcess();
        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);

        if (!SymInitialize(hProcess, nullptr, true))
        {
            PHANTOM_ASSERT(false, "SymInitialize failed. error : %d", GetLastError());
        }
    }
};

static void _CallStackInitializer()
{
    static CallStackInitializer Initializer;
}

int CallStackTrace::GetSymbolLine(const void* _ptr)
{
    _CallStackInitializer();
    HANDLE          hProcess = GetCurrentProcess();
    DWORD           dwDisplacement = 0;
    DWORD64         dwAddress = DWORD64(_ptr);
    IMAGEHLP_LINE64 lineInfo;
    ZeroMemory(&lineInfo, sizeof(IMAGEHLP_LINE64));
    if (!SymGetLineFromAddr64(hProcess, dwAddress, &dwDisplacement, &lineInfo))
    {
        return -1;
    }
    return lineInfo.LineNumber;
}
size_t CallStackTrace::GetSymbolName(const void* _ptr, char* _buffer, size_t _bufferSize)
{
    _CallStackInitializer();
    HANDLE  hProcess = GetCurrentProcess();
    DWORD64 dwDisplacement = 0;
    DWORD64 dwAddress = DWORD64(_ptr);

    char         pSymbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)pSymbolBuffer;

    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

#    pragma warning(push, 0)
    if (size_t(_ptr) == ~size_t(0))
    {
        strncpy(_buffer, TRUNCATED_STACK_NAME, _bufferSize);
        return strlen(TRUNCATED_STACK_NAME);
    }
    if (!SymFromAddr(hProcess, dwAddress, &dwDisplacement, pSymbol))
    {
        strncpy(_buffer, UNKNOWN_STACK_NAME, _bufferSize);
        return strlen(UNKNOWN_STACK_NAME);
    }
    strncpy(_buffer, pSymbol->Name, _bufferSize);
#    pragma warning(pop)
    return pSymbol->NameLen;
}

size_t CallStackTrace::Capture(void** backTrace, size_t backTraceSize)
{
    // Note : skip 1 stack frames for this func call
    return size_t(CaptureStackBackTrace(1, (DWORD)backTraceSize, backTrace, NULL));
}

#else

#    pragma message("CallStackTrace not implemented for current platform/compiler")

size_t CallStackTrace::GetSymbolName(const void*, char*, size_t)
{
    return 0;
}

size_t CallStackTrace::Capture(void**, size_t)
{
    return 0;
}

#endif

void CallStackTrace::ToString(const void* const* backTrace, size_t backTraceSize, char* buffer, size_t bufferSize)
{
    if (backTraceSize > 0 && bufferSize > 0)
    {
        for (auto i = 0u; i < backTraceSize; i++)
        {
            size_t count = GetSymbolName(backTrace[i], buffer, bufferSize);
            if (count + 1 >= bufferSize)
            {
                break;
            }
            // add line return
            bufferSize -= count + 1;
            buffer += count + 1;
            *(buffer - 1) = '\n';
        }
        // ensure the String is null terminated
        *(buffer - 1) = 0;
    }
}

} // namespace phantom
