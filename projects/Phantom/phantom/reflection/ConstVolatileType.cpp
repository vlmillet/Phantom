// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "ConstVolatileType.h"

#include <phantom/utils/Delegate.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
ConstVolatileType::ConstVolatileType(Type* a_pType)
    : QualifiedType(a_pType, a_pType->getTypeKind(), "const volatile", 0, 0,
                    a_pType->getModifiers() | PHANTOM_R_VOLATILE | PHANTOM_R_CONST,
                    a_pType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
{
    addReferencedElement(a_pType);
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
    return a_pOther ==
    this OR(a_pOther->asConstVolatileType()
            AND m_pUnderlyingType->isSame(static_cast<ConstVolatileType*>(a_pOther)->m_pUnderlyingType));
}

void ConstVolatileType::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    a_Buf += "@const@volatile";
}

void ConstVolatileType::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    a_Buf += " const volatile";
}

void ConstVolatileType::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    a_Buf += " const volatile";
}

void ConstVolatileType::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    a_Buf += " const volatile";
}

} // namespace reflection
} // namespace phantom
