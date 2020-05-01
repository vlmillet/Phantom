// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Destructor.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Destructor::Destructor(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/,
                       uint a_uiFlags /*= 0*/)
    : Method(a_strName, a_pSignature, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
