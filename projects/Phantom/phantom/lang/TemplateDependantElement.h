// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Symbol.h>
#include <phantom/utils/Optional.h>
#include <phantom/utils/OptionalArrayView.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Template dependant element.
/// \code{.cpp}
///             ex : MyClass<t_Ty>::templateDependantElement(0.5f)
/// \endcode
class PHANTOM_EXPORT_PHANTOM TemplateDependantElement : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(TemplateDependantElement);

public:
    TemplateDependantElement(LanguageElement* a_pLeft, StringView a_strName,
                             OptionalArrayView<LanguageElement*> a_pTemplateArguments = NullOpt,
                             OptionalArrayView<LanguageElement*> a_pFunctionArguments = NullOpt,
                             Modifiers modifiers = 0, uint a_uiFlags = 0);
    ~TemplateDependantElement() override;

    Type*            toType() const override;
    LanguageElement* getLeft() const
    {
        return m_pLeft;
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

    LanguageElements* getTemplateArguments() const
    {
        return m_pTemplateArguments;
    }
    LanguageElements* getFunctionArguments() const
    {
        return m_pFunctionArguments;
    }

protected:
    mutable Type*     m_pAsType = nullptr;
    LanguageElement*  m_pLeft = nullptr;
    LanguageElements* m_pTemplateArguments = nullptr;
    LanguageElements* m_pFunctionArguments = nullptr;
};

} // namespace reflection
} // namespace phantom
