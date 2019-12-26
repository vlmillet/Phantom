// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "RecursiveSemaphore.h"
#include "Semaphore.h"

namespace phantom
{
typedef RecursiveSemaphore<SpinSemaphore> RecursiveSpinMutex;
}
