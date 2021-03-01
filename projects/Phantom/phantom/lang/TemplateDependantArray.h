// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Array.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Template dependant array type. \brief  Template dependant template instance.
/// \code{.cpp}
///             ex : int[t_ConstantTemplateArgument]
/// \endcode
class PHANTOM_EXPORT_PHANTOM TemplateDependantArray : public Array
{
    PHANTOM_DECL_TYPE;

public:
    TemplateDependantArray(Type* a_pType, Expression* a_pSize, PlaceholderConstant* a_pDeductibleConstant);

    bool isA(Type*) const override
    {
        return true;
    }
    virtual TemplateDependantArray* asTemplateDependantArray() const
    {
        return (TemplateDependantArray*)this;
    }

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;

    bool isCopyable() const override
    {
        return true;
    }

    bool isSame(Symbol* a_pOther) const override;

    PlaceholderConstant* getDeductibleConstant() const
    {
        return m_pDeductibleConstant;
    }

    Expression* getItemCountExpression() const
    {
        return m_pItemCountExpression;
    }

protected:
    Expression*          m_pItemCountExpression;
    PlaceholderConstant* m_pDeductibleConstant;
};

} // namespace lang
} // namespace phantom
