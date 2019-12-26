// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/ExtendedType.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Base for const/volatile qualified type

class PHANTOM_EXPORT_PHANTOM QualifiedType : public ExtendedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(QualifiedType);

    friend class phantom::reflection::Namespace;
    friend class phantom::reflection::Type;

protected:
    QualifiedType(Type* a_pType, TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                  Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    Type* removeAllVolatile() const override;
    Type* removeAllQualifiers() const override;
    Type* removeQualifiers() const override;
    Type* removePointerOrArray() const override;
    Type* removeAddress() const override;
    Type* removeArray() const override;

    void* cast(Type* a_pTargetType, void* a_pSrc) const override;
    void* upcast(Type* a_pTargetType, void* a_pSrc) const override;
    void* downcast(Type* a_pTargetType, void* a_pSrc) const override;

    void construct(void* a_pBuffer) const override;
    void destroy(void* a_pBuffer) const override;

    void* newInstance() const override;
    void  deleteInstance(void* a_pInstance) const override;

    ERelation getRelationWith(Type* a_pType) const override;

    bool equal(void const* a_pSrc0, void const* a_pSrc1) const override;

    void valueFromString(StringView a_str, void* dest) const override;
    void valueToString(StringBuffer& a_Buf, const void* src) const override;
    void copyAssign(void* a_pDest, void const* a_pSrc) const override;
    void moveAssign(void* a_pDest, void* a_pSrc) const override;
    void copyConstruct(void* a_pDest, void const* a_pSrc) const override;
    void moveConstruct(void* a_pDest, void* a_pSrc) const override;
    bool convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const override;

    Type* removeEverything() const override;
    void  fetchElements(LanguageElements& out, Class* a_pClass = nullptr) const override;

    // Traits
    bool hasCopyDisabled() const override;
    bool hasMoveDisabled() const override;

    bool isCopyable() const override;
    bool isCopyConstructible() const override;
    bool isCopyAssignable() const override;
    bool isTriviallyCopyAssignable() const override;
    bool isTriviallyCopyConstructible() const override;

    bool isMoveable() const override;
    bool isMoveConstructible() const override;
    bool isMoveAssignable() const override;
    bool isTriviallyMoveAssignable() const override;
    bool isTriviallyMoveConstructible() const override;
    bool isEquivalent(Type* a_pType) const override;

    Type* unsignedType() const override;
    Type* signedType() const override;
    Type* longType() const override;
    bool  isSignedInteger() const override;
    bool  isUnsignedInteger() const override;

    Type* promote() const override;

    uint getDataPointerLevel() const override;
};

} // namespace reflection
} // namespace phantom
