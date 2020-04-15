// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Constant.h>
#include <phantom/lang/Placeholder.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Placeholder constant.
/// \code{.cpp}
///             ex : template<int t_PlaceholderConstant>
/// \endcode
class PHANTOM_EXPORT_PHANTOM PlaceholderConstant : public Constant, public Placeholder
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(PlaceholderConstant);

public:
    PlaceholderConstant(Type* a_pType, StringView a_strName, Modifiers modifiers = 0, uint a_uiFlags = 0)
        : Constant(a_pType, a_strName, modifiers, a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT)
    {
    }
    bool                 accepts(LanguageElement* a_pLanguageElement) const override;
    Symbol*              asSymbol() const override { return (PlaceholderConstant*)this; }
    Placeholder*         asPlaceholder() const override { return (PlaceholderConstant*)this; }
    PlaceholderConstant* asPlaceholderConstant() const override { return (PlaceholderConstant*)this; }
    void                 getValue(void*) const override { PHANTOM_ASSERT_NO_IMPL(); }
    bool                 hasValue(const void*) const override
    {
        PHANTOM_ASSERT_NO_IMPL();
        return false;
    }
    bool isZero() const override
    {
        PHANTOM_ASSERT_NO_IMPL();
        return false;
    }
    void        toString(StringBuffer&) const override { PHANTOM_ASSERT_NO_IMPL(); }
    void        toLiteral(StringBuffer&) const override { PHANTOM_ASSERT_NO_IMPL(); }
    void const* getAddress() const override
    {
        PHANTOM_ASSERT_NO_IMPL();
        return nullptr;
    }

    Placeholder* clone(uint a_Flags = 0) const override;

    virtual void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getDecoratedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getQualifiedName(StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeDecoratedName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }
    virtual void getRelativeName(LanguageElement*, StringBuffer& a_Buf) const override { getName(a_Buf); }

private:
    Constant* cloneImpl() const override;
};

} // namespace lang
} // namespace phantom
