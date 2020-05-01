// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
#include <phantom/utils/SmallMap.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM TemplateSpecialization : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(TemplateSpecialization);

    friend class LanguageElement;
    friend class Template;
    friend class Scope;

protected:
    static TemplateSpecialization* Create(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                          const LanguageElements& a_Arguments, Symbol* a_pTemplated, uint a_Flags);

    TemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pSignature, const LanguageElements& arguments,
                           uint a_Flags);
    TemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pSignature, const LanguageElements& arguments,
                           Symbol* a_pTemplated, uint a_Flags);
    TemplateSpecialization(TemplateSpecialization* a_pInstantiationSpecialization, const LanguageElements& arguments,
                           const PlaceholderMap& a_PlaceholderSubstitutions);

public:
    PHANTOM_DTOR ~TemplateSpecialization() override;

    TemplateSpecialization* asTemplateSpecialization() const override { return (TemplateSpecialization*)this; }

    virtual TemplateSpecialization* getEnclosingTemplateSpecialization() const
    {
        return const_cast<TemplateSpecialization*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the naming scope of this template specialization which is in fact the naming
    /// scope of the related template.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getNamingScope() const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the argument count of this template specialization.
    ///
    /// \return The argument count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getArgumentCount() const { return m_Arguments.size(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the argument matching the given template parameter name, if it is type.
    ///
    /// \param  a_strParameterName  The parameter's name.
    ///
    /// \return null if no parameter maches the given name or if argument is not a type, else the
    /// argument as a type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getArgumentAsType(StringView a_strParameterName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the argument matching the given template parameter name.
    ///
    /// \param  a_strParameterName  The parameter's name.
    ///
    /// \return null if no parameter maches the given name, else the argument as a type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getArgument(StringView a_strParameterName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the argument at given index position.
    ///
    /// \param  i   The argument's index.
    ///
    /// \return The argument at given index position.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getArgument(size_t i) const { return m_Arguments[i]; }
    size_t           getArgumentIndex(StringView a_strParameterName) const;
    LanguageElement* getDefaultArgument(size_t i) const
    {
        return m_pDefaultArguments ? (*m_pDefaultArguments)[i] : nullptr;
    }
    LanguageElement*        getDefaultArgument(StringView a_strParameterName) const;
    const LanguageElements& getArguments() const { return m_Arguments; }

    LanguageElements::const_iterator beginArguments() const { return m_Arguments.begin(); }
    LanguageElements::const_iterator endArguments() const { return m_Arguments.end(); }

    void setArgument(size_t a_uiIndex, LanguageElement* a_pElement);
    void setDefaultArgument(size_t i, LanguageElement* a_pElement);
    void setDefaultArgument(StringView a_strParameterName, LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template parameters.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameters const& getTemplateParameters() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query is this template specialization is variadic.
    ///
    /// \return true if variadic, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isVariadic() const;

    void   getDecoration(StringBuffer& a_Buf) const;
    hash64 getDecorationHash() const;
    void   getRelativeDecoration(LanguageElement* a_pTo, StringBuffer& a_Buf) const;
    void   getQualifiedDecoration(StringBuffer& a_Buf) const;
    void   getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void   getQualifiedName(StringBuffer& a_Buf) const override;
    void   getDecoratedName(StringBuffer& a_Buf) const override;
    void   getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void   getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;

    Template*          getTemplate() const { return m_pTemplate; }
    TemplateSignature* getTemplateSignature() const { return m_pTemplateSignature; }

    void checkCompleteness() const override;
    bool canBeDestroyed() const override;

    bool isEmpty() const;

    bool isPartial() const;

    bool isFull() const;

    bool isSpecializingParameter(TemplateParameter* a_pParameter) const;

    bool matches(LanguageElementsView a_Arguments) const;
    bool isSame(TemplateSpecialization* a_pTemplateSpecialization) const;
    using Symbol::isSame;

    using Symbol::partialAccepts;

    bool partialAccepts(const LanguageElements& a_Arguments, size_t& a_Score, PlaceholderMap& a_Deductions) const;

    void    setTemplated(Symbol* a_pTemplated);
    Symbol* getTemplated() const { return m_pTemplated; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets an extended version of this template specialization  (for example set by a scripting language to
    /// enrich a native template)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* clone(uint a_Flags = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets an extended version of this template specialization  (for example set by a scripting language to
    /// enrich a native template)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setExtendedSpecialization(TemplateSpecialization* a_pExtended);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the extended version of this template specialization
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getExtendedSpecialization() const { return m_pExtendedSpecialization; }

    TemplateSpecialization* getInstantiationSpecialization() const { return m_pInstantiationSpecialization; }

    PlaceholderMap const& getPlaceholderSubstitutions() const { return m_PlaceholderSubstitutions; }

    Source* getCodeLocationSource() const override;

private:
    void _updateName();

private:
    Template*               m_pTemplate{};
    TemplateSignature*      m_pTemplateSignature{};
    LanguageElements        m_Arguments;
    Symbol*                 m_pTemplated{};
    LanguageElements*       m_pDefaultArguments{};
    TemplateSpecialization* m_pExtendedSpecialization{};
    TemplateSpecialization* m_pInstantiationSpecialization{};
    PlaceholderMap          m_PlaceholderSubstitutions;
};

} // namespace lang
} // namespace phantom
