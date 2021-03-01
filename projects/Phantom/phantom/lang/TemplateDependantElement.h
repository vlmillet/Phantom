// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
#include <phantom/utils/Optional.h>
#include <phantom/utils/OptionalArrayView.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
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

    void initialize();

    Type*            toType() const override;
    LanguageElement* getLeft() const { return m_pLeft; }

    void getRelativeDecoration(LanguageElement* a_pTo, StringBuffer& a_Buf) const;
    void getDecoration(StringBuffer& a_Buf) const;
    void getQualifiedDecoration(StringBuffer& a_Buf) const;

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;

    Optional<LanguageElements> const& getTemplateArguments() const { return m_pTemplateArguments; }
    Optional<LanguageElements> const& getFunctionArguments() const { return m_pFunctionArguments; }

protected:
    mutable Type*              m_pAsType = nullptr;
    LanguageElement*           m_pLeft = nullptr;
    Optional<LanguageElements> m_pTemplateArguments{};
    Optional<LanguageElements> m_pFunctionArguments{};
};

} // namespace lang
} // namespace phantom
