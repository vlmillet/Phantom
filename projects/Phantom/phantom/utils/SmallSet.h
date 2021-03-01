// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Pair.h"

#include <functional>
#include <phantom/utils/SmallVectorBasedContainer.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
template<class T, uint32_t S, class Pred>
class SmallSet : public SmallVectorBasedContainer<T, S>
{
public:
    typedef SmallSet<T, S, Pred>            SelfType;
    typedef SmallVectorBasedContainer<T, S> BaseType;
    typedef PHANTOM_TYPENAME BaseType::value_type value_type;
    typedef PHANTOM_TYPENAME BaseType::iterator iterator;
    typedef PHANTOM_TYPENAME BaseType::const_iterator const_iterator;
    using BaseType::erase;

public:
    SmallSet() = default;
    SmallSet(std::initializer_list<value_type> a_InitList)
    {
        for (auto const& val : a_InitList)
            insert(val);
    }
    explicit SmallSet(CustomAllocator const* a_pMemTraits) : BaseType(a_pMemTraits){};

    T const& operator[](size_t i) const { return this->m_storage[i]; }

    const_iterator find(T const& a_Val) const
    {
        const_iterator it = lower_bound(a_Val);
        if (it != this->end() && _eq(*it, a_Val))
            return it;
        return this->end();
    }

    iterator find(T const& a_Val)
    {
        iterator it = lower_bound(a_Val);
        if (it != this->end() && _eq(*it, a_Val))
            return it;
        return this->end();
    }

    size_t erase(T const& a_Val)
    {
        iterator it = lower_bound(a_Val);
        if (it != this->end() && _eq(*it, a_Val)) // ==
        {
            erase(it);
            return 1;
        }
        return 0;
    }

    Pair<iterator, bool> insert(T const& a_Val)
    {
        iterator it = lower_bound(a_Val);
        if (it != this->end() && _eq(*it, a_Val)) // ==
        {
            return Pair<iterator, bool>(it, false);
        }
        return Pair<iterator, bool>(this->m_storage.insert(it, a_Val), true);
    }

    const_iterator lower_bound(const T& a_Val) const
    {
        return std::lower_bound(this->begin(), this->end(), a_Val, Pred());
    }

    iterator lower_bound(const T& a_Val) { return (iterator) const_cast<SelfType const*>(this)->lower_bound(a_Val); }

    void swap(SelfType& a_Other) { this->m_storage.swap(a_Other.m_storage); }

private:
    static bool _eq(T const& k0, T const& k1)
    {
        Pred p;
        return !p(k0, k1) && !p(k1, k0);
    }
};
} // namespace phantom
