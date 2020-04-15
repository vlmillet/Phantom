// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Class.h>
#include <phantom/lang/Placeholder.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Placeholder class.
/// \code{.cpp}
///             ex : int (t_Ty::*)
/// \endcode
class PHANTOM_EXPORT_PHANTOM PlaceholderClass : public Class, public Placeholder
{
    PHANTOM_DECL_TYPE;

public:
    PlaceholderClass(StringView a_Name, Modifiers a_Modifiers, uint a_Flags);
    PlaceholderClass(PlaceholderType* a_pPlaceholderType);

    bool accepts(LanguageElement* a_pLanguageElement) const override
    {
        return a_pLanguageElement->asClass() != nullptr;
    }
    Symbol*           asSymbol() const override { return (PlaceholderClass*)this; }
    Placeholder*      asPlaceholder() const override { return (PlaceholderClass*)this; }
    PlaceholderClass* asPlaceholderClass() const override { return (PlaceholderClass*)this; }

    bool         isA(Type*) const override { return true; }
    virtual bool isA(Class*) const { return true; }

    bool isCopyable() const override { return true; }

    Placeholder* clone(uint a_Flags = 0) const override;

    virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getQualifiedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeDecoratedName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }
};

} // namespace lang
} // namespace phantom
