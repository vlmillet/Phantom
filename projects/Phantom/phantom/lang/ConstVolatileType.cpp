// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ConstVolatileType.h"

#include <phantom/utils/Delegate.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ConstVolatileType::ConstVolatileType(Type* a_pType)
    : QualifiedType(TypeKind::Unknown, a_pType, " const volatile", a_pType->isNative() ? a_pType->getSize() : 0,
                    a_pType->isNative() ? a_pType->getAlignment() : 0, PHANTOM_R_VOLATILE | PHANTOM_R_CONST,
                    a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
}

Type* ConstVolatileType::replicate(Type* a_pSource) const
{
    return m_pUnderlyingType->replicate(a_pSource)->addConstVolatile();
}

bool ConstVolatileType::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const
{
    if (a_pType->asConstVolatileType())
    {
        a_Score += 10;
        return m_pUnderlyingType
        ? m_pUnderlyingType->partialAccepts(a_pType->removeConstVolatile(), a_Score, a_Deductions)
        : false;
    }
    return false;
}

bool ConstVolatileType::isSame(Symbol* a_pOther) const
{
    return a_pOther == this ||
    (a_pOther->asConstVolatileType() &&
     m_pUnderlyingType->isSame(static_cast<ConstVolatileType*>(a_pOther)->m_pUnderlyingType));
}

hash64 ConstVolatileType::computeLocalHash() const
{
    hash64 h = getUnderlyingType()->computeLocalHash();
    CombineHash(h, hash64('�'));
    return h;
}

} // namespace lang
} // namespace phantom
