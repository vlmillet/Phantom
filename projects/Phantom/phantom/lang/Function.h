// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Subroutine.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a procedural function.
class PHANTOM_EXPORT_PHANTOM Function : public Subroutine
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    friend class Scope;

public:
    PHANTOM_DECLARE_META_CLASS(Function);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a void function without parameters with given name, ABI and custom
    /// modifiers.
    ///
    /// \param  a_strName   The function name.
    /// \param  a_eABI      The ABI.
    /// \param  a_Modifiers (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Function(StringView a_strName, ABI a_eABI, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a function with given name, signature and optional modifiers.
    ///
    /// \param  a_strName               The function name.
    /// \param [in,out] a_pSignature    The function signature.
    /// \param  a_Modifiers             (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Function(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a function with given name, signature, ABI and optional modifiers..
    ///
    /// \param  a_strName               The function name.
    /// \param [in,out] a_pSignature    The function signature.
    /// \param  a_eABI                  The ABI.
    /// \param  a_Modifiers             (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Function(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    Function* asFunction() const override { return (Function*)this; }

    void invoke(void*, void** a_pArgs) const override { call(a_pArgs); }
    void invoke(void*, void** a_pArgs, void* a_pReturnAddress) const override { call(a_pArgs, a_pReturnAddress); }
};

} // namespace lang
} // namespace phantom
