// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "core.h"

#include <phantom/utils/Delegate.h>

namespace phantom
{
using Registrer = phantom::Delegate<void(RegistrationStep)>;
}
