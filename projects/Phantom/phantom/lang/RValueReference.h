// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Reference.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents an r-value reference type.
/// \code{.cpp}
///     ex: int&&, float&&, unsigned const&&, etc...
/// \endcode
class PHANTOM_EXPORT_PHANTOM RValueReference : public Reference
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(RValueReference);

public:
    friend class Type;

protected:
    RValueReference(Type* a_pReferencedType);

public:
    RValueReference* asClassRValueReference() const override
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<RValueReference*>(this) : nullptr;
    }
    RValueReference* asConstClassRValueReference() const override
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asConstClass()) ? const_cast<RValueReference*>(this) : nullptr;
    }

    Type* replicate(Type* a_pInput) const override;

    void getUniqueName(StringBuffer& a_Buf) const override;

    void   getQualifiedName(StringBuffer& a_Buf) const override;
    void   getDecoratedName(StringBuffer& a_Buf) const override;
    void   getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    hash64 computeLocalHash() const override;

    bool isCopyable() const override { return true; }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const override;

    bool isSame(Symbol* a_pOther) const override;
};

} // namespace lang
} // namespace phantom
