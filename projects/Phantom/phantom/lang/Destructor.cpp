// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Destructor.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Destructor::Destructor()
{
}

Destructor::Destructor(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/,
                       uint a_uiFlags /*= 0*/)
    : Method(a_strName, a_pSignature, a_Modifiers, a_uiFlags)
{
}

Destructor::Destructor(ClassType* a_pClassType, StringView a_strName, StringView a_strSignature,
                       Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Method(a_pClassType, a_strName, a_strSignature, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
