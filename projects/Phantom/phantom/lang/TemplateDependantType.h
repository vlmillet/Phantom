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
/// \brief  Template dependant type.
/// \code{.cpp}
///             ex : typename std::unsigned_type<t_Ty>::type
/// \endcode
class PHANTOM_EXPORT_PHANTOM TemplateDependantType : public Type
{
    PHANTOM_DECL_TYPE;

public:
    TemplateDependantType()
        : Type(TypeKind::Unknown, "", 0, 0, 0, PHANTOM_R_FLAG_TEMPLATE_DEPENDANT), m_pElement(nullptr)
    {
    }
    TemplateDependantType(TemplateDependantElement* a_pElement);

    bool isA(Type*) const override { return true; }

    virtual TemplateDependantType* asTemplateDependantType() const { return (TemplateDependantType*)this; }

    TemplateDependantElement* getTemplateDependantElement() const { return m_pElement; }

    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;

private:
    TemplateDependantElement* m_pElement;
};

} // namespace lang
} // namespace phantom
