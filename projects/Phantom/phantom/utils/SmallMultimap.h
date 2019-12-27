// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
template<class K, class V, size_t S, size_t D, class Pred>
class SmallMultimap : public SmallVectorBasedContainer<Pair<const K, V>, S, D>
{
public:
    typedef SmallMultimap<K, V, S, D, Pred>                   SelfType;
    typedef SmallVectorBasedContainer<Pair<const K, V>, S, D> BaseType;
    typedef PHANTOM_TYPENAME BaseType::value_type value_type;
    typedef PHANTOM_TYPENAME BaseType::iterator iterator;
    typedef PHANTOM_TYPENAME BaseType::const_iterator const_iterator;
    using BaseType::erase;

private:
    struct PredWrapper
    {
        bool operator()(value_type const& l, K const& r)
        {
            return pred(l.first, r);
        }
        bool operator()(K const& l, value_type const& r)
        {
            return pred(l, r.first);
        }
        Pred pred;
    };

public:
    SmallMultimap() = default;
    SmallMultimap(std::initializer_list<value_type> a_InitList)
    {
        for (auto const& pair : a_InitList)
            insert(pair);
    }
    explicit SmallMultimap(MemoryTraits const* a_pMemTraits) : BaseType(a_pMemTraits){};
    V& operator[](K const& _key)
    {
        auto it = upper_bound(_key);
        return this->m_storage.insert(it, value_type(_key, V()))->second;
    }

    const_iterator find(K const& _key) const
    {
        return lower_bound(_key);
    }

    iterator find(K const& _key)
    {
        return lower_bound(_key);
    }

    size_t erase(K const& _key)
    {
        iterator it = lower_bound(_key);
        if (it != this->end())
        {
            iterator last = upper_bound(_key);
            size_t   count = last - it;
            this->m_storage.erase(it, last);
            return count;
        }
        return 0;
    }

    iterator insert(value_type const& a_Pair)
    {
        iterator it = upper_bound(a_Pair.first);
        return this->m_storage.insert(it, a_Pair);
    }

    const_iterator lower_bound(const K& _key) const
    {
        return std::lower_bound(this->begin(), this->end(), _key, PredWrapper());
    }

    iterator lower_bound(const K& _key)
    {
        return (iterator) const_cast<SelfType const*>(this)->lower_bound(_key);
    }

    const_iterator upper_bound(const K& _key) const
    {
        return std::upper_bound(
        this->begin(), this->end(), value_type(_key, V()),
        [](value_type const& l, value_type const& r) -> bool { return Pred()(l.first, r.first); });
    }

    iterator upper_bound(const K& _key)
    {
        return (iterator) const_cast<SelfType const*>(this)->upper_bound(_key);
    }

    void swap(SelfType& a_Other)
    {
        typedef SmallVector<Pair<K, V>, S, D> StorageTypeNoConst;
        ((StorageTypeNoConst&)this->m_storage).swap((StorageTypeNoConst&)a_Other.m_storage);
    }
};
} // namespace phantom
