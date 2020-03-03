// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "MemorySentry.h"

#include <stdio.h>

namespace phantom
{
#if !defined(PHANTOM_NO_MEMORY_SENTRY_CHECKS)
// Called when data is invalid
void MemorySentry::Error() const
{
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
#endif
} // namespace phantom
