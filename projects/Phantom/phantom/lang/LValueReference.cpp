// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "LValueReference.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
LValueReference::LValueReference(Type* a_pReferencedType) : Reference(TypeKind::LValueReference, a_pReferencedType, "&")
{
}

LValueReference* LValueReference::asClassLValueReference() const
{
    return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<LValueReference*>(this) : nullptr;
}

LValueReference* LValueReference::asConstClassLValueReference() const
{
    return (m_pUnderlyingType && m_pUnderlyingType->asConstClass()) ? const_cast<LValueReference*>(this) : nullptr;
}

Type* LValueReference::replicate(Type* a_pInput) const
{
    return m_pUnderlyingType->replicate(a_pInput)->addLValueReference();
}

bool LValueReference::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const
{
    if (a_pType->asLValueReference())
    {
        a_Score += 10;
        return m_pUnderlyingType
        ? m_pUnderlyingType->partialAccepts(a_pType->removeReference(), a_Score, a_DeducedConstants)
        : false;
    }
    return false;
}

bool LValueReference::isSame(Symbol* a_pOther) const
{
    return a_pOther == this ||
    (a_pOther->asLValueReference() &&
     m_pUnderlyingType->isSame(static_cast<LValueReference*>(a_pOther)->m_pUnderlyingType));
}

hash64 LValueReference::computeLocalHash() const
{
    hash64 h = getUnderlyingType()->computeLocalHash();
    CombineHash(h, '&');
    return h;
}

} // namespace lang
} // namespace phantom
