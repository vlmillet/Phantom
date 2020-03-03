// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <haunt>
HAUNT_STOP;
#include <phantom/detail/memory.h>
#include <phantom/utils/SmallSet.h>
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
