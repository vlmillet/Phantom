// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ExtendedType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Represents an array type (ex: int[5]).
/////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM Array : public ExtendedType, public Aggregate
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Array);

public:
    friend class Type;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Array(Type* a_pType, size_t a_uiCount)
        : ExtendedType(TypeKind::Array, a_pType, String("[") + (a_uiCount ? StringUtil::ToString(a_uiCount) : "") + ']',
                       a_uiCount == 0 ? sizeof(void*) : a_pType->isNative() ? a_pType->getSize() * a_uiCount : 0,
                       a_uiCount == 0 ? PHANTOM_ALIGNOF(void*) : a_pType->isNative() ? a_pType->getAlignment() : 0, 0,
                       a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT),

          Aggregate(this),
          m_uiCount(a_uiCount) /// m_uiCount == 0 => unknown bound array
    {
    }

public:
    bool isDefaultConstructible() const override { return m_pUnderlyingType->isDefaultConstructible(); }

    Type* asPOD() const override { return m_pUnderlyingType->asPOD() ? (Type*)this : nullptr; }
    bool  isPOD() const override { return m_pUnderlyingType->isPOD(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets item count of this array type.
    ///
    /// \return The item count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getItemCount() const { return m_uiCount; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets underlying item type of this array type.
    ///
    /// \return The item type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getItemType() const { return m_pUnderlyingType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets item address at given index from an array address.
    ///
    /// \param a_pArrayAddress The array address.
    /// \param  a_uiIndex      The index.
    ///
    /// \return the item address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getItemAddress(void* a_pArrayAddress, size_t a_uiIndex) const
    {
        return ((byte*)a_pArrayAddress) + (a_uiIndex * m_pUnderlyingType->getSize());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets item address at given index from a const array address.
    ///
    /// \param a_pArrayAddress The const array address.
    /// \param  a_uiIndex      The index.
    ///
    /// \return the item address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const void* getItemAddress(const void* a_pArrayAddress, size_t a_uiIndex) const
    {
        return ((const byte*)a_pArrayAddress) + (a_uiIndex * m_pUnderlyingType->getSize());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets item value at given address from an array address.
    ///
    /// \param  a_pArrayAddress The array address.
    /// \param  a_uiIndex       The index.
    /// \param  a_pDest         The destination address to store the item value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getItemValue(const void* a_pArrayAddress, size_t a_uiIndex, void* a_pDest) const
    {
        return m_pUnderlyingType->copyConstruct(a_pDest, getItemAddress(a_pArrayAddress, a_uiIndex));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets item value at given address from an array address.
    ///
    /// \param  a_pArrayAddress The array address.
    /// \param  a_uiIndex       The index.
    /// \param  a_pDest         The source address where to load the item value from.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setItemValue(void* a_pArrayAddress, size_t a_uiIndex, const void* a_pSrc) const
    {
        return m_pUnderlyingType->copyConstruct(getItemAddress(a_pArrayAddress, a_uiIndex), a_pSrc);
    }

    void construct(void* a_pBuffer) const override;
    void destroy(void* a_pBuffer) const override;

    void* allocate() const override { return m_pUnderlyingType->allocate(m_uiCount); }
    void  deallocate(void* a_pInstance) const override { m_pUnderlyingType->deallocate(a_pInstance, m_uiCount); }
    void* allocate(size_t a_uiCount) const override { return m_pUnderlyingType->allocate(m_uiCount * a_uiCount); }
    void  deallocate(void* a_pChunk, size_t a_uiCount) const override
    {
        m_pUnderlyingType->deallocate(a_pChunk, m_uiCount * a_uiCount);
    }

    void copyAssign(void* a_pDest, void const* a_pSrc) const override;
    void moveAssign(void* a_pDest, void* a_pSrc) const override;
    void copyConstruct(void* a_pDest, void const* a_pSrc) const override;
    void moveConstruct(void* a_pDest, void* a_pSrc) const override;

    bool isCopyable() const override { return m_pUnderlyingType->isCopyable(); }
    bool isMoveable() const override { return m_pUnderlyingType->isMoveable(); }

    bool isCopyConstructible() const override { return m_pUnderlyingType->isCopyConstructible(); }
    bool isCopyAssignable() const override { return m_pUnderlyingType->isCopyAssignable(); }
    bool isMoveConstructible() const override { return m_pUnderlyingType->isMoveConstructible(); }
    bool isMoveAssignable() const override { return m_pUnderlyingType->isMoveAssignable(); }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const override;

    bool isSame(Symbol* a_pOther) const override;

    hash64 computeLocalHash() const override;
    void   getFields(AggregateFields& _fields) const override;
    void   getFlattenedAggregateFields(AggregateFields& _aggregateFields) const override;

    Type* replicate(Type* a_pInput) const override;

protected:
    size_t getElementCount() const { return m_uiCount; } // just to check compile error, must be removed !!

protected:
    size_t m_uiCount;
};

} // namespace lang
} // namespace phantom
