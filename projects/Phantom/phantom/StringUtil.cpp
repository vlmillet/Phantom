// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "SmallVector.h"
#include "String.h"
#include "StringUtil.h"

namespace phantom
{
void StringUtil::ReplaceAll(String& source, StringView from, StringView to)
{
    String newString;
    newString.reserve(source.size()); // avoids a few memory allocations

    String::size_type lastPos = 0;
    String::size_type findPos;

    while (String::npos != (findPos = source.find(from, lastPos)))
    {
        PHANTOM_ASSERT(findPos >= lastPos);
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.size();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);

    source.swap(newString);
}

void StringUtil::Split(Strings& a_Words, StringView a_Str, const char* a_Separators,
                       bool a_bRemoveEmptyTokens /*= true*/)
{
    String buff{""};
    for (auto ch : a_Str)
    {
        const char* seps = a_Separators;
        char        sep;
        // is 'ch' a separator in 'a_Range' ?
        while ((sep = *seps++) && sep != ch)
            ;
        if (sep) // found a sep
        {
            if (!a_bRemoveEmptyTokens || !buff.empty())
            {
                a_Words.push_back((String &&) buff);
            }
            buff.clear();
        }
        else
        {
            buff += ch;
        }
    }
    if (!buff.empty())
        a_Words.push_back((String &&) buff);
}

void StringUtil::Split(StringViews& a_Words, StringView a_Str, const char* a_Separators,
                       bool a_bRemoveEmptyTokens /*= true*/)
{
    const char* start = a_Str.begin();
    size_t      count = 0;
    for (const char& ch : a_Str)
    {
        const char* seps = a_Separators;
        char        sep;
        // is 'ch' a separator in 'a_Range' ?
        while ((sep = *seps++) && sep != ch)
            ;
        if (sep) // found a sep
        {
            if (!a_bRemoveEmptyTokens || count)
            {
                a_Words.emplace_back(start, count);
            }
            start = &ch + 1;
            count = 0;
        }
        else
        {
            count++;
        }
    }
    if (count)
        a_Words.emplace_back(start, count);
}

} // namespace phantom
