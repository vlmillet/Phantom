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
/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Represents an array type (ex: int[5]).
/////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM ExtendedType : public Type
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ExtendedType);

public:
    friend class Type;

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ExtendedType(TypeKind a_eTypeKind, Type* a_pUnderlyingType, StringView a_strSuffix, size_t a_Size,
                 size_t a_Alignment, Modifiers a_Mods, uint a_uiFlags)
        : Type(a_eTypeKind, a_pUnderlyingType, a_strSuffix, a_Size, a_Alignment, a_Mods, a_uiFlags)
    {
    }
    LanguageElement* getNamingScope() const override { return getUnderlyingType()->getNamingScope(); }
    void             getQualifiedName(StringBuffer& a_Buf) const override
    {
        getUnderlyingType()->getQualifiedName(a_Buf);
        getName(a_Buf);
    }
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        getUnderlyingType()->getQualifiedDecoratedName(a_Buf);
        getName(a_Buf);
    }
    void getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override
    {
        getUnderlyingType()->getRelativeName(a_pTo, a_Buf);
        getName(a_Buf);
    }
    void getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override
    {
        getUnderlyingType()->getRelativeDecoratedName(a_pTo, a_Buf);
        getName(a_Buf);
    }
    void getDecoratedName(StringBuffer& a_Buf) const override
    {
        getUnderlyingType()->getDecoratedName(a_Buf);
        getName(a_Buf);
    }
    void getUniqueName(StringBuffer& a_Buf) const override
    {
        getUnderlyingType()->getUniqueName(a_Buf);
        getName(a_Buf);
    }
};

} // namespace lang
} // namespace phantom
