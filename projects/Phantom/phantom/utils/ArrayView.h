// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/detail/core.h>

namespace phantom
{
template<class T>
class Optional;

template<class T>
class ArrayView
{
public:
    ArrayView() = default;
    ArrayView(const T* a_pData, size_t a_Size) : m_pBegin(a_pData), m_pEnd(m_pBegin + a_Size)
    {
        PHANTOM_ASSERT(m_pBegin);
    }
    ArrayView(const T* a_pData, const T* a_pEnd) : m_pBegin(a_pData), m_pEnd(a_pEnd)
    {
        PHANTOM_ASSERT(m_pBegin <= m_pEnd);
    }
    ArrayView(std::initializer_list<T> a_InitList) : m_pBegin(a_InitList.begin()), m_pEnd(a_InitList.end())
    {
        PHANTOM_ASSERT(m_pBegin <= m_pEnd);
    }

    const T* begin() const
    {
        return m_pBegin;
    }
    const T* end() const
    {
        return m_pEnd;
    }
    const T* data() const
    {
        return m_pBegin;
    }
    size_t size() const
    {
        return m_pEnd - m_pBegin;
    }
    bool empty() const
    {
        return m_pEnd == m_pBegin;
    }

    T const& front() const
    {
        PHANTOM_ASSERT(size());
        return *m_pBegin;
    }
    T const& back() const
    {
        PHANTOM_ASSERT(size());
        return *(m_pEnd - 1);
    }

    T const& operator[](size_t i) const
    {
        PHANTOM_ASSERT(i < size());
        return m_pBegin[i];
    }

private:
    const T* m_pBegin = nullptr;
    const T* m_pEnd = nullptr;
};

template<class Container>
auto MakeArrayView(Container const& c)
{
    return ArrayView<typename Container::value_type>(c.begin(), c.end());
}
} // namespace phantom
