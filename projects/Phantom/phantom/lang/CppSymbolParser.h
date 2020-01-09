// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Symbol.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
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
} // namespace reflection
} // namespace phantom
