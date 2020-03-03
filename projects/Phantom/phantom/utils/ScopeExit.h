// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

/* ****************** Includes ******************* */
#include <phantom/plugin.h>
/* *********************************************** */

namespace phantom
{
template<class t_Functor>
struct ScopeExit
{
    using SelfType = ScopeExit<t_Functor>;
    ScopeExit(t_Functor a_F) : m_F(a_F)
    {
    }
    ~ScopeExit()
    {
        if (!m_bCancelled)
            m_F();
    }
    ScopeExit(SelfType const&) = delete;
    ScopeExit(SelfType&& a_Temp) : m_F(std::move(a_Temp.m_F))
    {
        a_Temp.m_bCancelled = true;
    }
    SelfType& operator=(SelfType const&) = delete;
    SelfType& operator=(SelfType&&) = delete;

    void cancel()
    {
        m_bCancelled = true;
    }

private:
    t_Functor m_F;
    bool      m_bCancelled = false;
};

template<class t_Functor>
PHANTOM_FORCEINLINE ScopeExit<t_Functor> makeScopeExit(t_Functor&& a_F)
{
    return ScopeExit<t_Functor>(std::forward<t_Functor>(a_F));
}
} // namespace phantom
