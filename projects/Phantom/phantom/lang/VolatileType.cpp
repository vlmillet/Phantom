// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
    : QualifiedType(a_pType, a_pType->getTypeKind(), "volatile", 0, 0, a_pType->getModifiers() | PHANTOM_R_VOLATILE,
                    a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
    addReferencedElement(a_pType);
}

VolatileType::~VolatileType() {}

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
    return a_pOther ==
    this OR(a_pOther->asVolatileType()
            AND m_pUnderlyingType->isSame(static_cast<VolatileType*>(a_pOther)->m_pUnderlyingType));
}

Type* VolatileType::addConst() const
{
    return m_pUnderlyingType->makeConstVolatile();
}

Type* VolatileType::addVolatile() const
{
    return const_cast<VolatileType*>(this);
}

Type* VolatileType::addConstVolatile() const
{
    return m_pUnderlyingType->makeConstVolatile();
}

Type* VolatileType::replicate(Type* a_pInput) const
{
    return m_pUnderlyingType->replicate(a_pInput)->addVolatile();
}

void VolatileType::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    a_Buf += "@volatile";
}

void VolatileType::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    a_Buf += " volatile";
}

void VolatileType::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    a_Buf += " volatile";
}

void VolatileType::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    a_Buf += " volatile";
}

} // namespace lang
} // namespace phantom
