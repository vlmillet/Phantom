// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

#define PHANTOM_FIELD_PROPERTY_DEF(type, Name, field)                                              \
    PHANTOM_PROPERTY_DEF(type, Name, (field = value;), (return field;))

#define PHANTOM_PROPERTY_DEF(type, Name, set_code, get_code)                                       \
    void set##Name(type value){PHANTOM_PP_IDENTITY set_code} type get##Name()                      \
    const {PHANTOM_PP_IDENTITY get_code} HAUNT_ON                 HAUNT_PROPERTY(type, Name)
