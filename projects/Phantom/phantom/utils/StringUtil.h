// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
};
} // namespace phantom
