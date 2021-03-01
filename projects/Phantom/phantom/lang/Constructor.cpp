// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Constructor.h"

#include "Parameter.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Constructor::Constructor() : Method(ABI::MethodCall) {}

Constructor::Constructor(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : Method(a_strName, a_pSignature, ABI::MethodCall, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
