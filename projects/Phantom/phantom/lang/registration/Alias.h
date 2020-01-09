// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "Namespace.h"

#include <phantom/lang/Alias.h>

#define PHANTOM_ALIAS(name, ...) PHANTOM_TYPEDEF(name, ##__VA_ARGS__)
#define PHANTOM_MALIAS(name) PHANTOM_MTYPEDEF(name)
#define PHANTOM_T_MALIAS(name) PHANTOM_T_MTYPEDEF(name)

#define PHANTOM_TYPEDEF(name)                                                                      \
    PHANTOM_REGISTER(Typedefs, End)                                                                \
    {                                                                                              \
        if (PHANTOM_REGISTRATION_STEP == phantom::RegistrationStep::Typedefs)                      \
            (this_()).typedef_<name>(PHANTOM_PP_QUOTE(name));                                      \
    }                                                                                              \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1
#define PHANTOM_MTYPEDEF(name)                                                                     \
    (this_()).typedef_<PHANTOM_SCOPE::name>(PHANTOM_PP_QUOTE(name));                               \
    _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_MTYPEDEF(name)                                                                   \
    (this_()).PHANTOM_T typedef_<PHANTOM_TYPENAME PHANTOM_SCOPE::name>(PHANTOM_PP_QUOTE(name));    \
    _PHNTM_MTRAILING_METADATA

#define PHANTOM_USING(name) PHANTOM_TYPEDEF(name)
#define PHANTOM_MUSING(name)                                                                       \
    (this_()).using_(PHANTOM_PP_QUOTE(name));                                                      \
    _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_MUSING(name)                                                                     \
    (this_()).using_(PHANTOM_PP_QUOTE(name));                                                      \
    _PHNTM_MTRAILING_METADATA
