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
    : QualifiedType(TypeKind::Unknown, a_pType, "const", 0, 0, PHANTOM_R_CONST,
                    a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
    addReferencedElement(a_pType);
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

void ConstType::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    a_Buf += "@const";
}

void ConstType::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    a_Buf += " const";
}

void ConstType::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    a_Buf += " const";
}

void ConstType::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    a_Buf += " const";
}

} // namespace lang
} // namespace phantom
