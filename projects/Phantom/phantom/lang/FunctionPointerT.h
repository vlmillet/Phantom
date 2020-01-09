// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/FunctionPointer.h>
#include <phantom/lang/native/native.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace lang
{
/// STDCALL

#if defined(stdcall)
#    define _PHNTM_STDCALL_WAS_DEFINED
#    pragma push_macro("stdcall")
#    undef stdcall
#endif

#define _PHNTM_CONV_ID StdCall
#define _PHNTM_CONV_KW PHANTOM_CALLCONV(stdcall)
#include "FunctionPointerCustomConventionT.h"
#undef _PHNTM_CONV_ID
#undef _PHNTM_CONV_KW

#if defined(_PHNTM_STDCALL_WAS_DEFINED)
#    undef _PHNTM_STDCALL_WAS_DEFINED
#    pragma pop_macro("stdcall")
#endif

/// CDECL

#if defined(cdecl)
#    define _PHNTM_CDECL_WAS_DEFINED
#    pragma push_macro("cdecl")
#    undef cdecl
#endif

#define _PHNTM_CONV_ID CDecl
#define _PHNTM_CONV_KW PHANTOM_CALLCONV(cdecl)
#include "FunctionPointerCustomConventionT.h"
#undef _PHNTM_CONV_ID
#undef _PHNTM_CONV_KW

#if defined(_PHNTM_CDECL_WAS_DEFINED)
#    undef _PHNTM_CDECL_WAS_DEFINED
#    pragma pop_macro("cdecl")
#endif

/// FASTCALL

#if defined(fastcall)
#    define _PHNTM_FASTCALL_WAS_DEFINED
#    pragma push_macro("fastcall")
#    undef fastcall
#endif

#define _PHNTM_CONV_ID FastCall
#define _PHNTM_CONV_KW PHANTOM_CALLCONV(fastcall)
#include "FunctionPointerCustomConventionT.h"
#undef _PHNTM_CONV_ID
#undef _PHNTM_CONV_KW

#if defined(_PHNTM_FASTCALL_WAS_DEFINED)
#    undef _PHNTM_FASTCALL_WAS_DEFINED
#    pragma pop_macro("fastcall")
#endif

} // namespace lang
} // namespace phantom

/// @endcond
