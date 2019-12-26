// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Constant.h>
#include <phantom/reflection/Placeholder.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
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
    bool    accepts(LanguageElement* a_pLanguageElement) const override;
    Symbol* asSymbol() const override
    {
        return (PlaceholderConstant*)this;
    }
    Placeholder* asPlaceholder() const override
    {
        return (PlaceholderConstant*)this;
    }
    PlaceholderConstant* asPlaceholderConstant() const override
    {
        return (PlaceholderConstant*)this;
    }
    void getValue(void*) const override
    {
        PHANTOM_EXCEPTION_NO_IMPLEM();
    }
    bool hasValue(const void*) const override
    {
        PHANTOM_EXCEPTION_NO_IMPLEM();
        return false;
    }
    bool isZero() const override
    {
        PHANTOM_EXCEPTION_NO_IMPLEM();
        return false;
    }
    void toString(StringBuffer&) const override
    {
        PHANTOM_EXCEPTION_NO_IMPLEM();
    }
    void toLiteral(StringBuffer&) const override
    {
        PHANTOM_EXCEPTION_NO_IMPLEM();
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

    bool isSame(Symbol* a_pOther) const override;

    Placeholder* clone(uint a_Flags = 0) const override;

private:
    Constant* cloneImpl() const override;
};

} // namespace reflection
} // namespace phantom
