// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base for const/volatile qualified type

class PHANTOM_EXPORT_PHANTOM QualifiedType : public Type
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(QualifiedType);

    friend class phantom::lang::Namespace;
    friend class phantom::lang::Type;

protected:
    QualifiedType(TypeKind a_eTypeKind, Type* a_pType, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                  Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    void* cast(Type* a_pTargetType, void* a_pSrc) const override;
    void* upcast(Type* a_pTargetType, void* a_pSrc) const override;
    void* downcast(Type* a_pTargetType, void* a_pSrc) const override;

    void construct(void* a_pBuffer) const override;
    void destroy(void* a_pBuffer) const override;

    void* newInstance() const override;
    void  deleteInstance(void* a_pInstance) const override;

    TypeRelation getRelationWith(Type* a_pType) const override;

    bool equal(void const* a_pSrc0, void const* a_pSrc1) const override;

    void valueFromString(StringView a_str, void* dest) const override;
    void valueToString(StringBuffer& a_Buf, const void* src) const override;
    void copyAssign(void* a_pDest, void const* a_pSrc) const override;
    void moveAssign(void* a_pDest, void* a_pSrc) const override;
    void copyConstruct(void* a_pDest, void const* a_pSrc) const override;
    void moveConstruct(void* a_pDest, void* a_pSrc) const override;
    bool convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const override;

    void fetchElements(LanguageElements& out, Class* a_pClass = nullptr) const override;

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

} // namespace lang
} // namespace phantom
