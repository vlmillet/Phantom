// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "Namespace.h"
#include "registration.h"

#include <phantom/lang/VariableT.h>

#define PHANTOM_VARIABLE(Name)                                                                     \
    PHANTOM_REGISTER(Variables, End)                                                               \
    {                                                                                              \
        if (PHANTOM_REGISTRATION_STEP == phantom::RegistrationStep::Typedefs)                      \
            (this_()).variable(PHANTOM_PP_QUOTE(Name), Name);                                      \
    }                                                                                              \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1
#define PHANTOM_STATIC_FIELD(Name)                                                                 \
    (this_()).staticField(PHANTOM_PP_QUOTE(Name), Name);                                           \
    _PHNTM_MTRAILING_METADATA
