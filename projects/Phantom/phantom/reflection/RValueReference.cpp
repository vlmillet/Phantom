// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "RValueReference.h"

#include "LValueReference.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
RValueReference::RValueReference(Type* a_pReferencedType)
    : Reference(TypeKind::RValueReference, a_pReferencedType, "&&")
{
}

Type* RValueReference::addLValueReference() const
{
    return m_pUnderlyingType->makeLValueReference();
}

Type* RValueReference::addRValueReference() const
{
    return const_cast<RValueReference*>(this);
}

Type* RValueReference::addPointer() const
{
    return m_pUnderlyingType->addPointer();
}

Type* RValueReference::removeAllConst() const
{
    return m_pUnderlyingType->removeAllConst()->makeRValueReference();
}

Type* RValueReference::removeAllQualifiers() const
{
    return m_pUnderlyingType->removeAllQualifiers()->makeRValueReference();
}

Type* RValueReference::replicate(Type* a_pInput) const
{
    return m_pUnderlyingType->replicate(a_pInput->removeReference())->addRValueReference();
}

bool RValueReference::partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const
{
    if (a_pType->asRValueReference())
    {
        a_Score += 10;
        return m_pUnderlyingType
        ? m_pUnderlyingType->partialAccepts(a_pType->removeRValueReference(), a_Score, a_DeducedConstants)
        : false;
    }
    return false;
}

bool RValueReference::isSame(Symbol* a_pOther) const
{
    return (a_pOther == this) OR(a_pOther->asRValueReference() AND m_pUnderlyingType->isSame(
    static_cast<RValueReference*>(a_pOther)->m_pUnderlyingType));
}

void RValueReference::getUniqueName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getUniqueName(a_Buf);
    a_Buf += '&';
    a_Buf += '&';
}

void RValueReference::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedName(a_Buf);
    a_Buf += '&';
    a_Buf += '&';
}

void RValueReference::getDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getDecoratedName(a_Buf);
    a_Buf += '&';
    a_Buf += '&';
}

void RValueReference::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    m_pUnderlyingType->getQualifiedDecoratedName(a_Buf);
    a_Buf += '&';
    a_Buf += '&';
}

} // namespace reflection
} // namespace phantom
