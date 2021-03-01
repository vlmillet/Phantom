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
    return getUnderlyingType()->cast(a_pTargetType, a_pSrc);
}

void* QualifiedType::upcast(Type* a_pTargetType, void* a_pSrc) const
{
    return getUnderlyingType()->upcast(a_pTargetType, a_pSrc);
}

void* QualifiedType::downcast(Type* a_pTargetType, void* a_pSrc) const
{
    return getUnderlyingType()->downcast(a_pTargetType, a_pSrc);
}

void QualifiedType::construct(void* a_pBuffer) const
{
    getUnderlyingType()->construct(a_pBuffer);
}

void QualifiedType::destroy(void* a_pBuffer) const
{
    getUnderlyingType()->destroy(a_pBuffer);
}

void* QualifiedType::newInstance() const
{
    return getUnderlyingType()->newInstance();
}

void QualifiedType::deleteInstance(void* a_pInstance) const
{
    getUnderlyingType()->deleteInstance(a_pInstance);
}

phantom::lang::Type::TypeRelation QualifiedType::getRelationWith(Type* a_pType) const
{
    return getUnderlyingType()->getRelationWith(a_pType);
}

bool QualifiedType::equal(void const* a_pSrc0, void const* a_pSrc1) const
{
    return getUnderlyingType()->equal(a_pSrc0, a_pSrc1);
}

void QualifiedType::valueFromString(StringView a_str, void* dest) const
{
    getUnderlyingType()->valueFromString(a_str, dest);
}

void QualifiedType::valueToString(StringBuffer& a_Buf, const void* src) const
{
    return getUnderlyingType()->valueToString(a_Buf, src);
}

void QualifiedType::copyAssign(void* a_pDest, void const* a_pSrc) const
{
    getUnderlyingType()->copyAssign(a_pDest, a_pSrc);
}

void QualifiedType::moveAssign(void* a_pDest, void* a_pSrc) const
{
    getUnderlyingType()->moveAssign(a_pDest, a_pSrc);
}

void QualifiedType::copyConstruct(void* a_pDest, void const* a_pSrc) const
{
    getUnderlyingType()->copyConstruct(a_pDest, a_pSrc);
}

void QualifiedType::moveConstruct(void* a_pDest, void* a_pSrc) const
{
    getUnderlyingType()->moveConstruct(a_pDest, a_pSrc);
}

bool QualifiedType::convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const
{
    return getUnderlyingType()->convert(a_pDstType, a_pDst, a_pSrc);
}

void QualifiedType::fetchElements(LanguageElements& out, Class* a_pClass /*= nullptr */) const
{
    getUnderlyingType()->fetchElements(out, a_pClass);
}

bool QualifiedType::hasCopyDisabled() const
{
    return getUnderlyingType()->hasCopyDisabled();
}

bool QualifiedType::hasMoveDisabled() const
{
    return getUnderlyingType()->hasMoveDisabled();
}

bool QualifiedType::isCopyable() const
{
    return getUnderlyingType()->isCopyable();
}

bool QualifiedType::isCopyConstructible() const
{
    return getUnderlyingType()->isCopyConstructible();
}

bool QualifiedType::isCopyAssignable() const
{
    return getUnderlyingType()->isCopyAssignable();
}

bool QualifiedType::isTriviallyCopyAssignable() const
{
    return getUnderlyingType()->isTriviallyCopyAssignable();
}

bool QualifiedType::isTriviallyCopyConstructible() const
{
    return getUnderlyingType()->isTriviallyCopyConstructible();
}

bool QualifiedType::isMoveable() const
{
    return getUnderlyingType()->isMoveable();
}

bool QualifiedType::isMoveConstructible() const
{
    return getUnderlyingType()->isMoveConstructible();
}

bool QualifiedType::isMoveAssignable() const
{
    return getUnderlyingType()->isMoveAssignable();
}

bool QualifiedType::isTriviallyMoveAssignable() const
{
    return getUnderlyingType()->isTriviallyMoveAssignable();
}

bool QualifiedType::isTriviallyMoveConstructible() const
{
    return getUnderlyingType()->isTriviallyMoveConstructible();
}

bool QualifiedType::isEquivalent(Type* a_pType) const
{
    return getUnderlyingType()->isEquivalent(a_pType);
}

phantom::lang::Type* QualifiedType::unsignedType() const
{
    return replicate(getUnderlyingType()->unsignedType());
}

phantom::lang::Type* QualifiedType::signedType() const
{
    return replicate(getUnderlyingType()->signedType());
}

phantom::lang::Type* QualifiedType::longType() const
{
    return replicate(getUnderlyingType()->longType());
}

bool QualifiedType::isSignedInteger() const
{
    return getUnderlyingType()->isSignedInteger();
}

bool QualifiedType::isUnsignedInteger() const
{
    return getUnderlyingType()->isUnsignedInteger();
}

phantom::lang::Type* QualifiedType::promote() const
{
    return replicate(getUnderlyingType()->promote());
}

uint QualifiedType::getDataPointerLevel() const
{
    return getUnderlyingType()->getDataPointerLevel();
}

} // namespace lang
} // namespace phantom
