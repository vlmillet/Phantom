// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/CallHelpers.h>
#include <phantom/lang/Method.h>
#include <phantom/lang/NativeVTableInspector.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace lang
{
class MethodBaseT : public Method
{
public:
    MethodBaseT(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Method(a_strName, a_pSignature, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
};

} // namespace lang
} // namespace phantom

/// @endcond
