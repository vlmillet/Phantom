// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "LockGuard.h"
#include "RecursiveSemaphore.h"

namespace phantom
{
typedef RecursiveSemaphore<Semaphore> RecursiveMutex;
typedef LockGuard<RecursiveMutex>     RecursiveMutexGuard;
} // namespace phantom
