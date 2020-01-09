// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/PointerType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a function pointer type.
class PHANTOM_EXPORT_PHANTOM FunctionPointer : public PointerType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(FunctionPointer);

    friend class Source;

public:
    friend class Type;
    HAUNT_OFF typedef void (*minimal_function_pointer_t)(); /// cannot haunt due to VS2013 bug ...

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a function pointer type from a function type, an ABI and optional
    /// modifiers.
    ///
    /// \param [in,out] a_pSignature    The function signature.
    /// \param  a_eABI                  The function ABI.
    /// \param  a_Modifiers             (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionPointer(FunctionType* a_pFunctionType, ABI a_eABI, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    PHANTOM_DTOR ~FunctionPointer() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the closure of a function pointer.
    ///
    /// \param [in,out] a_pPointer  The function pointer.
    ///
    /// \return null if it fails, else the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Closure getClosure(void* a_pPointer) const;

    FunctionPointer* asFunctionPointer() const override
    {
        return const_cast<FunctionPointer*>(this);
    }

    void valueFromString(StringView a_str, void* dest) const override;

    void valueToString(StringBuffer& a_Buf, const void* src) const override;

    void valueToLiteral(StringBuffer& a_Buf, const void* src) const override;

    bool isCopyable() const override
    {
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the function type of the underlying function.
    ///
    /// \return The function type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType* getFunctionType() const
    {
        return (FunctionType*)m_pUnderlyingType;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the ABI of the underlying function.
    ///
    /// \return The ABI.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ABI getABI() const
    {
        return m_eABI;
    }

    virtual void call(void* a_pPointer, void** a_pArgs) const;
    virtual void call(void* a_pPointer, void** a_pArgs, void* a_pReturnAddress) const;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

protected:
    void                    onReferencedElementRemoved(LanguageElement* a_pElement) override;
    static FunctionPointer* Create(FunctionType* a_pFunctionType, ABI a_eABI, Modifiers a_Modifiers = 0,
                                   uint a_Flags = 0);

protected:
    ABI m_eABI;
};

} // namespace lang
} // namespace phantom
