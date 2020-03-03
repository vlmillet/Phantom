// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/lang/SignalT.h>
#include <phantom/lang/SignatureH.h>
#include <phantom/utils/Signal.h>

#define PHANTOM_T_SIGNAL(name) PHANTOM_T PHANTOM_SIGNAL(name)
#define PHANTOM_SIGNAL(name) .signal(PHANTOM_PP_QUOTE(name), &_::name)
