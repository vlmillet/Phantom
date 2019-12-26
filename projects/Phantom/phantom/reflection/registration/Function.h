// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "Namespace.h"
#include "phantom/reflection/FunctionProviderT.h"
#include "phantom/reflection/SignatureH.h"
#include "phantom/traits/FunctionTypeToFunctionPointerType.h"

#define PHANTOM_FUNCTION(...)                                                                                          \
    PHANTOM_REGISTER(Functions)                                                                                        \
    {                                                                                                                  \
        if (PHANTOM_REGISTRATION_STEP == phantom::RegistrationStep::Functions)                                         \
            PHANTOM_PP_VARARG(_PHNTM_FUNCTION_, ##__VA_ARGS__);                                                        \
    }                                                                                                                  \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1

#define _PHNTM_FUNCTION_1(Name) (this_()).function(PHANTOM_PP_QUOTE(Name), Name)

#define _PHNTM_FUNCTION_2(Name, Defaults) (this_()).function(PHANTOM_PP_QUOTE(Name), Name) _PHTNM_FMT_DEFAULTS Defaults

#define _PHNTM_FUNCTION_3(ReturnType, Name, ParamList)                                                                 \
    (this_()).function<PHANTOM_PP_REMOVE_PARENS(ReturnType) ParamList>(PHANTOM_PP_QUOTE(Name), Name)

#define _PHNTM_FUNCTION_4(ReturnType, Name, ParamList, Defaults)                                                       \
    (this_()).function<PHANTOM_PP_REMOVE_PARENS(ReturnType) ParamList>(PHANTOM_PP_QUOTE(Name), Name)                   \
    _PHTNM_FMT_DEFAULTS Defaults

#define PHANTOM_STATIC_METHOD(...) PHANTOM_PP_VARARG(_PHNTM_STATIC_METHOD_, ##__VA_ARGS__)

#define _PHNTM_STATIC_METHOD_1(Name) (this_()).staticMethod(PHANTOM_PP_QUOTE(Name), &_::Name)

#define _PHNTM_STATIC_METHOD_2(Name, Defaults)                                                                         \
    (this_()).staticMethod(PHANTOM_PP_QUOTE(Name), &_::Name) PHANTOM_PP_IDENTITY(_PHTNM_FMT_DEFAULTS) Defaults

#define _PHNTM_STATIC_METHOD_3(ReturnType, Name, ParamList)                                                            \
    (this_()).staticMethod<PHANTOM_PP_REMOVE_PARENS(ReturnType) ParamList>(PHANTOM_PP_QUOTE(Name), &_::Name)

#define _PHNTM_STATIC_METHOD_4(ReturnType, Name, ParamList, Defaults)                                                  \
    (this_()).staticMethod<PHANTOM_PP_REMOVE_PARENS(ReturnType) ParamList>(PHANTOM_PP_QUOTE(Name), &_::Name)           \
    PHANTOM_PP_IDENTITY(_PHTNM_FMT_DEFAULTS) Defaults
