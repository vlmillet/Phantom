// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once
// #pragma __PragmaPrintFile__

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a template signature parameter.
class PHANTOM_EXPORT_PHANTOM TemplateParameter : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(TemplateParameter);

    friend class TemplateSignature;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a template parameter with a given placeholder, optional default argument
    ///         and modifiers.
    ///
    /// \param  a_pPlaceholder      The placeholder associated with this template parameter.
    /// \param  a_pDefaultArgument  (optional) The default argument.
    /// \param  a_Modifiers         (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameter(Placeholder* a_pPlaceholder, LanguageElement* a_pDefaultArgument = nullptr,
                      Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    /// \brief  Destructor.
    ~TemplateParameter() override
    {
    }

    TemplateParameter* asTemplateParameter() const override
    {
        return (TemplateParameter*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of this template parameter in its templat signature.
    ///
    /// \return The index of this template parameter in its templat signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getIndex() const;

    TemplateParameter* clone() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template signature owning this template parameter.
    ///
    /// \return null if no template signature owns this template parameter, else the template
    /// signature owning this template parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateSignature* getTemplateSignature() const
    {
        return getOwner() ? getOwner()->asTemplateSignature() : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template using this template parameter.
    ///
    /// \return null if no template signature owns this template parameter, else the template using
    /// this template parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template* getTemplate() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument of this template parameter.
    ///
    /// \return null if no default argument, else the default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LanguageElement* getDefaultArgument() const
    {
        return m_pDefaultArgument;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument of this templat parameter.
    ///
    /// \param a_pDefaultArgument The default argument.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultArgument(LanguageElement* a_pElement);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the placeholder associated with this template parameter.
    ///
    /// \return The placeholder associated with this template parameter.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Placeholder* getPlaceholder() const
    {
        return m_pPlaceholder;
    }

    /// \internal
    bool partialAccepts(LanguageElement* a_pLanguageElement, size_t& a_Score,
                        PlaceholderMap& a_Deductions) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query of this template parameter accepts the given argument.
    ///
    /// \param a_pArgument  The argument.
    ///
    /// \return true if it accepts the given argument, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsArgument(LanguageElement* a_pLanguageElement) const;

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

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;

    bool isSame(Symbol* a_pLanguageElement) const override
    {
        TemplateParameter* pTP = a_pLanguageElement->asTemplateParameter();
        return pTP &&     isSame(pTP);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this template parameter is considered equal to another.
    ///
    /// \param [in,out] a_pOther    The template parameter to compare to this one.
    ///
    /// \return true if the template parameters are considered equal, false if they are not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isSame(TemplateParameter* a_pOther) const;

    using Symbol::isSame;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this template parameter is a variadic template parameter pack.
    ///
    /// \return true if the template parameter is a variadic template parameter, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isPack() const
    {
        return m_bPack;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Helper which created a replicate of this parameter with its own placeholder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TemplateParameter* clone(uint a_Flags = 0) const;

protected:
    void onElementRemoved(LanguageElement* a_pElement) override;
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;

protected:
    Placeholder*     m_pPlaceholder{};
    LanguageElement* m_pDefaultArgument{};
    bool             m_bPack{};
};

} // namespace lang
} // namespace phantom
