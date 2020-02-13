// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
    : QualifiedType(a_pType, a_pType->getTypeKind(), "const", 0, 0, a_pType->getModifiers() | PHANTOM_R_CONST,
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
    return a_pOther ==
    this OR(a_pOther->asConstType()
            AND m_pUnderlyingType->isSame(static_cast<ConstType*>(a_pOther)->m_pUnderlyingType));
}

Type* ConstType::addConst() const
{
    return const_cast<ConstType*>(this);
}

Type* ConstType::addVolatile() const
{
    return m_pUnderlyingType->makeConstVolatile();
}

Type* ConstType::addConstVolatile() const
{
    return m_pUnderlyingType->makeConstVolatile();
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
