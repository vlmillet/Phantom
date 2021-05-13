// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once
// #pragma __PragmaPrintFile__

/* ****************** Includes ******************* */
#include <phantom/lang/LocalVariable.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class Block;

/// \brief  Represents a function parameter.
class PHANTOM_EXPORT_PHANTOM Parameter : public LocalVariable
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    friend class Signature;
    friend struct TypeBuilderBase;
    friend struct MemberBuilder;
    friend struct NamespaceBuilder;

    PHANTOM_DECLARE_META_CLASS(Parameter);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a parameter with given value type and optionals name, default value
    ///         expression and modifiers.
    ///
    /// \param [in,out] a_pType             The parameter's type.
    /// \param  a_strName                   (optional) The parameter's name.
    /// \param  a_pDefaultValueExpression   (optional) The default value expression.
    /// \param  a_Modifiers                 (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parameter(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    Parameter(Type* a_pValueType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    Parameter(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    /// \brief  Destructor.
    ~Parameter() override {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the definition name of this parameter (by opposition to declaration name).
    ///
    /// \param  a_strName   The name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefinitionName(StringView a_strName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets if this parameter has a default argument defined (either as a native string, or
    /// an expression).
    ///
    /// \return The default argument expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDefaultArgument() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default argument expression of this parameter.
    ///
    /// \return The default argument expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getDefaultArgumentExpression() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default argument expression of this parameter.
    ///
    /// \param  a_pDefaultArgumentExpression The default argument expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultArgumentExpression(Expression* a_pDefaultArgumentExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the native default argument string
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getNativeDefaultArgumentString() const { return m_NativeDefaultArgumentStr; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Is this parameter a '...' parameter ellipsis in a variadic function/method.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isEllipsis() const;

    // reimplemented

    Parameter* asParameter() const override { return (Parameter*)this; }

    Parameter* cloneImpl(LanguageElement* a_pOwner) const override;

    StringView getNativeName() const { return m_NativeName; }

private:
    void setNativeDefaultArgumentStr(StringView a_DefaultArgument);
    void setNativeName(StringView a_Name);

protected:
    mutable Expression* m_pDefaultArgumentExpression = nullptr;

private:
    mutable StringView m_NativeName;
    mutable StringView m_NativeDefaultArgumentStr;
};

} // namespace lang
} // namespace phantom
