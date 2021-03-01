// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "MemoryContext.h"

namespace phantom
{
MemoryContext::~MemoryContext()
{
    for (auto pPtr : m_Allocations)
    {
        phantom::deallocate(pPtr);
    }
}

void MemoryContext::registerMemory(void* a_pPtr)
{
    PHANTOM_VERIFY(m_Allocations.insert(a_pPtr).second);
}

void MemoryContext::unregisterMemory(void* a_pPtr)
{
    PHANTOM_VERIFY(m_Allocations.erase(a_pPtr) == 1);
}

} // namespace phantom
