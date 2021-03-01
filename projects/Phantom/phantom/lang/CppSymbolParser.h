// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
struct PHANTOM_EXPORT_PHANTOM CppSymbolParser
{
    Symbol* parse(StringView a_Text, LanguageElement* a_pScope,
                  StringBuffer* a_pLastError = nullptr);
    bool    parse(StringView a_Text, Symbols& a_Symbols, LanguageElement* a_pScope,
                  StringBuffer* a_pLastError = nullptr);

private:
    StringBuffer m_LastError;
};
} // namespace lang
} // namespace phantom
