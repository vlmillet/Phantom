// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "ConstType.h"

#include "ConstVolatileType.h"

#include <phantom/utils/Delegate.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
ConstType::ConstType(Type* a_pType)
    : QualifiedType(TypeKind::Unknown, a_pType, " const", a_pType->isNative() ? a_pType->getSize() : 0,
                    a_pType->isNative() ? a_pType->getAlignment() : 0, PHANTOM_R_CONST,
                    a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
}

Type* ConstType::replicate(Type* a_pSource) const
{
    return m_pUnderlyingType->replicate(a_pSource)->addConst();
}

bool ConstType::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const
{
    if (a_pType->asConstType())
    {
        a_Score += 10;
        return m_pUnderlyingType ? m_pUnderlyingType->partialAccepts(a_pType->removeConst(), a_Score, a_Deductions)
                                 : false;
    }
    return false;
}

bool ConstType::isSame(Symbol* a_pOther) const
{
    return a_pOther == this ||
    (a_pOther->asConstType() && m_pUnderlyingType->isSame(static_cast<ConstType*>(a_pOther)->m_pUnderlyingType));
}

hash64 ConstType::computeLocalHash() const
{
    hash64 h = getUnderlyingType()->computeLocalHash();
    CombineHash(h, hash64('µ'));
    return h;
}

} // namespace lang
} // namespace phantom
