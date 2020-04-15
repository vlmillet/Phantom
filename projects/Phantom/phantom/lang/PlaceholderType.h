// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Placeholder.h>
#include <phantom/lang/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Placeholder type.
/// \code{.cpp}
///             ex : template<typename t_PlaceholderType>
/// \endcode
class PHANTOM_EXPORT_PHANTOM PlaceholderType : public Type, public Placeholder
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(PlaceholderType);

public:
    PlaceholderType(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    bool accepts(LanguageElement* a_pLanguageElement) const override { return a_pLanguageElement->asType() != nullptr; }
    Symbol*          asSymbol() const override { return (PlaceholderType*)this; }
    Placeholder*     asPlaceholder() const override { return (PlaceholderType*)this; }
    Class*           toClass() const;
    ClassType*       toClassType() const;
    PlaceholderType* asPlaceholderType() const override { return (PlaceholderType*)this; }

    bool isA(Type*) const override { return true; }

    bool isCopyable() const override { return true; }

    Placeholder* clone(uint a_Flags = 0) const override;

    virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getQualifiedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeDecoratedName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }

protected:
    mutable Class*     m_pAsClass;
    mutable ClassType* m_pAsClassType;
};

} // namespace lang
} // namespace phantom
