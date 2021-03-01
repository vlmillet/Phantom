// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "InstanceCache.h"

namespace phantom
{
namespace lang
{
InstanceCache::InstanceCache(Class* a_pClass) : m_pClass(a_pClass)
{
    PHANTOM_CONNECT(m_pClass, kindCreated, this, kindCreated);
    PHANTOM_CONNECT(m_pClass, kindDestroying, this, kindDestroying);
    PHANTOM_ASSERT(m_pClass->getInstanceCount() == 0,
                   "instance cache cannot be created after an instance has already been created");
}

InstanceCache::~InstanceCache()
{
    PHANTOM_DISCONNECT(m_pClass, kindCreated, this, kindCreated);
    PHANTOM_DISCONNECT(m_pClass, kindDestroying, this, kindDestroying);
}

void InstanceCache::kindCreated(void* a_pInstance)
{
    auto lock = m_Mutex.autoLock();
    PHANTOM_ASSERT(std::find(m_Instances.begin(), m_Instances.end(), a_pInstance) == m_Instances.end());
    m_Instances.push_back(a_pInstance);
}

void InstanceCache::kindDestroying(void* a_pInstance)
{
    auto lock = m_Mutex.autoLock();
    auto found = std::find(m_Instances.begin(), m_Instances.end(), a_pInstance);
    PHANTOM_ASSERT(found != m_Instances.end());
    m_Instances.erase(found);
}
} // namespace lang
} // namespace phantom
