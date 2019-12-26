// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/SmallVector.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
template<class t_Ty = void>
struct Less
{
    typedef t_Ty first_argument_type;
    typedef t_Ty second_argument_type;
    typedef bool result_type;

    constexpr bool operator()(const t_Ty& _Left, const t_Ty& _Right) const
    {
        return (_Left < _Right);
    }
};

template<class T, size_t S, size_t D>
class SmallVectorBasedContainer
{
    using StorageType = SmallVector<T, S, D>;

public:
    using value_type = PHANTOM_TYPENAME     StorageType::value_type;
    using iterator = PHANTOM_TYPENAME       StorageType::iterator;
    using const_iterator = PHANTOM_TYPENAME StorageType::const_iterator;

public:
    SmallVectorBasedContainer() = default;
    explicit SmallVectorBasedContainer(MemoryTraits const* a_pMemTraits) : m_storage(a_pMemTraits){};
    iterator erase(const_iterator a_Where)
    {
        return m_storage.erase(a_Where);
    }

    iterator erase(const_iterator a_From, const_iterator a_To)
    {
        return m_storage.erase(a_From, a_To);
    }

    const_iterator begin() const
    {
        return m_storage.begin();
    }
    const_iterator end() const
    {
        return m_storage.end();
    }
    iterator begin()
    {
        return m_storage.begin();
    }
    iterator end()
    {
        return m_storage.end();
    }

    T const* data() const
    {
        return m_storage.data();
    }
    size_t size() const
    {
        return m_storage.size();
    }

    void clear()
    {
        m_storage.clear();
    }
    bool empty() const
    {
        return m_storage.empty();
    }

    T const& operator[](size_t i) const
    {
        return m_storage[i];
    }

protected:
    HAUNT_OFF StorageType m_storage;
};
} // namespace phantom
