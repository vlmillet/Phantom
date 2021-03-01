// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ExtendedType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for reference types (l-value, r-value or custom reference type).
class PHANTOM_EXPORT_PHANTOM Reference : public ExtendedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Reference);

public:
    friend class Type;

protected:
    Reference(TypeKind a_eTypeKind, Type* a_pReferencedType, StringView a_strName)
        : ExtendedType(a_eTypeKind, a_pReferencedType, a_strName, sizeof(void*), std::alignment_of<void*>::value, 0,
                       a_pReferencedType->getFlags() | PHANTOM_R_FLAG_IMPLICIT)
    {
    }

public:
    bool isDefaultConstructible() const override { return false; }

    Type* asClassAddressType() const override
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<Reference*>(this) : nullptr;
    }
    virtual Reference* asClassReference() const
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<Reference*>(this) : nullptr;
    }
    Type* asConstClassAddressType() const override
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asConstClass()) ? const_cast<Reference*>(this) : nullptr;
    }
    virtual Reference* asConstClassReference() const
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asConstClass()) ? const_cast<Reference*>(this) : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the referenced type (int& => int).
    ///
    /// \return The referenced type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getReferencedType() const { return m_pUnderlyingType; }

    void valueFromString(StringView a_str, void* dest) const override
    {
        m_pUnderlyingType->valueFromString(a_str, dest);
    }

    void valueToString(StringBuffer& a_Buf, const void* src) const override
    {
        return m_pUnderlyingType->valueToString(a_Buf, src);
    }

    Type* replicate(Type* a_pSource) const override = 0;

    bool isCopyable() const override { return false; }

    void copyAssign(void*, void const*) const override { PHANTOM_ASSERT_NO_IMPL(); }

    void moveAssign(void*, void*) const override { PHANTOM_ASSERT_NO_IMPL(); }

    void copyConstruct(void*, void const*) const override { PHANTOM_ASSERT_NO_IMPL(); }

    void moveConstruct(void*, void*) const override { PHANTOM_ASSERT_NO_IMPL(); }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const override = 0;

    bool isSame(Symbol* a_pOther) const override = 0;

    void address(void** a_pArgs, void* a_pOutput) { *(void**)a_pOutput = a_pArgs[0]; }

    void memcpyStoreDelegate(void** a_pArgs, void* a_pOutput)
    {
        memcpy(a_pArgs[0], a_pArgs[1], m_pUnderlyingType->getSize());
        *(void**)a_pOutput = (void*)a_pArgs[0];
    }

    bool isMoveAssignable() const override { return false; }
    bool isMoveConstructible() const override { return false; }
    bool isCopyAssignable() const override { return false; }
    bool isCopyConstructible() const override { return false; }
};

} // namespace lang
} // namespace phantom
