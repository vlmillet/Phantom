// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Function.h"

#include "Parameter.h"
#include "Signature.h"

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

} // namespace lang
} // namespace phantom
