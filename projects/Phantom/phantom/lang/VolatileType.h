// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/QualifiedType.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Reprepsents a volatile type.
class PHANTOM_EXPORT_PHANTOM VolatileType : public QualifiedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(VolatileType);

    friend class phantom::lang::Namespace;

public:
    VolatileType(Type* a_pType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the underlying volatiled type.
    ///
    /// \return The volatiled type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getVolatiledType() const { return m_pUnderlyingType; }

    Type* replicate(Type* a_pSource) const override;

    virtual VolatileType* asVolatileClass() const
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<VolatileType*>(this) : nullptr;
    }

    void* cast(Type* a_pTargetType, void* a_pSrc) const override
    {
        return m_pUnderlyingType->cast(a_pTargetType, a_pSrc);
    }
    void* upcast(Type* a_pTargetType, void* a_pSrc) const override
    {
        return m_pUnderlyingType->upcast(a_pTargetType, a_pSrc);
    }
    void* downcast(Type* a_pTargetType, void* a_pSrc) const override
    {
        return m_pUnderlyingType->downcast(a_pTargetType, a_pSrc);
    }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const override;

    bool isSame(Symbol* a_pOther) const override;

    hash64 computeLocalHash() const override;
};

} // namespace lang
} // namespace phantom
