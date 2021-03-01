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
class Block;

/// \brief  Represents a local variable.
class PHANTOM_EXPORT_PHANTOM LocalVariable : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    friend class Block;

    PHANTOM_DECLARE_META_CLASS(LocalVariable);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a local variable with value type, name and modifiers.
    ///
    /// \param [in,out] a_pValueType        The value type.
    /// \param  a_strName                   The name.
    /// \param  modifiers                   (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable(Type* a_pValueType, StringView a_strName, Modifiers modifiers = 0, uint a_uiFlags = 0);

    void initialize()
    {
        Symbol::initialize();
        addReferencedElement(m_pValueType);
    }

    LocalVariable* asLocalVariable() const override { return (LocalVariable*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets value type of this local variable.
    ///
    /// \return null if it fails, else the value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    lang::Type* getValueType() const { return m_pValueType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this local variable is a member function 'this'.
    ///
    /// \return true if this local variable is a member function 'this', else false.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isThis() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the block containing this local variable.
    ///
    /// \return null if it fails, else the block.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Block* getBlock() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine containing this local variable.
    ///
    /// \return null if it fails, else the subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the offset where to find the local variable from the eval interpreter stack
    /// frame base address.
    ///
    /// \param  a_iOffset   The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setEvalFrameOffset(ptrdiff_t a_iOffset)
    {
        PHANTOM_ASSERT(m_iEvalFrameOffset == -1, "frame offset already defined and cannot be changed");
        m_iEvalFrameOffset = a_iOffset;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the offset where to find the local variable from the native stack frame base
    /// address.
    ///
    /// \param  a_iOffset   The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setNativeFrameOffset(ptrdiff_t a_iOffset)
    {
        PHANTOM_ASSERT(m_iNativeFrameOffset == -1, "frame offset already defined and cannot be changed");
        m_iNativeFrameOffset = a_iOffset;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset where to find the local variable from the eval interpreter stack
    /// frame base address.
    ///
    /// \return The frame offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrdiff_t getEvalFrameOffset() const { return m_iEvalFrameOffset; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset where to find the local variable from the native stack frame base
    /// address.
    ///
    /// \return The frame offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrdiff_t getNativeFrameOffset() const { return m_iNativeFrameOffset; }

    LocalVariable* clone(LanguageElement* a_pOwner) const;

    virtual LocalVariable* cloneImpl(LanguageElement* a_pOwner) const;

    void getQualifiedName(StringBuffer& a_Buf) const override;

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

    void setValueType(Type* a_pType);

protected:
    Type*     m_pValueType = nullptr;
    ptrdiff_t m_iEvalFrameOffset = -1;
    ptrdiff_t m_iNativeFrameOffset = -1;
};

} // namespace lang
} // namespace phantom
