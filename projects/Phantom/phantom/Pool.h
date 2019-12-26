// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "BasicPool.h"

namespace phantom
{
//////////////////////////////////////////////////////////////////////////
/// The Pool class allows contiguous storage of items with very fast and
/// (almost) constant time allocation and deallocation of items.
/// It also allows to iterate over allocated items with iterators.
///
/// Implementation info:
/// Items are stored inside "blocks" of memory. Blocks are not contiguous in memory,
/// but items inside them are. The Pool also allocates 1 bit per item in a separate buffer
/// to keep track of allocated items. For simplicity reasons, blocks are never deallocated
/// (except when the Pool is destroyed). Free items are aliased with pointers to the next
/// free item for fast allocation/deallocation.
///
/// @note Iteration can be relatively slow if capacity() is much greater than size()
/// because the iterators internally iterate over all items to find allocated ones.
/// @note Storing items smaller than a pointer will result in wasting a lot of memory.
/// @note Custom alignment of items is not supported at the moment (but feasible).
//////////////////////////////////////////////////////////////////////////
template<class T, size_t N = 100>
class Pool : public BasicPool
{
public:
    typedef T value_type;

    typedef value_type*       pointer;
    typedef const value_type* const_pointer;

    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type is_always_equal;

    HAUNT_OFF template<class Ty>
    struct rebind
    {
        typedef Pool<Ty, N> other;
    };

    class iterator;

public:
    Pool(size_t _itemsPerBlock = N);
    ~Pool();

    Pool(const Pool<T, N>&) : BasicPool(sizeof(T), N)
    { // construct by copying (do nothing)
    }

    template<class Other, size_t Nn>
    Pool(const Pool<Other, Nn>&) : BasicPool(sizeof(T), N)
    { // construct from a related Pool (do nothing)
    }

    template<class Other, size_t Nn>
    Pool<T, N>& operator=(const Pool<Other, Nn>&)
    { // assign from a related Pool (do nothing)
        return (*this);
    }

    bool operator==(const Pool<T, N>&) const
    {
        return true;
    }

    T* New();
    template<class... Args>
    T*   New(Args... arg);
    void Delete(T* _item);
    void Delete(iterator _it);

    void   clear();
    size_t size() const
    {
        return BasicPool::size();
    }
    size_t capacity() const
    {
        return BasicPool::capacity();
    }

    bool isAllocated(const T* _item) const
    {
        return BasicPool::isAllocated((const uint8_t*)_item);
    }
    bool isAllocated(iterator _it) const;

    T* allocate()
    {
        return (T*)BasicPool::allocate();
    }
    T* allocate(size_t n)
    {
        PHANTOM_ASSERT(n == 1);
        return allocate();
    }
    void deallocate(T* _item)
    {
        BasicPool::deallocate((uint8_t*)_item);
    }
    void deallocate(T* _items, size_t n)
    {
        PHANTOM_ASSERT(n == 1);
        deallocate(_items);
    }

    iterator begin();
    iterator end();

    // Low level accessors - do NOT use items that are not allocated
    T& getItem(size_t _blockIdx, size_t _itemIdx)
    {
        return *(T*)BasicPool::getItem(_blockIdx, _itemIdx);
    }
    const T& getItem(size_t _blockIdx, size_t _itemIdx) const
    {
        return *(const T*)BasicPool::getItem(_blockIdx, _itemIdx);
    }
    size_t getBlockCount() const
    {
        return BasicPool::getBlockCount();
    }
    size_t getItemsPerBlock() const
    {
        return BasicPool::getItemsPerBlock();
    }
};

//---- Implementation ----

template<class T, size_t N>
class Pool<T, N>::iterator
{
    friend class Pool<T, N>;

public:
    iterator() : m_pool(NULL), m_blockIdx(0), m_itemIdx(0)
    {
    }

    iterator(Pool<T, N>* _pool, size_t _blockIdx, size_t _itemIdx)
        : m_pool(_pool), m_blockIdx(_blockIdx), m_itemIdx(_itemIdx)
    {
    }

    T& operator*()
    {
        PHANTOM_ASSERT(m_pool->isAllocated(*this));
        return m_pool->getItem(m_blockIdx, m_itemIdx);
    }

    T* operator->()
    {
        PHANTOM_ASSERT(m_pool->isAllocated(*this));
        return &m_pool->getItem(m_blockIdx, m_itemIdx);
    }

    iterator operator++(int)
    {
        iterator it = *this;
        ++(*this);
        return it;
    }

    iterator& operator++()
    {
        PHANTOM_ASSERT(m_blockIdx < m_pool->getBlockCount() &&
                       m_itemIdx < m_pool->getItemsPerBlock());

        do
        {
            m_itemIdx++;
            if (m_itemIdx == m_pool->getItemsPerBlock())
            {
                m_itemIdx = 0;
                m_blockIdx++;
            }
        } while (m_blockIdx < m_pool->getBlockCount() // reached end()
                 && !m_pool->isAllocated(*this));

        return *this;
    }

    bool operator==(const iterator& _rhs) const
    {
        PHANTOM_ASSERT(m_pool == _rhs.m_pool);
        return m_blockIdx == _rhs.m_blockIdx && m_itemIdx == _rhs.m_itemIdx;
    }

    bool operator!=(const iterator& _rhs) const
    {
        return !(*this == _rhs);
    }

private:
    Pool<T, N>* m_pool;
    size_t      m_blockIdx;
    size_t      m_itemIdx;
};

template<class T, size_t N>
Pool<T, N>::Pool(size_t _itemsPerBlock) : BasicPool(sizeof(T), _itemsPerBlock)
{
}

template<class T, size_t N>
Pool<T, N>::~Pool()
{
    clear();
}

template<class T, size_t N>
void Pool<T, N>::clear()
{
    for (iterator it = begin(); it != end(); ++it)
    {
        Delete(it);
    }
}

template<class T, size_t N>
T* Pool<T, N>::New()
{
    T* item = (T*)BasicPool::allocate();
    new (item) T; // no brackets = no zero initialization
    return item;
}

template<class T, size_t N>
template<class... Args>
T* Pool<T, N>::New(Args... arg)
{
    T* item = (T*)BasicPool::allocate();
    new (item) T(arg...);
    return item;
}

template<class T, size_t N>
void Pool<T, N>::Delete(T* _item)
{
    _item->~T();
    return BasicPool::deallocate((uint8_t*)_item);
}

template<class T, size_t N>
typename Pool<T, N>::iterator Pool<T, N>::begin()
{
    // If Pool empty just return end()
    if (size() == 0)
        return end();

    // Else, search the first allocated item
    iterator it(this, 0, 0);

    if (!isAllocated(it))
    {
        ++it;
    }

    return it;
}

template<class T, size_t N>
typename Pool<T, N>::iterator Pool<T, N>::end()
{
    return iterator(this, getBlockCount(), 0);
}

template<class T, size_t N>
void Pool<T, N>::Delete(iterator _it)
{
    T* item = &*_it;
    item->~T();
    return BasicPool::deallocate(_it.m_blockIdx, _it.m_itemIdx);
}

template<class T, size_t N>
bool Pool<T, N>::isAllocated(iterator _it) const
{
    return BasicPool::isAllocated(_it.m_blockIdx, _it.m_itemIdx);
}
} // namespace phantom
