// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Constructor.h"

#include "Parameter.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Constructor::Constructor() : Method(ABI::MethodCall)
{
}

Constructor::Constructor(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : Method(a_strName, a_pSignature, ABI::MethodCall, a_Modifiers, a_uiFlags)
{
}

Constructor::Constructor(ClassType* a_pClassType, StringView a_strName, StringView a_strSignature,
                         Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Method(a_pClassType, a_strName, a_strSignature, ABI::MethodCall, a_Modifiers, a_uiFlags)
{
}

} // namespace reflection
} // namespace phantom
