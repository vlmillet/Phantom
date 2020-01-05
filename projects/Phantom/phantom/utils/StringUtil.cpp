// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "StringUtil.h"

#include "SmallVector.h"
#include "String.h"

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

StringView StringUtil::RemoveExtraBlanks(StringView _str)
{
    while (_str.size() && IsBlank(_str.back()))
        _str.dropBack();
    while (_str.size() && IsBlank(_str.front()))
        _str.dropFront();
    return _str;
}

bool StringUtil::IsBlank(char a_c)
{
	return a_c == ' ' || a_c == '\t' || a_c == '\r' || a_c == '\n';
}

String StringUtil::ToString(uint64 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIu64;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIu64, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(sint64 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIi64;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi64, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(uint32 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIu32;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIu32, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(sint32 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIi32;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi32, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(uint16 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIu16;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIu16, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(sint16 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIi16;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi16, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(uint8 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIu8;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIu8, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(sint8 _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIi8;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi8, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(char _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%c" ;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi8, _fmt);
	char res[4];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(double _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%f";
	if (_fmt)
		snprintf(fmt, 32, "%%%sf", _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(long double _val, const char* _fmt /*= nullptr*/)
{
	return ToString((double)_val, _fmt);
}

String StringUtil::ToString(float _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%f";
	if (_fmt)
		snprintf(fmt, 32, "%%%sf", _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

#if PHANTOM_INT64_IS_LONGLONG
String StringUtil::ToString(long _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%l";
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIu64, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(ulong _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%lu" ;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi64, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}
#else
String StringUtil::ToString(longlong _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIi64;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIu64, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}

String StringUtil::ToString(ulonglong _val, const char* _fmt /*= nullptr*/)
{
	char fmt[32] = "%" PRIu64;
	if (_fmt)
		snprintf(fmt, 32, "%%%s" PRIi64, _fmt);
	char res[64];
	snprintf(res, 64, fmt, _val);
	return res;
}
#endif
} // namespace phantom
