// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "ClassType.h"
#include "registration.h"

#define PHANTOM_FRIEND(name) .friend_(PHANTOM_PP_QUOTE(name))
#define PHANTOM_FRIEND_CLASS(name) .friend_<name>()
#define PHANTOM_T_FRIEND(name) PHANTOM_T PHANTOM_FRIEND(name)
