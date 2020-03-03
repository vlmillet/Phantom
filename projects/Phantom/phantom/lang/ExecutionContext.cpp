// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "ExecutionContext.h"
#include "Type.h"

namespace phantom
{
namespace lang
{
static thread_local SmallVector<lang::ExecutionContext*> g_ExecutionContexts;

ExecutionContext::~ExecutionContext()
{
    releaseTemporaries();
}

void ExecutionContext::Push(ExecutionContext* a_pExecutionContext)
{
    g_ExecutionContexts.push_back(a_pExecutionContext);
}

void ExecutionContext::Pop()
{
    PHANTOM_ASSERT(g_ExecutionContexts.size() > 0);
    g_ExecutionContexts.pop_back();
}

ExecutionContext* ExecutionContext::Current()
{
    PHANTOM_ASSERT(g_ExecutionContexts.size(),
                   "no execution context, use ExecutionContext::Push() / ExecutionContext::Pop()");
    return g_ExecutionContexts.back();
}

void ExecutionContext::pushTempDestruction(Evaluable* a_pScope, Type* a_pType, void* a_pBuffer)
{
    m_Temporaries.push_back(TempDestruction{
    a_pScope, SmallVector<std::pair<Type*, void*> >{std::make_pair(a_pType, a_pBuffer)}});
}

void ExecutionContext::releaseTemporaries(Evaluable* a_pScope)
{
    for (auto it = m_Temporaries.begin(); it != m_Temporaries.end();)
    {
        if (it->scope == a_pScope)
        {
            for (auto& pair : it->buffers)
            {
                pair.first->destroy(pair.second);
                pair.first->deallocate(pair.second);
            }
            it = m_Temporaries.erase(it);
        }
        else
            ++it;
    }
}

void ExecutionContext::releaseTemporaries()
{
    for (auto& temp : m_Temporaries)
    {
        auto& vec = temp.buffers;
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
