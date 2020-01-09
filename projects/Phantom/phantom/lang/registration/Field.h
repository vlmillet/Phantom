// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "registration.h"

#include <phantom/lang/FieldT.h>

namespace phantom
{
namespace lang
{
}
} // namespace phantom

#define PHANTOM_FIELD(...) PHANTOM_PP_VARARG(_PHNTM_FIELD_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_FIELD(...)                                                                       \
    PHANTOM_PP_VARARG(_PHNTM_T_FIELD_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA

#define _PHNTM_FIELD_1(name) (this_()).field(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_FIELD_2(name, modifiers)                                                            \
    (this_()).field<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);

#define _PHNTM_T_FIELD_1(name)                                                                     \
    (this_()).PHANTOM_T field(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_T_FIELD_2(name, modifiers)                                                          \
    (this_()).PHANTOM_T field<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
