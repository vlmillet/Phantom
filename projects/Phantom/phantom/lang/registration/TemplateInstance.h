// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "GlobalRegistrer.h"
#include "registration.h"

#include <phantom/lang/TypeOf.h>

#define PHANTOM_TEMPLATE_INSTANCE(...)                                                             \
    namespace                                                                                      \
    {                                                                                              \
    static_assert(                                                                                 \
    !std::is_same<phantom::lang::Type*,                                                      \
                  decltype(phantom::TypeOf(phantom::TypeOfTag<__VA_ARGS__>()))>::value,            \
    "no template lang found for '" PHANTOM_PP_QUOTE(                                         \
    __VA_ARGS__) "' ; if using haunt, the associated '.hxx' #include is probably missing");        \
    PHANTOM_REGISTER(End)                                                                          \
    {                                                                                              \
        phantom::TypeOf(phantom::TypeOfTag<__VA_ARGS__>());                                        \
    }                                                                                              \
    }
