// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "LValueReference.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
LValueReference::LValueReference(Type* a_pReferencedType)
    : Reference(TypeKind::LValueReference, a_pReferencedType, "&")
{
}

bool LValueReference::partialAccepts(Type* a_pType, size_t& a_Score,
                                     PlaceholderMap& a_DeducedConstants) const
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
    return a_pOther ==
    this OR(a_pOther->asLValueReference() AND m_pUnderlyingType->isSame(
    static_cast<LValueReference*>(a_pOther)->m_pUnderlyingType));
}

void LValueReference::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    a_Buf += '&';
}

void LValueReference::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    a_Buf += '&';
}

void LValueReference::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    a_Buf += '&';
}

void LValueReference::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    a_Buf += '&';
}

} // namespace reflection
} // namespace phantom
