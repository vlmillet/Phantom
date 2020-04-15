// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "QualifiedType.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
void* QualifiedType::cast(Type* a_pTargetType, void* a_pSrc) const
{
    return m_pUnderlyingType->cast(a_pTargetType, a_pSrc);
}

void* QualifiedType::upcast(Type* a_pTargetType, void* a_pSrc) const
{
    return m_pUnderlyingType->upcast(a_pTargetType, a_pSrc);
}

void* QualifiedType::downcast(Type* a_pTargetType, void* a_pSrc) const
{
    return m_pUnderlyingType->downcast(a_pTargetType, a_pSrc);
}

void QualifiedType::construct(void* a_pBuffer) const
{
    m_pUnderlyingType->construct(a_pBuffer);
}

void QualifiedType::destroy(void* a_pBuffer) const
{
    m_pUnderlyingType->destroy(a_pBuffer);
}

void* QualifiedType::newInstance() const
{
    return m_pUnderlyingType->newInstance();
}

void QualifiedType::deleteInstance(void* a_pInstance) const
{
    m_pUnderlyingType->deleteInstance(a_pInstance);
}

phantom::lang::Type::TypeRelation QualifiedType::getRelationWith(Type* a_pType) const
{
    return m_pUnderlyingType->getRelationWith(a_pType);
}

bool QualifiedType::equal(void const* a_pSrc0, void const* a_pSrc1) const
{
    return m_pUnderlyingType->equal(a_pSrc0, a_pSrc1);
}

void QualifiedType::valueFromString(StringView a_str, void* dest) const
{
    m_pUnderlyingType->valueFromString(a_str, dest);
}

void QualifiedType::valueToString(StringBuffer& a_Buf, const void* src) const
{
    return m_pUnderlyingType->valueToString(a_Buf, src);
}

void QualifiedType::copyAssign(void* a_pDest, void const* a_pSrc) const
{
    m_pUnderlyingType->copyAssign(a_pDest, a_pSrc);
}

void QualifiedType::moveAssign(void* a_pDest, void* a_pSrc) const
{
    m_pUnderlyingType->moveAssign(a_pDest, a_pSrc);
}

void QualifiedType::copyConstruct(void* a_pDest, void const* a_pSrc) const
{
    m_pUnderlyingType->copyConstruct(a_pDest, a_pSrc);
}

void QualifiedType::moveConstruct(void* a_pDest, void* a_pSrc) const
{
    m_pUnderlyingType->moveConstruct(a_pDest, a_pSrc);
}

bool QualifiedType::convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const
{
    return m_pUnderlyingType->convert(a_pDstType, a_pDst, a_pSrc);
}

void QualifiedType::fetchElements(LanguageElements& out, Class* a_pClass /*= nullptr */) const
{
    m_pUnderlyingType->fetchElements(out, a_pClass);
}

bool QualifiedType::hasCopyDisabled() const
{
    return m_pUnderlyingType->hasCopyDisabled();
}

bool QualifiedType::hasMoveDisabled() const
{
    return m_pUnderlyingType->hasMoveDisabled();
}

bool QualifiedType::isCopyable() const
{
    return m_pUnderlyingType->isCopyable();
}

bool QualifiedType::isCopyConstructible() const
{
    return m_pUnderlyingType->isCopyConstructible();
}

bool QualifiedType::isCopyAssignable() const
{
    return m_pUnderlyingType->isCopyAssignable();
}

bool QualifiedType::isTriviallyCopyAssignable() const
{
    return m_pUnderlyingType->isTriviallyCopyAssignable();
}

bool QualifiedType::isTriviallyCopyConstructible() const
{
    return m_pUnderlyingType->isTriviallyCopyConstructible();
}

bool QualifiedType::isMoveable() const
{
    return m_pUnderlyingType->isMoveable();
}

bool QualifiedType::isMoveConstructible() const
{
    return m_pUnderlyingType->isMoveConstructible();
}

bool QualifiedType::isMoveAssignable() const
{
    return m_pUnderlyingType->isMoveAssignable();
}

bool QualifiedType::isTriviallyMoveAssignable() const
{
    return m_pUnderlyingType->isTriviallyMoveAssignable();
}

bool QualifiedType::isTriviallyMoveConstructible() const
{
    return m_pUnderlyingType->isTriviallyMoveConstructible();
}

bool QualifiedType::isEquivalent(Type* a_pType) const
{
    return m_pUnderlyingType->isEquivalent(a_pType);
}

phantom::lang::Type* QualifiedType::unsignedType() const
{
    return replicate(m_pUnderlyingType->unsignedType());
}

phantom::lang::Type* QualifiedType::signedType() const
{
    return replicate(m_pUnderlyingType->signedType());
}

phantom::lang::Type* QualifiedType::longType() const
{
    return replicate(m_pUnderlyingType->longType());
}

bool QualifiedType::isSignedInteger() const
{
    return m_pUnderlyingType->isSignedInteger();
}

bool QualifiedType::isUnsignedInteger() const
{
    return m_pUnderlyingType->isUnsignedInteger();
}

phantom::lang::Type* QualifiedType::promote() const
{
    return replicate(m_pUnderlyingType->promote());
}

uint QualifiedType::getDataPointerLevel() const
{
    return m_pUnderlyingType->getDataPointerLevel();
}

} // namespace lang
} // namespace phantom
