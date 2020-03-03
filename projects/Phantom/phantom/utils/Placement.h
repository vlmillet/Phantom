// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/alignof>
#include <phantom/detail/core.h>
#include <phantom/traits/IntTypeBySize.h>
#include <phantom/traits/prerequisites.h>
/* *********************************************** */
namespace phantom
{
template<class T>
class Placement
{
public:
    using SelfType = Placement<T>;

    Placement() = default;
    Placement(const SelfType& a_Other) = delete;
    Placement(SelfType&& a_Temp) = delete;
    Placement& operator=(const SelfType& a_Other) = delete;
    Placement& operator=(SelfType&& a_Temp) = delete;
    ~Placement()
    {
        if (m_constructed)
            destroy();
    }
    template<class... Args>
    void construct(Args... args)
    {
        PHANTOM_ASSERT(!m_constructed);
        new (m_data) T(std::forward<Args>(args)...);
        m_constructed = (T*)&m_data;
    }
    void assign(T const& a_Val)
    {
        PHANTOM_ASSERT(m_constructed);
        *m_constructed = a_Val;
    }
    void assign(T&& a_Val)
    {
        PHANTOM_ASSERT(m_constructed);
        *m_constructed = std::move(a_Val);
    }
    void destroy()
    {
        PHANTOM_ASSERT(m_constructed);
        ((T*)m_data)->~T();
        m_constructed = nullptr;
    }

    T& operator*()
    {
        PHANTOM_ASSERT(m_constructed);
        return *m_constructed;
    }

    T const& operator*() const
    {
        PHANTOM_ASSERT(m_constructed);
        return *m_constructed;
    }

    T* operator->()
    {
        PHANTOM_ASSERT(m_constructed);
        return m_constructed;
    }

    T const* operator->() const
    {
        PHANTOM_ASSERT(m_constructed);
        return m_constructed;
    }

    operator T const*() const
    {
        return m_constructed;
    }

    operator T*()
    {
        return m_constructed;
    }

    operator bool() const
    {
        return m_constructed != nullptr;
    }

private:
    typename IntTypeBySize<PHANTOM_ALIGNOF(T)>::type m_data[sizeof(T) / PHANTOM_ALIGNOF(T)];
    T*                                               m_constructed = nullptr;
};

template<class T>
class RawPlacement
{
public:
    template<class... Args>
    void construct(Args... args)
    {
        PHANTOM_ASSERT(!m_constructed);
        new (m_data) T(args...);
        m_constructed = (T*)&m_data;
    }
    void destroy()
    {
        PHANTOM_ASSERT(m_constructed);
#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
        ((T*)m_data)->~T();
#if defined(__clang__)
#    pragma clang diagnostic pop
#endif
        m_constructed = nullptr;
    }

    T& operator*()
    {
        PHANTOM_ASSERT(m_constructed);
        return *m_constructed;
    }

    T const& operator*() const
    {
        PHANTOM_ASSERT(m_constructed);
        return *m_constructed;
    }

    T* operator->()
    {
        PHANTOM_ASSERT(m_constructed);
        return m_constructed;
    }

    T const* operator->() const
    {
        PHANTOM_ASSERT(m_constructed);
        return m_constructed;
    }

    operator T const*() const
    {
        return m_constructed;
    }

    operator T*()
    {
        return m_constructed;
    }

    operator bool() const
    {
        return m_constructed != nullptr;
    }

    typename IntTypeBySize<PHANTOM_ALIGNOF(T)>::type m_data[sizeof(T) / PHANTOM_ALIGNOF(T)];
    T*                                               m_constructed;
};
} // namespace phantom
