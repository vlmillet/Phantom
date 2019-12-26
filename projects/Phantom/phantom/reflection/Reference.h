// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/ExtendedType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Base class for reference types (l-value, r-value or custom reference type).
class PHANTOM_EXPORT_PHANTOM Reference : public ExtendedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Reference);

public:
    friend class Type;

protected:
    Reference(TypeKind a_eTypeKind, Type* a_pReferencedType, StringView a_strName);

public:
    PHANTOM_DTOR ~Reference() override;

    bool isDefaultConstructible() const override
    {
        return false;
    }

    Type* removeAddress() const override
    {
        return m_pUnderlyingType;
    }
    Type* asAddressType() const override
    {
        return const_cast<Reference*>(this);
    }
    Reference* asReference() const override
    {
        return const_cast<Reference*>(this);
    }
    Type* asClassAddressType() const override
    {
        return (m_pUnderlyingType AND m_pUnderlyingType->asClass()) ? const_cast<Reference*>(this) : nullptr;
    }
    virtual Reference* asClassReference() const
    {
        return (m_pUnderlyingType AND m_pUnderlyingType->asClass()) ? const_cast<Reference*>(this) : nullptr;
    }
    Type* asConstClassAddressType() const override
    {
        return (m_pUnderlyingType AND m_pUnderlyingType->asConstClass()) ? const_cast<Reference*>(this) : nullptr;
    }
    virtual Reference* asConstClassReference() const
    {
        return (m_pUnderlyingType AND m_pUnderlyingType->asConstClass()) ? const_cast<Reference*>(this) : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the referenced type (int& => int).
    ///
    /// \return The referenced type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getReferencedType() const
    {
        return m_pUnderlyingType;
    }

    void valueFromString(StringView a_str, void* dest) const override
    {
        m_pUnderlyingType->valueFromString(a_str, dest);
    }

    void valueToString(StringBuffer& a_Buf, const void* src) const override
    {
        return m_pUnderlyingType->valueToString(a_Buf, src);
    }

    Type* removeReference() const override
    {
        return m_pUnderlyingType;
    }

    Type* addLValueReference() const override = 0;

    Type* addRValueReference() const override = 0;

    Type* removeAllConst() const override = 0;

    Type* removeAllQualifiers() const override = 0;

    Type* replicate(Type* a_pSource) const override = 0;

    bool isCopyable() const override
    {
        return false;
    }

    void copyAssign(void*, void const*) const override
    {
        PHANTOM_ASSERT_NO_IMPL();
    }

    void moveAssign(void*, void*) const override
    {
        PHANTOM_ASSERT_NO_IMPL();
    }

    void copyConstruct(void*, void const*) const override
    {
        PHANTOM_ASSERT_NO_IMPL();
    }

    void moveConstruct(void*, void*) const override
    {
        PHANTOM_ASSERT_NO_IMPL();
    }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const override = 0;

    bool isSame(Symbol* a_pOther) const override = 0;

    void address(void** a_pArgs, void* a_pOutput)
    {
        *(void**)a_pOutput = a_pArgs[0];
    }

    Type* getUnderlyingType() const override
    {
        return m_pUnderlyingType;
    }

    void memcpyStoreDelegate(void** a_pArgs, void* a_pOutput)
    {
        memcpy(a_pArgs[0], a_pArgs[1], m_pUnderlyingType->getSize());
        *(void**)a_pOutput = (void*)a_pArgs[0];
    }

protected:
    ConstType* createConstType() const override
    {
        return nullptr;
    } // cannot create reference const
    Pointer* createPointer() const override
    {
        return nullptr;
    } // cannot create pointers on reference
    virtual Reference* createReference() const
    {
        return nullptr;
    } // cannot create reference of reference
    RValueReference* createRValueReference() const override
    {
        return nullptr;
    } // cannot create rvalue reference of reference

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;
};

} // namespace reflection
} // namespace phantom
