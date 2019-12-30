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
class SmallMap : public SmallVectorBasedContainer<Pair<const K, V>, S, D>
{
public:
    typedef SmallMap<K, V, S, D, Pred>                        SelfType;
    typedef SmallVectorBasedContainer<Pair<const K, V>, S, D> BaseType;
    typedef K                                                 key_type;
    typedef V                                                 mapped_type;
    typedef PHANTOM_TYPENAME BaseType::value_type value_type;
    typedef PHANTOM_TYPENAME BaseType::iterator iterator;
    typedef PHANTOM_TYPENAME BaseType::const_iterator const_iterator;
    using BaseType::erase;
    using BaseType::operator[];

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
    SmallMap() = default;
    SmallMap(std::initializer_list<value_type> a_InitList)
    {
        for (auto const& pair : a_InitList)
            insert(pair);
    }
    explicit SmallMap(CustomAllocator const* a_pMemTraits) : BaseType(a_pMemTraits){};

    V& operator[](K const& _key)
    {
        auto it = lower_bound(_key);
        if (it != this->end() && _eq(it->first, _key))
        {
            return it->second;
        }
        return this->m_storage.insert(it, value_type(_key, V()))->second;
    }

    const_iterator find(K const& _key) const
    {
        const_iterator it = lower_bound(_key);
        if (it != this->end() && _eq(it->first, _key))
            return it;
        return this->end();
    }

    iterator find(K const& _key)
    {
        iterator it = lower_bound(_key);
        if (it != this->end() && _eq(it->first, _key))
            return it;
        return this->end();
    }

    size_t erase(K const& _key)
    {
        iterator it = lower_bound(_key);
        if (it != this->end() && _eq(it->first, _key))
        {
            erase(it);
            return 1;
        }
        return 0;
    }

    iterator insert(value_type const& a_Pair)
    {
        iterator it = lower_bound(a_Pair.first);
        if (it != this->end() && _eq(it->first, a_Pair.first))
        {
            it->second = a_Pair.second;
            return it;
        }
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

    void swap(SelfType& a_Other)
    {
        typedef SmallVector<Pair<K, V>, S, D> StorageTypeNoConst;
        ((StorageTypeNoConst&)this->m_storage).swap((StorageTypeNoConst&)a_Other.m_storage);
    }

private:
    static bool _eq(K const& k0, K const& k1)
    {
        Pred p;
        return !p(k0, k1) && !p(k1, k0);
    }
};
} // namespace phantom
