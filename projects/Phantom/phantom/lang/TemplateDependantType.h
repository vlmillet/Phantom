// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
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
        : Type(TypeKind::TemplateDependant, "", 0, 0, 0,
               PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS),
          m_pElement(nullptr)
    {
    }
    TemplateDependantType(TemplateDependantElement* a_pElement);

    bool isA(Type*) const override
    {
        return true;
    }

    virtual TemplateDependantType* asTemplateDependantType() const
    {
        return (TemplateDependantType*)this;
    }

    TemplateDependantElement* getTemplateDependantElement() const
    {
        return m_pElement;
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

private:
    TemplateDependantElement* m_pElement;
};

} // namespace reflection
} // namespace phantom
