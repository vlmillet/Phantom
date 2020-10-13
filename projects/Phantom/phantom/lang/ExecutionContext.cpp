// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "ExecutionContext.h"

#include "Class.h"

namespace phantom
{
namespace lang
{
PHANTOM_EXPORT_PHANTOM ExecutionContext* (*GetDefaultExecutionContext)();

namespace
{
thread_local SmallVector<lang::ExecutionContext*> g_ExecutionContexts;
} // namespace

ExecutionContext::~ExecutionContext()
{
    releaseTemporaries();
}

void ExecutionContext::Push(ExecutionContext* a_pExecutionContext)
{
    PHANTOM_ASSERT(a_pExecutionContext);
    g_ExecutionContexts.push_back(a_pExecutionContext);
}

void ExecutionContext::Pop()
{
    PHANTOM_ASSERT(g_ExecutionContexts.size() > 0);
    g_ExecutionContexts.pop_back();
}

ExecutionContext* ExecutionContext::Current()
{
    PHANTOM_ASSERT(g_ExecutionContexts.size() || GetDefaultExecutionContext,
                   "no execution context, use ExecutionContext::Push() / ExecutionContext::Pop()");
    if (g_ExecutionContexts.empty())
    {
        return GetDefaultExecutionContext();
    }
    return g_ExecutionContexts.back();
}

void ExecutionContext::pushTempDestruction(Evaluable* a_pScope, Type* a_pType, void* a_pBuffer)
{
    m_Temporaries[a_pScope].push_back({std::make_pair(a_pType, a_pBuffer)});
}

void ExecutionContext::releaseTemporaries(Evaluable* a_pScope)
{
    auto found = m_Temporaries.find(a_pScope);
    if (found != m_Temporaries.end())
    {
        for (auto& pair : found->second)
        {
            pair.first->destroy(pair.second);
            pair.first->deallocate(pair.second);
        }
        m_Temporaries.erase(found);
    }
}

void ExecutionContext::releaseTemporaries()
{
    for (auto& temp : m_Temporaries)
    {
        auto& vec = temp.second;
        {
            for (auto it = vec.begin(); it != vec.end(); ++it)
            {
                it->first->destroy(it->second);
                it->first->deallocate(it->second);
            }
        }
    }
    m_Temporaries.clear();
}

} // namespace lang
} // namespace phantom
