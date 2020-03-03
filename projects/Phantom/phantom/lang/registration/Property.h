// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/lang/PropertyT.h>

namespace phantom
{
namespace lang
{
}
} // namespace phantom

#define PHANTOM_PROPERTY(...)                                                                      \
    PHANTOM_PP_VARARG(_PHNTM_PROPERTY_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_PROPERTY(...)                                                                    \
    PHANTOM_PP_VARARG(_PHNTM_T_PROPERTY_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define _PHNTM_PROPERTY_FUNCS(...) PHANTOM_PP_VARARG(_PHNTM_PROPERTY_FUNCS_, ##__VA_ARGS__)

#define _PHNTM_PROPERTY_FUNCS_1(get) &PHANTOM_SCOPE::get
#define _PHNTM_PROPERTY_FUNCS_2(get, set) &PHANTOM_SCOPE::get, &PHANTOM_SCOPE::set
#define _PHNTM_PROPERTY_FUNCS_3(get, set, signal)                                                  \
    &PHANTOM_SCOPE::get, &PHANTOM_SCOPE::set, &PHANTOM_SCOPE::signal

#define _PHNTM_PROPERTY_1(name)                                                                    \
    (this_()).property(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name, &PHANTOM_SCOPE::name);
#define _PHNTM_PROPERTY_2(name, funcs)                                                             \
    (this_()).property(PHANTOM_PP_QUOTE(name), _PHNTM_PROPERTY_FUNCS funcs);
#define _PHNTM_PROPERTY_3(name, funcs, modifiers)                                                  \
    (this_()).property<modifiers>(PHANTOM_PP_QUOTE(name), _PHNTM_PROPERTY_FUNCS funcs);
#define _PHNTM_PROPERTY_4(name, funcs, modifiers, filters)                                         \
    (this_()).property<modifiers>(PHANTOM_PP_QUOTE(name), _PHNTM_PROPERTY_FUNCS funcs, filters);

#define _PHNTM_T_PROPERTY_1(name)                                                                  \
    (this_()).PHANTOM_T property(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name,                     \
                                 &PHANTOM_SCOPE::name);
#define _PHNTM_T_PROPERTY_2(name, funcs)                                                           \
    (this_()).PHANTOM_T property(PHANTOM_PP_QUOTE(name), _PHNTM_PROPERTY_FUNCS funcs);
#define _PHNTM_T_PROPERTY_3(name, funcs, modifiers)                                                \
    (this_()).PHANTOM_T property<modifiers>(PHANTOM_PP_QUOTE(name), _PHNTM_PROPERTY_FUNCS funcs);
#define _PHNTM_T_PROPERTY_4(name, funcs, modifiers, filters)                                       \
    (this_()).PHANTOM_T property<modifiers>(PHANTOM_PP_QUOTE(name), _PHNTM_PROPERTY_FUNCS funcs,   \
                                            filters);
