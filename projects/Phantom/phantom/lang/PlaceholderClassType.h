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
    Symbol* asSymbol() const override
    {
        return (PlaceholderClassType*)this;
    }
    Placeholder* asPlaceholder() const override
    {
        return (PlaceholderClassType*)this;
    }
    virtual PlaceholderClassType* asPlaceholderClassType() const
    {
        return (PlaceholderClassType*)this;
    }

    bool isA(Type*) const override
    {
        return true;
    }
    virtual bool isA(Class*) const
    {
        return true;
    }

    Class* asClass() const override
    {
        return getOwner()->asClass();
    }

    bool isSame(Symbol* a_pOther) const override
    {
        return Type::isSame(a_pOther) ||(a_pOther->asType() && a_pOther->asPlaceholder());
    }

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        return getName(a_Buf);
    }
    void getDecoratedName(StringBuffer& a_Buf) const override
    {
        return getName(a_Buf);
    }
    void getQualifiedName(StringBuffer& a_Buf) const override
    {
        return getName(a_Buf);
    }
    void getUniqueName(StringBuffer& a_Buf) const override
    {
        a_Buf += "@ph ";
        getName(a_Buf);
    }
    bool isCopyable() const override
    {
        return true;
    }

    Placeholder* clone(uint a_Flags = 0) const override;
};

} // namespace lang
} // namespace phantom
