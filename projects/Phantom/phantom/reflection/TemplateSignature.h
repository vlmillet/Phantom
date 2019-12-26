// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/SmallMap.h>
#include <phantom/reflection/LanguageElement.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
class PHANTOM_EXPORT_PHANTOM TemplateSignature : public LanguageElement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(TemplateSignature);

public:
    static TemplateSignature* Create(uint a_Flags = PHANTOM_R_NONE);

    /// \brief  Default constructor, creates an empty template signature.
    TemplateSignature(uint flags = PHANTOM_R_NONE);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a template signature with a given template parameters list.
    ///
    /// \param  a_Parameters    The templat parameters list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSignature(const TemplateParameters& a_Parameters, bool a_bVariadic = false, uint flags = PHANTOM_R_NONE);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a template signature by parsing template parameter types and template
    /// parameter names in two distinct ',' separated lists.
    ///
    /// \param  a_strTemplateParameterTypes     The template parameter types.
    /// \param  a_strTemplateParameterParams    The template parameter names.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static TemplateSignature* Parse(StringView a_strTemplateParameterTypes, StringView a_strTemplateParameterNames,
                                    LanguageElement* a_pContextScope, uint a_uiFlags = PHANTOM_R_NONE);

    PHANTOM_DTOR ~TemplateSignature() override;

    TemplateSignature* asTemplateSignature() const override
    {
        return const_cast<TemplateSignature*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template owning this signature.
    ///
    /// \return The template owning this signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template* getTemplate() const
    {
        return getOwner() ? getOwner()->asTemplate() : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template value parameter (ex : bool t_Bool, int t_Int ...).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    TemplateParameter* addTemplateValueParameter(StringView a_Name)
    {
        return addTemplateValueParameter(phantom::reflection::TypeOf<T>::object(), a_Name);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template value parameter (ex : bool t_Bool, int t_Int ...).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameter* addTemplateValueParameter(Type* a_pType, StringView a_Name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template type parameter (ex : typename, class ...).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameter* addTemplateTypeParameter(StringView a_Name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template template parameter (ex : template<&lt;class, int&gt; class ...).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameter* addTemplateTemplateParameter(TemplateSignature* a_pTemplateSignature, StringView a_Name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of the parameter matching the given name.
    ///
    /// \param  a_strName   The paramater's name.
    ///
    /// \return The parameter's index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getTemplateParameterIndex(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of the parameter matching the given placeholder.
    ///
    /// \param  a_pPlaceholder   The paramater's placeholder.
    ///
    /// \return The parameter's index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getTemplateParameterIndex(Placeholder* a_pPlacelholder) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of the given parameter.
    ///
    /// \param  a_pParameter The parameter.
    ///
    /// \return The parameter's index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getTemplateParameterIndex(TemplateParameter* a_pParameter) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument of the parameter matching the given name.
    ///
    /// \param  a_strParameterName  The template parameter name.
    /// \param  a_pDefaultArgument  The default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultArgument(StringView a_strParameterName, LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument of the parameter at given index position.
    ///
    /// \param  a_uiIndex           The template parameter index.
    /// \param  a_pDefaultArgument  The default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultArgument(size_t a_uiIndex, LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument of the parameter matching the given name.
    ///
    /// \param  a_strParameterName  The parameter's name.
    ///
    /// \return null if no parameter found matching the given name, else the default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getDefaultArgument(StringView a_strParameterName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument of the parameter at given index position.
    ///
    /// \param  i   The parameter's index.
    ///
    /// \return null if the parameter does not have a default argument, else the default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getDefaultArgument(size_t i) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument count.
    ///
    /// \return The default argument count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getDefaultArgumentCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a template parameter to this template signature.
    ///
    /// \param [in,out] a_pTemplateParameter   If non-null, the template signature parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplateParameter(TemplateParameter* a_pTemplateParameter);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an alias name to the parameter at given index position.
    ///
    /// \param  a_uiIndex   The index.
    /// \param  a_strAlias  The alias.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addTemplateParameterAliasName(size_t a_uiIndex, StringView a_strAlias);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the placeholders.
    ///
    /// \return The placeholders.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const LanguageElements& getPlaceholders() const
    {
        return m_Placeholders;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template parameters list.
    ///
    /// \return The template parameters list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const TemplateParameters& getTemplateParameters() const
    {
        return m_TemplateParameters;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter matching the given name, if any.
    ///
    /// \param  a_strName   The parameter's name.
    ///
    /// \return null if there is no paramter matching the given name, else the parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameter* getTemplateParameter(StringView a_strName) const
    {
        size_t i = getTemplateParameterIndex(a_strName);
        return (i == ~size_t(0)) ? nullptr : m_TemplateParameters[i];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query of this template signature accepts the given arguments.
    ///
    /// \param  a_Arguments The arguments.
    ///
    /// \return true if it accepts the given arguments, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsArguments(const LanguageElements& a_Arguments) const;

    virtual bool isSame(Symbol* a_pLanguageElement) const;

    using LanguageElement::isSame;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this template signature is considered equal to another.
    ///
    /// \param a_pOther    If non-null, the template signature* to compare to this template
    /// signature.
    ///
    /// \return true if the template signatures are considered equal, false if they are not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isSame(TemplateSignature* a_pOther) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query is this template signature is variadic (the last template parameter is a
    /// pack).
    ///
    /// \return true if variadic, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isVariadic() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets this template signature as variadic (the last template parameter becomes a
    /// pack).
    ///
    /// \param a_bVariadic true if variadic, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setVariadic(bool a_bValue);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Helper which created a replicate of this signature with its own parameters.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSignature* clone(uint a_Flags = 0) const;

private:
    TemplateParameters       m_TemplateParameters;
    LanguageElements         m_Placeholders;
    SmallMap<String, size_t> m_TemplateParameterAliasNames;
};

} // namespace reflection
} // namespace phantom
