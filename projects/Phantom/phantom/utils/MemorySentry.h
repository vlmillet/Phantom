// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;
#include <phantom/plugin.h>

namespace phantom
{
// Helper to check for valid/invalid instances. Inherit or include as a member. 4 bytes.
// Memory pattern goes:
//    0xCDCDCDCD/0xFDFDFDFD (allocated*) --> 0xCAFEBABE (constructed) --> 0xDEADBEEF (destructed) --> 0xDDDDDDDD
//    (deleted*)			(* PC only)
// Usage:
// - Call Check() occasionally or when in doubt that a pointer is valid.
// - Call RequestBreakOnDestruction() when debugging to request a breakpoint when specified instance is being deleted.
struct PHANTOM_EXPORT_PHANTOM MemorySentry
{
private:
#if !defined(PHANTOM_NO_MEMORY_SENTRY_CHECKS)
    // Enum so that we see a value in the debugger regardless of using decimal or hexadecimal.
    enum class MemoryState : uint32_t
    {
        Valid = 0xCAFEBABE,
        ValidBreakOnDestruction = 0xCAFEBABB,
        Destructed = 0xDEADBEEF,
        _MSVC_Destructed = 0xDDDDDDDD
    };

    mutable MemoryState m_state; // 4 bytes

protected:
    MemorySentry() { m_state = MemoryState::Valid; }
    ~MemorySentry()
    {
        Check();
        PHANTOM_IF_UNLIKELY(m_state == MemoryState::ValidBreakOnDestruction) Break();
        m_state = MemoryState::Destructed;
    }

    inline void Check() const
    {
        PHANTOM_IF_UNLIKELY(m_state != MemoryState::Valid && m_state != MemoryState::ValidBreakOnDestruction) Error();
    }
    void RequestBreakOnDestruction() const
    {
        Check();
        m_state = MemoryState::ValidBreakOnDestruction;
    }
    void Error() const; // Called when data is invalid
    void Break() const; // Called when destructed and user called RequestBreakOnDestruction() earlier
#else
    inline void Check() const {}
    void        RequestBreakOnDestruction() const {}
    void        Error() const {} // Called when data is invalid
    void        Break() const {} // Called when destructed and user called RequestBreakOnDestruction() earlier
#endif
};
} // namespace phantom
