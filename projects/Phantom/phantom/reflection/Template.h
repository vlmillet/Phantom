// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Symbol.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
struct TemplateRegistrer;

class PHANTOM_EXPORT_PHANTOM Template : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Template);

    friend class Namespace;
    friend class TemplateSpecialization;
    friend struct TemplateRegistrer;

public:
    static Template* Parse(StringView a_strTemplateTypes, StringView a_strTemplateParam,
                           StringView a_strName, LanguageElement* a_pContextScope,
                           Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an anonymous empty template.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a template with a default empty specialization (C++ like 'primary'
    /// template).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template(TemplateSignature* a_pSignature, StringView a_strName, Modifiers a_Modifiers = 0,
             uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an empty template (D language like template group).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    PHANTOM_DTOR ~Template() override;

    Scope* getScope() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template specializations of this template.
    ///
    /// \return the template specialization list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecializations const& getTemplateSpecializations() const
    {
        return m_TemplateSpecializations;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template parameters.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameters const& getTemplateParameters() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of the template parameter matching the given name in the signature .
    ///
    /// \param  a_strName   The template parameter name.
    ///
    /// \return The template parameter index or -1 if no parameter matches the given name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getTemplateParameterIndex(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of the given template parameter in the signature .
    ///
    /// \param a_pTemplateParameter    The template parameter.
    ///
    /// \return The template parameter index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getTemplateParameterIndex(TemplateParameter* a_pTemplateParameter) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument of the template parameter matching the given name.
    ///
    /// \param  a_strParameterName  The template parameter name.
    /// \param  a_pDefaultArgument The default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultArgument(StringView a_strParameterName, LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument of the template parameter at given index position.
    ///
    /// \param  a_uiIndex  The template parameter index.
    /// \param  a_pDefaultArgument The default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultArgument(size_t a_uiIndex, LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument of the template parameter matching the given name.
    ///
    /// \param  a_strParameterName  The template parameter name..
    ///
    /// \return The default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getDefaultArgument(StringView a_strParameterName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument of the template parameter at given index position.
    ///
    /// \param  i   The template parameter index.
    ///
    /// \return The default argument of the template parameter at given index position.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getDefaultArgument(size_t i) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument count.
    ///
    /// \return The default argument count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getDefaultArgumentCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template parameter to the template signature.
    ///
    /// \param [in,out] a_pTemplateParameter    The template parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplateParameter(TemplateParameter* a_pTemplateParameter);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'primary' template signature.
    ///
    /// \return The 'primary' template signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSignature* getTemplateSignature() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a specialization of this template with given template arguments, optional
    ///         body and signature.
    ///
    /// \param  a_Arguments               The template arguments.
    /// \param  a_pBody                 (optional) The body.
    /// \param  a_pTemplateSignature    (optional) The template signature.
    ///
    /// \return The PHANTOM_NEW(template) specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* createTemplateSpecialization(const LanguageElements& a_Arguments,
                                                         ClassType*              a_pTemplated = nullptr,
                                                         TemplateSignature*      a_pTemplateSignature = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a specialization of this template with a signe template argument, optional
    ///         body and signature.
    ///
    /// \param  a_pArgument             The template argument.
    /// \param  a_pBody                 (optional) The body.
    /// \param  a_pTemplateSignature    (optional) The template signature.
    ///
    /// \return The PHANTOM_NEW(template) specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization*
    createTemplateSpecialization(LanguageElement* a_pArgument, ClassType* a_pBody = nullptr,
                                 TemplateSignature* a_pTemplateSignature = nullptr)
    {
        LanguageElements args{a_pArgument};
        return createTemplateSpecialization(args, a_pBody, a_pTemplateSignature);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a name alias to the template parameter at given position index.
    ///
    /// \param  a_uiIndex   The index.
    /// \param  a_strAlias  The alias name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplateParameterAliasName(size_t a_uiIndex, StringView a_strAlias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template specialization matching the given template arguments.
    ///
    /// \param  a_Arguments The arguments.
    ///
    /// \return null if no specialization found, else the specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getTemplateSpecialization(LanguageElementsView a_Arguments) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template instantiation matching the given template arguments. The
    /// difference
    ///         with getTemplateSpecialization is that it can accept less arguments than parameters
    ///         if there is default arguments in the template signature.
    ///
    /// \param  a_Arguments The arguments.
    ///
    /// \return null if no specialization found, else the specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getTemplateInstantiation(LanguageElementsView a_Arguments) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets template specialization matching the given placeholder/template argument map.
    ///
    /// \param  arguments   The arguments.
    ///
    /// \return null if no specialization found, else the specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getTemplateSpecialization(const PlaceholderMap& arguments) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a template specialization matching the given one.
    ///
    /// \param [in,out] a_pTemplateSpecialization   The template specialization to test.
    ///
    /// \return null if no template specialization found that match the given one, else the template
    /// specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization*
    getTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the empty template specialization (template as is, every argument is a
    /// placeholder).
    ///
    /// \return The empty template specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSpecialization* getEmptyTemplateSpecialization() const
    {
        return m_TemplateSpecializations.size() ? m_TemplateSpecializations[0] : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this template accepts the given template arguments.
    ///
    /// \param  a_Arguments The arguments to test.
    ///
    /// \return true if it succeeds, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsArguments(const LanguageElements& a_Arguments) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Map arguments to placeholders.
    ///
    /// \param  a_Arguments     The arguments.
    /// \param [in,out] a_Out   [in,out] The result map.
    ///
    /// \return true if it succeeds to map every argument with a placeholder, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool mapArguments(const LanguageElements& a_Arguments, PlaceholderMap& a_Out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query is this template is variadic.
    ///
    /// \return true if variadic, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isVariadic() const;

    Template* asTemplate() const override
    {
        return const_cast<Template*>(this);
    }

    void setNativeDefaultArgumentStrings(const char**     a_ppArgs,
                                         LanguageElement* a_pScope /*= nullptr*/);

protected: // native constructor
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template specialization to this template.
    ///
    /// \param [in,out] a_pSpecialization   If non-null, the specialization.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization);

protected:
    bool canBeDestroyed() const override
    {
        return m_TemplateSpecializations.empty() AND Symbol::canBeDestroyed();
    }
    void onAncestorChanged(LanguageElement* a_pOwner) override;
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;

private:
    TemplateSpecialization*
         createEmptyTemplateSpecialization(TemplateSignature* a_pTemplateSignature, Symbol* a_pBody);
    void createEmptyTemplateSpecialization(TemplateSignature* a_pTemplateSignature);

protected:
    TemplateSpecializations m_TemplateSpecializations;
};

} // namespace reflection
} // namespace phantom
