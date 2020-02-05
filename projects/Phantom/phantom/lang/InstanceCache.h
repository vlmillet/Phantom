// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/TypeOf.h>
#include <phantom/register>
#include <phantom/thread/SpinMutex.h>
#include <phantom/thread/ThreadSafeArrayView.h>
#include <phantom/utils/ArrayView.h>
/* **************** Declarations ***************** */

/* *********************************************** */

#define PHANTOM_INSTANCE_CACHE(T, ...)                                                                                 \
    PHANTOM_REGISTER(PostClassTypes) { PHANTOM_CLASSOF(T)->getOrCreateInstanceCache(); }

namespace phantom
{
namespace lang
{
/// \brief  Represents an object oriented class.
class PHANTOM_EXPORT_PHANTOM InstanceCache
{
    friend class Class;

protected:
    InstanceCache(Class* a_pClass);
    InstanceCache(const InstanceCache& a_Cache) = delete;
    InstanceCache(InstanceCache&& a_Cache) = delete;
    InstanceCache& operator=(const InstanceCache& a_Cache) = delete;
    InstanceCache& operator=(InstanceCache&& a_Cache) = delete;

public:
    ~InstanceCache();

    ThreadSafeArrayView<void*, SpinMutex> getInstances() const
    {
        return MakeThreadSafeArrayView(m_Instances.data(), m_Instances.size(), m_Mutex);
    }

    template<class T>
    ThreadSafeArrayView<T*, SpinMutex> getInstancesReinterpretedAs() const
    {
        return MakeThreadSafeArrayView((T**)m_Instances.data(), m_Instances.size(), m_Mutex);
    }

private:
    void kindCreated(void* a_pInstance);
    void kindDestroying(void* a_pInstance);

private:
    VoidPtrs          m_Instances;
    Class*            m_pClass;
    mutable SpinMutex m_Mutex;
};

} // namespace lang
} // namespace phantom
