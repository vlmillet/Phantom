// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a global/static variable.
class PHANTOM_EXPORT_PHANTOM Variable : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Variable);

    friend class AnonymousSection;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a variable with given value type, name and optional modifiers.
    ///
    /// \param  a_pValueType    The value type.
    /// \param  a_strName       The name.
    /// \param  a_Modifiers     (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variable(Type* a_pValueType, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an anonymous variable with given allocated memory address, value type,
    /// optionals range and modifiers.
    ///
    /// \param  a_pAddress      The allocated memory address.
    /// \param  a_pValueType    The value type.
    /// \param  a_pRange        (optional) the range.
    /// \param  a_Modifiers     (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variable(Type* a_pValueType, void* a_pAddress, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a variable with given allocated memory address, value type, name,
    ///         optionals range and modifiers.
    ///
    /// \param  a_pAddress      The allocated memory address.
    /// \param  a_pValueType    The value type.
    /// \param  a_strName       The name.
    /// \param  a_pRange        (optional) the range.
    /// \param  a_Modifiers     (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Variable(Type* a_pValueType, StringView a_strName, void* a_pAddress, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    PHANTOM_DTOR ~Variable() override;

    void initialize();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the address of this variable.
    ///
    /// \return The address of this variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getAddress() const { return m_pAddress; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value type of this variable.
    ///
    /// \return The value type of this variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getValueType() const { return m_pValueType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the value of this variable.
    ///
    /// \param  a_pInputBuffer  The input buffer holding the source value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setValue(void const* a_pInputBuffer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value of this variable.
    ///
    /// \param [in,out] a_pOutputBuffer The output buffer receiving the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getValue(void* a_pOutputBuffer) const;

    Variable* asVariable() const override { return (Variable*)this; }

    StaticField* asStaticField() const override;

    void setValueType(Type* a_pType);

    void setInitializationExpression(Expression* a_pExpression);

    Expression* getInitializationExpression() const { return m_pInitializationExpression; }

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the address of this variable.
    ///
    /// \param a_pAddress  The address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setAddress(void* a_pAddress) { m_pAddress = a_pAddress; }

protected:
    virtual void finalize();

protected:
    Type*             m_pValueType;
    void*             m_pAddress;
    AnonymousSection* m_pAnonymousSection;
    Expression*       m_pInitializationExpression;
};

} // namespace lang
} // namespace phantom
