// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "newImpl.h"

#define PHANTOM_DECL_VIRTUAL_DELETE_METHOD(...)                                                    \
    virtual void PHANTOM_CUSTOM_VIRTUAL_DELETE()                                                   \
    {                                                                                              \
        ::phantom::detail::DeleteH<true, __VA_ARGS__>() * this;                                    \
    }
#define PHANTOM_DECL_OVERRIDE_DELETE_METHOD(...)                                                   \
    virtual void PHANTOM_CUSTOM_VIRTUAL_DELETE() override                                          \
    {                                                                                              \
        ::phantom::detail::DeleteH<true, __VA_ARGS__>() * this;                                    \
    }
#define PHANTOM_DECL_ABSTRACT_DELETE_METHOD(...) virtual void PHANTOM_CUSTOM_VIRTUAL_DELETE() = 0
#define PHANTOM_DELETE_VIRTUAL ::phantom::detail::VirtualDeleteH()*
