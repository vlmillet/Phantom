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
/// \brief  Represents a primitive type (fundamentals + internals)
class PHANTOM_EXPORT_PHANTOM PrimitiveType : public Type
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(PrimitiveType);

public:
    PHANTOM_DTOR ~PrimitiveType() override {}

    PrimitiveType* asPrimitiveType() const override { return const_cast<PrimitiveType*>(this); }
    Type*          asPOD() const override { return const_cast<PrimitiveType*>(this); }

    bool isCopyable() const override { return true; }

    bool isSignedInteger() const override { return signedType() == const_cast<PrimitiveType*>(this); }

    bool isUnsignedInteger() const override { return unsignedType() == const_cast<PrimitiveType*>(this); }

    void getUniqueName(StringBuffer& a_Buf) const override { return Symbol::getName(a_Buf); }

    void getQualifiedName(StringBuffer& a_Buf) const override { return Symbol::getName(a_Buf); }
    void getDecoratedName(StringBuffer& a_Buf) const override { return Symbol::getName(a_Buf); }
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { return Symbol::getName(a_Buf); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a constant object of this type.
    ///
    /// \param [in,out] a_pSrc      The value input buffer.
    /// \param  a_strName           (optional) the constant name.
    /// \param  a_pPrimitiveType    (optional) And alternative type for the constant (useful for
    /// enums).
    ///
    /// \return The new constant.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Constant* createConstant(LanguageElement* a_pOwner, void* a_pSrc, StringView a_strName = "",
                                     PrimitiveType* a_pPrimitiveType = nullptr) const;

    Constant* createConstant(void* a_pSrc, StringView a_strName = "", PrimitiveType* a_pPrimitiveType = nullptr) const;

protected:
    /// \internal
    PrimitiveType(TypeKind a_eTypeKind);
    /// \internal
    PrimitiveType(TypeKind a_eTypeKind, Type* a_pUnderlyingType, StringView a_strName, size_t a_uiSize,
                  size_t a_uiAlignment, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    PrimitiveType(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                  Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
};

} // namespace lang
} // namespace phantom
