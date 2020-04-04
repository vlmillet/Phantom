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
Array::Array(Type* a_pType, size_t a_uiCount)
    : Type(TypeKind::Array, a_pType, String("[") + (a_uiCount ? phantom::lexical_cast<String>(a_uiCount) : "") + ']',
           a_uiCount == 0 ? sizeof(void*) : a_pType->isNative() ? a_pType->getSize() * a_uiCount : 0,
           a_uiCount == 0 ? PHANTOM_ALIGNOF(void*) : a_pType->isNative() ? a_pType->getAlignment() : 0, 0,
           a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT),

      Aggregate(this),
      m_uiCount(a_uiCount) /// m_uiCount == 0 => unknown bound array
{
}

Array::~Array() {}

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

void Array::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    char buf[32];
    int  n = snprintf(buf, 32, "[%zu]", m_uiCount);
    a_Buf.append(buf, n);
}

void Array::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    char buf[32];
    int  n = snprintf(buf, 32, "[%zu]", m_uiCount);
    a_Buf.append(buf, n);
}

void Array::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    char buf[32];
    int  n = snprintf(buf, 32, "[%zu]", m_uiCount);
    a_Buf.append(buf, n);
}

void Array::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    char buf[32];
    int  n = snprintf(buf, 32, "[%zu]", m_uiCount);
    a_Buf.append(buf, n);
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
