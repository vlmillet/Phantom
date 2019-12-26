// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "InstanceCache.h"

namespace phantom
{
namespace reflection
{
InstanceCache::InstanceCache(Class* a_pClass) : m_pClass(a_pClass)
{
    PHANTOM_CONNECT(m_pClass, kindCreated, this, kindCreated);
    PHANTOM_CONNECT(m_pClass, kindDestroyed, this, kindDestroyed);
}

InstanceCache::~InstanceCache()
{
    PHANTOM_DISCONNECT(m_pClass, kindCreated, this, kindCreated);
    PHANTOM_DISCONNECT(m_pClass, kindDestroyed, this, kindDestroyed);
}

void InstanceCache::kindCreated(void* a_pInstance)
{
    PHANTOM_ASSERT(std::find(m_Instances.begin(), m_Instances.end(), a_pInstance) ==
                   m_Instances.end());
    m_Instances.push_back(a_pInstance);
}

void InstanceCache::kindDestroyed(void* a_pInstance)
{
    auto found = std::find(m_Instances.begin(), m_Instances.end(), a_pInstance);
    PHANTOM_ASSERT(found != m_Instances.end());
    m_Instances.erase(found);
}
} // namespace reflection
} // namespace phantom
