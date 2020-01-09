// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Function.h"

#include "Parameter.h"
#include "Signature.h"

#include <phantom/detail/new.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
Function::Function(StringView a_strName, ABI a_eABI, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Subroutine(a_strName, a_eABI, a_Modifiers, a_uiFlags)
{
}

Function::Function(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers /*= 0*/,
                   uint a_uiFlags /*= 0*/)
    : Subroutine(a_strName, a_pSignature, a_eABI, a_Modifiers, a_uiFlags)
{
}

Function::Function(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Subroutine(a_strName, a_pSignature, ABI::DefaultCall, a_Modifiers, a_uiFlags)
{
}

Function::Function(LanguageElement* a_pScope, StringView a_strName, StringView a_strSignature, ABI a_eABI,
                   Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Subroutine(a_pScope, a_strName, a_strSignature, a_eABI, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
