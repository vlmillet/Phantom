// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "VolatileType.h"

#include "ConstVolatileType.h"

#include <phantom/utils/Delegate.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
VolatileType::VolatileType(Type* a_pType)
    : QualifiedType(TypeKind::Unknown, a_pType, " volatile", a_pType->isNative() ? a_pType->getSize() : 0,
                    a_pType->isNative() ? a_pType->getAlignment() : 0, PHANTOM_R_VOLATILE,
                    a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
}

bool VolatileType::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const
{
    if (a_pType->asVolatileType())
    {
        a_Score += 10;
        return m_pUnderlyingType ? m_pUnderlyingType->partialAccepts(a_pType->removeVolatile(), a_Score, a_Deductions)
                                 : false;
    }
    return false;
}

bool VolatileType::isSame(Symbol* a_pOther) const
{
    return a_pOther == this ||
    (a_pOther->asVolatileType() && m_pUnderlyingType->isSame(static_cast<VolatileType*>(a_pOther)->m_pUnderlyingType));
}

Type* VolatileType::replicate(Type* a_pInput) const
{
    return m_pUnderlyingType->replicate(a_pInput)->addVolatile();
}

hash64 VolatileType::computeLocalHash() const
{
    hash64 h = getUnderlyingType()->computeLocalHash();
    CombineHash(h, '#');
    return h;
}

} // namespace lang
} // namespace phantom
