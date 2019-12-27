// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "MemoryContext.h"

namespace phantom
{
MemoryContext::~MemoryContext()
{
    for (auto pPtr : m_Allocations)
    {
        phantom::memory::deallocBytes(pPtr);
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
