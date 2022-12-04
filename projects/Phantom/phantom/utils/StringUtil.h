// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "StringViews.h"

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM StringUtil
{
    static void ReplaceAll(String& source, StringView from, StringView to);
    static void Split(Strings& a_Words, StringView a_Str, const char* a_Separators, bool a_bRemoveEmptyTokens = true);
    static void Split(StringViews& a_Words, StringView a_Str, const char* a_Separators,
                      bool a_bRemoveEmptyTokens = true);
    static StringView RemoveExtraBlanks(StringView _str);
    static bool       IsBlank(char a_c);

    /// @brief	convert a float to a string with an optional format \code ToString(0.56123f, ".2") => "0.56" \endcode
    static String ToString(float _val, const char* _fmt = nullptr);
    /// @brief	convert a double to a string with an optional format \code ToString(0.56123, ".2") => "0.56" \endcode
    static String ToString(double _val, const char* _fmt = nullptr);

    static String ToString(char _val, const char* _fmt = nullptr);
    static String ToString(sint8 _val, const char* _fmt = nullptr);
    static String ToString(uint8 _val, const char* _fmt = nullptr);
    static String ToString(sint16 _val, const char* _fmt = nullptr);
    static String ToString(uint16 _val, const char* _fmt = nullptr);
    static String ToString(sint32 _val, const char* _fmt = nullptr);
    static String ToString(uint32 _val, const char* _fmt = nullptr);
#if PHANTOM_INT64_IS_LONGLONG
    static_assert(sizeof(int64) == sizeof(longlong), "sizeof(int64) == sizeof(longlong)");
    static String ToString(long _val, const char* _fmt = nullptr);
    static String ToString(ulong _val, const char* _fmt = nullptr);
    static String ToString(sint64 _val, const char* _fmt = nullptr);
    static String ToString(uint64 _val, const char* _fmt = nullptr);
#else
#    if PHANTOM_INT64_IS_LONG == 0
#        error inconsistent int64/long/longlong macro definitions
#    endif
    static_assert(sizeof(int64) == sizeof(long), "sizeof(int64) == sizeof(long)");
    static String ToString(longlong _val, const char* _fmt = nullptr);
    static String ToString(ulonglong _val, const char* _fmt = nullptr);
#endif
    static String ToString(long double _val, const char* _fmt = nullptr);
};
} // namespace phantom
