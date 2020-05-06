// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "History.h"

#include "CallStackTrace.h"

#include <stdarg.h>

namespace phantom
{
History::Entry::Entry(const char* _text) : text(_text)
{
    void* funcs[callStackSize + 1];
    CallStackTrace::Capture(
    funcs, callStackSize + 1); // +2 because we skip current constructor and caller (which is "History::push")
    memcpy(callStack, funcs + 1, callStackSize * sizeof(void*));
}

const size_t History::callStackSize;

void History::record(const char* _Format, ...)
{
    char    buffer[1024];
    int     _Result;
    va_list _ArgList;
    va_start(_ArgList, _Format);
    _Result = vsnprintf(buffer, 1024, _Format, _ArgList);
    entries.push_back(buffer);
    va_end(_ArgList);
}

void History::record(StringView _view)
{
    record("%.*s", (int)_view.size(), _view.data());
}

void History::record()
{
    record("");
}

void History::dump()
{
    for (Entry const& entry : entries)
    {
        printf("%s\n", entry.text.c_str());
        char buffer[512];
        CallStackTrace::ToString(entry.callStack, callStackSize, buffer, 512);
        printf("%s", buffer);
    }
}

} // namespace phantom
