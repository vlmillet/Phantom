// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ClassType.h>
#include <phantom/lang/Placeholder.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Placeholder class type.
class PHANTOM_EXPORT_PHANTOM PlaceholderClassType : public ClassType, public Placeholder
{
    PHANTOM_DECL_TYPE;

public:
    PlaceholderClassType(PlaceholderType* a_pPlaceholderType);
    PlaceholderClassType(StringView a_Name, Modifiers a_Modifiers, uint a_Flags);

    bool accepts(LanguageElement* a_pLanguageElement) const override
    {
        return a_pLanguageElement->asClassType() != nullptr;
    }
    Symbol*                       asSymbol() const override { return (PlaceholderClassType*)this; }
    Placeholder*                  asPlaceholder() const override { return (PlaceholderClassType*)this; }
    virtual PlaceholderClassType* asPlaceholderClassType() const { return (PlaceholderClassType*)this; }

    bool         isA(Type*) const override { return true; }
    virtual bool isA(Class*) const { return true; }

    Class* toClass() const;

    bool isCopyable() const override { return true; }

    Placeholder* clone(LanguageElement* a_pOwner, uint a_Flags = 0) const override;

    virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getQualifiedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeDecoratedName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }
    bool         isPack() const override { return m_isPack; }

private:
    bool m_isPack{};
};

} // namespace lang
} // namespace phantom
