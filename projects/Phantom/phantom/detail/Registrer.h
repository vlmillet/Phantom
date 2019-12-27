// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "phantom.h"

#include <phantom/utils/Delegate.h>

namespace phantom
{
using Registrer = phantom::Delegate<void(RegistrationStep)>;
}
