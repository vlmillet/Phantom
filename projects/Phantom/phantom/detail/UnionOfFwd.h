// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/detail/TypeOfFwd.h>

#define PHANTOM_UNIONOF(...) _PHNTM_TYPEOF_CHECK(union, Union, __VA_ARGS__)
