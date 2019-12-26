// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/Placement.h>

namespace phantom
{
enum _NullOpt
{
    NullOpt
};

template<class T>
class Optional
{
public:
    using SelfType = Optional<T>;

    Optional() = default;
    Optional(T const& a_Val)
    {
        m_data.construct(a_Val);
    }
    Optional(T&& a_Val)
    {
        m_data.construct(std::move(a_Val));
    }
    Optional(const SelfType& a_Other);
    Optional(SelfType&& a_Temp);
    Optional(_NullOpt) : Optional()
    {
    }
    SelfType& operator=(const SelfType& a_Other);
    SelfType& operator=(SelfType&& a_Temp);
    ~Optional();

    SelfType& operator=(_NullOpt)
    {
        destroy();
        return *this;
    }

    SelfType& operator=(T&& a_Temp);

    SelfType& operator=(T const& a_Val);

    T* operator->()
    {
        PHANTOM_ASSERT(m_data);
        return m_data.operator->();
    }
    T& operator*()
    {
        PHANTOM_ASSERT(m_data);
        return m_data.operator*();
    }

    T const* operator->() const
    {
        PHANTOM_ASSERT(m_data);
        return m_data.operator->();
    }
    T const& operator*() const
    {
        PHANTOM_ASSERT(m_data);
        return m_data.operator*();
    }

    operator T const*() const
    {
        return isInitialized() ? (T const*)m_data : nullptr;
    }
    operator T*()
    {
        return isInitialized() ? (T*)m_data : nullptr;
    }

    void destroy();
    void reset()
    {
        destroy();
    }

    bool isInitialized() const
    {
        return m_data;
    }
    bool isNull() const
    {
        return !isInitialized();
    }

    operator bool() const
    {
        return isInitialized();
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        PHANTOM_ASSERT(!isInitialized());
        m_data.construct(std::forward<Args>(args)...);
    }

private:
    Placement<T> m_data;
};

template<class T>
Optional<T>::Optional(const SelfType& a_Other)
{
    if (a_Other.isInitialized())
        m_data.construct(*a_Other.m_data);
}

template<class T>
Optional<T>::Optional(SelfType&& a_Temp)
{
    if (a_Temp.isInitialized())
    {
        m_data.construct(std::move(*a_Temp.m_data));
        a_Temp.m_data.destroy();
    }
}

template<class T>
Optional<T>& Optional<T>::operator=(const SelfType& a_Other)
{
    if (isInitialized())
    {
        if (!a_Other.isInitialized())
            m_data.destroy();
        else
            m_data.assign(*a_Other.m_data);
    }
    else
    {
        if (a_Other.isInitialized())
            m_data.construct(*a_Other.m_data);
    }
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(SelfType&& a_Temp)
{
    if (isInitialized())
    {
        if (!a_Temp.isInitialized())
        {
            m_data.destroy();
        }
        else
        {
            m_data.assign(std::move(*a_Temp.m_data));
            a_Temp.m_data.destroy();
        }
    }
    else
    {
        if (a_Temp.isInitialized())
        {
            m_data.construct(std::move(*a_Temp.m_data));
            a_Temp.m_data.destroy();
        }
    }
    return *this;
}
template<class T>
Optional<T>::~Optional()
{
    destroy();
}

template<class T>
Optional<T>& Optional<T>::operator=(T&& a_Temp)
{
    if (isInitialized())
        m_data.assign(std::move(a_Temp));
    else
        m_data.construct(std::move(a_Temp));
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(T const& a_Val)
{
    if (isInitialized())
        m_data.assign(a_Val);
    else
        m_data.construct(a_Val);
    return *this;
}

template<class T>
void Optional<T>::destroy()
{
    if (isInitialized())
        m_data.destroy();
}
} // namespace phantom
