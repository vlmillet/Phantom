// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Import.h"

#include "Source.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Import::Import(Source* a_pSource, StringView a_strSymbolName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strSymbolName, a_Modifiers, a_uiFlags), m_pImportedSource(a_pSource)
{
}

} // namespace lang
} // namespace phantom
