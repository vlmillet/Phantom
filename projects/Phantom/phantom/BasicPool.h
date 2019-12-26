// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "SmallVector.h"

#include <phantom/phantom.h>

namespace phantom
{
//////////////////////////////////////////////////////////////////////////
/// The BasicPool class is a common implementation base for the templated pool<T> class.
/// If you are looking for a pool container, take a look at pool<T> instead.
//////////////////////////////////////////////////////////////////////////
class PHANTOM_EXPORT_PHANTOM BasicPool
{
public:
    BasicPool(size_t _itemSize, size_t _itemsPerBlock);
    ~BasicPool();

    inline uint8_t* allocate();
    inline void     deallocate(uint8_t* _item);

    void   clear();
    size_t size() const
    {
        return capacity() - m_freeItemCount;
    }
    size_t capacity() const
    {
        return m_blocks.size() * m_itemsPerBlock;
    }

    size_t getItemSize() const
    {
        return m_itemSize;
    }
    size_t getItemsPerBlock() const
    {
        return m_itemsPerBlock;
    }

    inline bool isAllocated(const uint8_t* _item) const;

    // low level accessors
    size_t getBlockCount() const
    {
        return m_blocks.size();
    }
    uint8_t* getItem(size_t _blockIdx, size_t _itemIdx)
    {
        return m_blocks[_blockIdx] + _itemIdx * m_itemSize;
    }
    const uint8_t* getItem(size_t _blockIdx, size_t _itemIdx) const
    {
        return m_blocks[_blockIdx] + _itemIdx * m_itemSize;
    }
    inline bool isAllocated(size_t _blockIdx, size_t _itemIdx) const;

    /// Number of items per block can only be changed before the pool is used (ie. capacity is 0)
    void setItemsPerBlock(size_t _itemsPerBlock);

protected:
    void        addBlock();
    inline void deallocate(size_t _blockIdx, size_t _itemIdx);
    inline bool getItemIdx(const uint8_t* _item, size_t& _blockIdx, size_t& _itemIdx) const;
    inline void setAllocBit(const uint8_t* _item, bool _allocated);
    inline void setAllocBit(size_t _blockIdx, size_t _itemIdx, bool _allocated);

    /// Returns the number of bytes per block required to store allocation bits
    size_t getAllocationBytesPerBlock() const;

    struct FreeItem
    {
        FreeItem* m_next;
    };

    SmallVector<uint8_t*, 50> m_blocks;
    SmallVector<uint8_t, 50>
                 m_allocatedItems; /// Stores 1 bit per item to know if the item is allocated
    const size_t m_itemSize;
    size_t       m_itemsPerBlock;
    FreeItem*    m_nextFreeItem;
    size_t       m_freeItemCount;
};

uint8_t* BasicPool::allocate()
{
    if (!m_nextFreeItem)
    {
        addBlock();
    }

    FreeItem* item = m_nextFreeItem;
    m_nextFreeItem = item->m_next;

    m_freeItemCount--;
    setAllocBit((uint8_t*)item, true);

    return (uint8_t*)item;
}

void BasicPool::deallocate(uint8_t* _item)
{
    size_t blockIdx, itemIdx;
    bool   r = getItemIdx(_item, blockIdx, itemIdx);
    PHANTOM_VERIFY(r);
    if (r) // This item pointer is not in this pool!
    {
        deallocate(blockIdx, itemIdx);
    }
}

void BasicPool::deallocate(size_t _blockIdx, size_t _itemIdx)
{
    m_freeItemCount++;
    setAllocBit(_blockIdx, _itemIdx, false);

    FreeItem* item = (FreeItem*)(m_blocks[_blockIdx] + _itemIdx * m_itemSize);
    item->m_next = m_nextFreeItem;
    m_nextFreeItem = item;
}

bool BasicPool::isAllocated(size_t _blockIdx, size_t _itemIdx) const
{
    PHANTOM_ASSERT(_blockIdx < m_blocks.size());
    PHANTOM_ASSERT(_itemIdx < m_itemsPerBlock);

    // Number of bytes per block used to store allocation bits
    const size_t allocBytesPerBlock = getAllocationBytesPerBlock();

    // Allocation bits for this block start at this index
    size_t blockAllocIdx = allocBytesPerBlock * _blockIdx;

    // Location of the bit to change
    size_t byteIdx = _itemIdx / 8;
    size_t bitIdx = _itemIdx % 8;

    uint8_t allocByte = m_allocatedItems[blockAllocIdx + byteIdx];

    return (allocByte & (1u << bitIdx)) != 0;
}

bool BasicPool::isAllocated(const uint8_t* _item) const
{
    size_t blockIdx, itemIdx;
    bool   r = getItemIdx(_item, blockIdx, itemIdx);
    PHANTOM_VERIFY(r);
    if (r) // This item pointer is not in this pool!
    {
        return isAllocated(blockIdx, itemIdx);
    }
    return false;
}

void BasicPool::setAllocBit(const uint8_t* _item, bool _allocated)
{
    size_t blockIdx, itemIdx;
    bool   r = getItemIdx(_item, blockIdx, itemIdx);
    PHANTOM_VERIFY(r);
    if (r) // This item pointer is not in this pool!
    {
        setAllocBit(blockIdx, itemIdx, _allocated);
    }
}

void BasicPool::setAllocBit(size_t _blockIdx, size_t _itemIdx, bool _allocated)
{
    // Number of bytes per block used to store allocation bits
    const size_t allocBytesPerBlock = getAllocationBytesPerBlock();

    // Allocation bits for this block start at this byte index
    size_t blockAllocIdx = allocBytesPerBlock * _blockIdx;

    // Location of the bit to change
    size_t byteIdx = _itemIdx / 8;
    size_t bitIdx = _itemIdx % 8;

    uint8_t& allocByte = m_allocatedItems[blockAllocIdx + byteIdx];

    if (_allocated)
    {
        // Set the bit to 1 (= allocated)
        PHANTOM_ASSERT((allocByte & (1u << bitIdx)) == 0); // This item was already allocated!
        allocByte |= (1u << bitIdx);
    }
    else
    {
        // Set the bit to 0 (= free)
        PHANTOM_ASSERT((allocByte & (1u << bitIdx)) != 0); // This item was already destroyed!
        allocByte &= ~(1u << bitIdx);
    }
}

bool BasicPool::getItemIdx(const uint8_t* _item, size_t& _blockIdx, size_t& _itemIdx) const
{
    // Find the block containing this item
    for (size_t blockIdx = 0; blockIdx < m_blocks.size(); ++blockIdx)
    {
        if (_item >= m_blocks[blockIdx] &&
            _item < (m_blocks[blockIdx] + m_itemSize * m_itemsPerBlock))
        {
            // Found the block
            PHANTOM_ASSERT(((_item - m_blocks[blockIdx]) % m_itemSize) ==
                           0); // Misaligned pointer?!

            _blockIdx = blockIdx;
            _itemIdx = (_item - m_blocks[blockIdx]) / m_itemSize;

            return true;
        }
    }
    return false;
}
} // namespace phantom
