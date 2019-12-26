// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/SmallSet.h>
#include <phantom/memory/memory.h>
/* *********************************************** */
namespace phantom
{
class PHANTOM_EXPORT_PHANTOM MemoryContext
{
public:
    ~MemoryContext();
    void registerMemory(void* a_pPtr);
    void unregisterMemory(void* a_pPtr);

private:
    SmallSet<void*, 256, 256> m_Allocations;
};
} // namespace phantom
