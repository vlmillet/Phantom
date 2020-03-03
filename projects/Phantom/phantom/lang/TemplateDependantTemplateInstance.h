// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ClassType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Template dependant template instance.
/// \code{.cpp}
///             ex : MyClass<t_Ty>
/// \endcode
class PHANTOM_EXPORT_PHANTOM TemplateDependantTemplateInstance : public ClassType
{
    PHANTOM_DECL_TYPE;

public:
    TemplateDependantTemplateInstance(TemplateSpecialization* a_pTemplateSpecialization,
                                      const LanguageElements& a_Arguments, uint a_uiFlags = 0);
    TemplateDependantTemplateInstance(Template* a_pTemplate, const LanguageElements& a_Arguments, uint a_uiFlags = 0);

    bool isA(Type*) const override
    {
        return true;
    }

    Template* getTemplate() const
    {
        return m_pTemplate;
    }
    TemplateSpecialization* getTemplateSpecialization() const
    {
        return m_pTemplateSpecialization;
    }

    const LanguageElements& getArguments() const
    {
        return m_Arguments;
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

    bool isSame(Symbol* a_pOther) const override;

    bool isCopyable() const override
    {
        return true;
    }

private:
    Template*               m_pTemplate = nullptr;
    TemplateSpecialization* m_pTemplateSpecialization = nullptr;
    LanguageElements        m_Arguments;
};

} // namespace lang
} // namespace phantom
