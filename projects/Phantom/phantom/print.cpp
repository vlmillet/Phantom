// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Delegate.h"
#include "lexical_cast.h"
#include "print.h"

#include <phantom/SmallMap.h>
/* *********************************************** */

namespace phantom
{
static SmallMap<int, PrintDelegate> g_print_delegates;

PHANTOM_EXPORT_PHANTOM void print(const char* a_strText, int channel /*= 0*/, int options /*= 0*/)
{
    print(StringView(a_strText), channel, options);
}

PHANTOM_EXPORT_PHANTOM void print(StringView a_strText, int channel /*= 0*/, int options /*= 0*/)
{
    auto found = g_print_delegates.find(channel);
    if (found == g_print_delegates.end())
    {
        g_print_delegates[0](a_strText, options);
    }
    else
    {
        found->second(a_strText, options);
    }
}

PHANTOM_EXPORT_PHANTOM void setPrintDelegate(int a_channel, const PrintDelegate& a_delegate)
{
    if (a_delegate.empty())
        g_print_delegates.erase(a_channel);
    else
        g_print_delegates[a_channel] = a_delegate;
}

PHANTOM_EXPORT_PHANTOM void print(bool v, int channel /*= 0*/, int options /*= 0*/)
{
    print(v ? "true" : "false", channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(char v, int channel /*= 0*/, int options /*= 0*/)
{
    char str[2];
    str[0] = v;
    str[1] = 0;
    print((const char*)str, channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(schar v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(uchar v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(short v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(ushort v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(int v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(uint v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(long v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(ulong v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(longlong v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(ulonglong v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(float v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(double v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(longdouble v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}
PHANTOM_EXPORT_PHANTOM void print(void* v, int channel /*= 0*/, int options /*= 0*/)
{
    print(lexical_cast<String>(v), channel, options);
}

} // namespace phantom
