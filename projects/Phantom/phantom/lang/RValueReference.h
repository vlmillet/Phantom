// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Reference.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
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
    RValueReference* asRValueReference() const override
    {
        return const_cast<RValueReference*>(this);
    }
    RValueReference* asClassRValueReference() const override
    {
        return (m_pUnderlyingType AND m_pUnderlyingType->asClass())
        ? const_cast<RValueReference*>(this)
        : nullptr;
    }
    RValueReference* asConstClassRValueReference() const override
    {
        return (m_pUnderlyingType AND m_pUnderlyingType->asConstClass())
        ? const_cast<RValueReference*>(this)
        : nullptr;
    }

    Type* addLValueReference() const override;

    Type* addRValueReference() const override;

    Type* addPointer() const override;

    Type* removeRValueReference() const override
    {
        return m_pUnderlyingType;
    }

    Type* removeAllConst() const override;

    Type* removeAllQualifiers() const override;

    Type* replicate(Type* a_pInput) const override;

    void getUniqueName(StringBuffer& a_Buf) const override;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

    bool isCopyable() const override
    {
        return true;
    }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const override;

    bool isSame(Symbol* a_pOther) const override;
};

} // namespace reflection
} // namespace phantom
