// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "BasicPool.h"

namespace phantom
{
size_t BasicPool::getAllocationBytesPerBlock() const
{
    return (m_itemsPerBlock / 8u) + 1u;
}

BasicPool::BasicPool(size_t _itemSize, size_t _itemsPerBlock)
    : m_itemSize(std::max(_itemSize, sizeof(uint8_t*))),
      m_itemsPerBlock(_itemsPerBlock),
      m_nextFreeItem(NULL),
      m_freeItemCount(0)
{
    PHANTOM_ASSERT(_itemsPerBlock > 0);
}

BasicPool::~BasicPool()
{
    // No need to call clear() here, juste free the blocks
    for (size_t i = 0; i < m_blocks.size(); ++i)
    {
        PHANTOM_FREE(m_blocks[i]);
    }
}

void BasicPool::setItemsPerBlock(size_t _itemsPerBlock)
{
    PHANTOM_ASSERT(capacity() == 0);
    m_itemsPerBlock = _itemsPerBlock;
}

void BasicPool::clear()
{
    for (size_t blockIdx = 0; blockIdx < m_blocks.size(); ++blockIdx)
    {
        for (size_t itemIdx = 0; itemIdx < m_itemsPerBlock; ++itemIdx)
        {
            if (isAllocated(blockIdx, itemIdx))
            {
                deallocate(blockIdx, itemIdx);
            }
        }
    }
}

void BasicPool::addBlock()
{
    m_freeItemCount += m_itemsPerBlock;
    m_allocatedItems.resize(m_allocatedItems.size() + getAllocationBytesPerBlock(), 0);

    uint8_t* newBlock = (uint8_t*)PHANTOM_MALLOC(m_itemSize * m_itemsPerBlock);
    m_blocks.push_back(newBlock);

    for (size_t i = 0; i < m_itemsPerBlock - 1; ++i)
    {
        FreeItem* item0 = (FreeItem*)(newBlock + i * m_itemSize);
        FreeItem* item1 = (FreeItem*)(newBlock + (i + 1) * m_itemSize);
        item0->m_next = item1;
    }

    FreeItem* lastItem = (FreeItem*)(newBlock + (m_itemsPerBlock - 1) * m_itemSize);
    lastItem->m_next = m_nextFreeItem;
    m_nextFreeItem = (FreeItem*)newBlock;
}
} // namespace phantom
