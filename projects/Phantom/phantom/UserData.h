// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/phantom.h>
#include <phantom/utils/Delegate.h>

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM UserData
{
    UserData() = default;
    UserData(void* a_pData, Delegate<void(void*)> a_Destroy) : m_pData(a_pData), m_Destroy(a_Destroy)
    {
        PHANTOM_ASSERT(a_pData && m_Destroy);
    }
    UserData(UserData const&) = delete;
    UserData& operator=(UserData const&) = delete;
    UserData(UserData&& a_Temp) : m_pData(a_Temp.m_pData), m_Destroy(a_Temp.m_Destroy)
    {
        a_Temp.m_pData = nullptr;
    }
    UserData& operator=(UserData&& a_Temp)
    {
        m_pData = a_Temp.m_pData;
        m_Destroy = a_Temp.m_Destroy;
        a_Temp.m_pData = nullptr;
        return *this;
    }
    ~UserData()
    {
        if (m_pData)
            m_Destroy(m_pData);
    }

    void* getData() const
    {
        return m_pData;
    }

private:
    void*                 m_pData = nullptr;
    Delegate<void(void*)> m_Destroy;
};
} // namespace phantom
