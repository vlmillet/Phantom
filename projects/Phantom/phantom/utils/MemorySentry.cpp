// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "MemorySentry.h"

#include <stdio.h>

#if PHANTOM_CONSISTENCY_CHECK_ENABLED
#    include "CallStackTrace.h"

#    include <map>
#    include <phantom/thread/SpinMutex.h>
#endif

namespace phantom
{
#if PHANTOM_CONSISTENCY_CHECK_ENABLED
SpinMutex                         MEMORY_SENTRY_MUTEX;
std::map<void const*, void* [12]> MEMORY_SENTRY_TRACE;
#endif
#if !defined(PHANTOM_NO_MEMORY_SENTRY_CHECKS)
// Called when data is invalid
void MemorySentry::Error() const
{
#    if PHANTOM_CONSISTENCY_CHECK_ENABLED
    auto lock = MEMORY_SENTRY_MUTEX.autoLock();
    char buffer[1024];
    CallStackTrace::ToString(MEMORY_SENTRY_TRACE[this], 12, buffer, 1024);
    printf("%s\n", buffer);
#    endif
    // This is not inline in order to not pollute callee code with assert code for what's a rare event.
    printf("MemorySentry: Instance @ %p invalid data. Value = %08X\n", this, m_state);
    if (m_state == MemoryState::Destructed || m_state == MemoryState::_MSVC_Destructed)
    {
        PHANTOM_ASSERT(m_state == MemoryState::Valid && "Invalid data! Instance has been deleted.");
    }
    else
    {
        PHANTOM_ASSERT(m_state == MemoryState::Valid && "Invalid data!");
    }
}

// Called when destructed and user called magicRequestBreakOnDestruction() earlier
void MemorySentry::Break() const
{
    printf("MemorySentry: Instance @ %p requested breakpoint. Value = %08X\n", this, m_state);
    PHANTOM_DEBUG_BREAK();
}

void MemorySentry::Trace() const
{
#    if PHANTOM_MEMORY_SENTRY_TRACE_ENABLED
    auto lock = MEMORY_SENTRY_MUTEX.autoLock();
    CallStackTrace::Capture(MEMORY_SENTRY_TRACE[this], 12);
#    endif
}

#endif
} // namespace phantom
