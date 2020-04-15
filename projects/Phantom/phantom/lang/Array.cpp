// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Array.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
void Array::copyConstruct(void* a_pDest, void const* a_pSrc) const
{
    size_t i = 0;
    size_t storedTypeSize = m_pUnderlyingType->getSize();
    for (; i < m_uiCount; ++i)
    {
        m_pUnderlyingType->copyConstruct((byte*)a_pDest + i * storedTypeSize, (byte const*)a_pSrc + i * storedTypeSize);
    }
}

void Array::moveConstruct(void* a_pDest, void* a_pSrc) const
{
    size_t i = 0;
    size_t storedTypeSize = m_pUnderlyingType->getSize();
    for (; i < m_uiCount; ++i)
    {
        m_pUnderlyingType->moveConstruct((byte*)a_pDest + i * storedTypeSize, (byte*)a_pSrc + i * storedTypeSize);
    }
}

void Array::copyAssign(void* a_pDest, void const* a_pSrc) const
{
    size_t i = 0;
    size_t storedTypeSize = m_pUnderlyingType->getSize();
    for (; i < m_uiCount; ++i)
    {
        m_pUnderlyingType->copyAssign((byte*)a_pDest + i * storedTypeSize, (byte const*)a_pSrc + i * storedTypeSize);
    }
}

void Array::moveAssign(void* a_pDest, void* a_pSrc) const
{
    size_t i = 0;
    size_t storedTypeSize = m_pUnderlyingType->getSize();
    for (; i < m_uiCount; ++i)
    {
        m_pUnderlyingType->moveAssign((byte*)a_pDest + i * storedTypeSize, (byte*)a_pSrc + i * storedTypeSize);
    }
}

void Array::onReferencedElementRemoved(LanguageElement* a_pItem)
{
    Type::onReferencedElementRemoved(a_pItem);
    if (m_pUnderlyingType == a_pItem)
    {
        m_pUnderlyingType = nullptr;
    }
}

void Array::construct(void* a_pBuffer) const
{
    byte*  pBuffer = (byte*)a_pBuffer;
    size_t size = m_pUnderlyingType->getSize();
    // trivial
    for (size_t i = 0; i < m_uiCount; ++i)
    {
        m_pUnderlyingType->construct(pBuffer);
        pBuffer += size;
    }
}

void Array::destroy(void* a_pBuffer) const
{
    // trivial
    byte*  pBuffer = (byte*)a_pBuffer;
    size_t size = m_pUnderlyingType->getSize();
    // trivial
    for (size_t i = 0; i < m_uiCount; ++i)
    {
        m_pUnderlyingType->destroy(pBuffer);
        pBuffer += size;
    }
}

bool Array::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const
{
    if (a_pType->asArray())
    {
        if (a_pType->asArray()->getItemCount() == m_uiCount)
        {
            a_Score += 10;
            return m_pUnderlyingType->partialAccepts(a_pType->removeArray(), a_Score, a_Deductions);
        }
    }
    return false;
}

bool Array::isSame(Symbol* a_pOther) const
{
    return (a_pOther == this) ||
    (a_pOther->asArray() && m_pUnderlyingType->isSame(static_cast<Array*>(a_pOther)->m_pUnderlyingType) &&
     m_uiCount == static_cast<Array*>(a_pOther)->m_uiCount);
}

hash64 Array::computeLocalHash() const
{
    hash64 h = getUnderlyingType()->computeLocalHash();
    CombineHash(h, '[');
    CombineHash(h, m_uiCount);
    CombineHash(h, ']');
    return h;
}

void Array::getFields(AggregateFields& _fields) const
{
    _fields.reserve(m_uiCount);
    Type*  pItemType = getItemType();
    size_t itemSize = pItemType->getSize();
    for (size_t i = 0; i < m_uiCount; ++i)
    {
        AggregateField f;
        f.type = pItemType;
        f.offset = i * itemSize;
        _fields.push_back(f);
    }
}

void Array::getFlattenedAggregateFields(AggregateFields& _aggregateFields) const
{
    Aggregate::getFlattenedFields(_aggregateFields);
}

Type* Array::replicate(Type* a_pInput) const
{
    return m_pUnderlyingType->replicate(a_pInput)->addArray(m_uiCount);
}

} // namespace lang
} // namespace phantom
