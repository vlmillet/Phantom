// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/DataElement.h>
#include <phantom/lang/ValueMember.h>
/* ************* Class Declarations ************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a class type Data member.
class PHANTOM_EXPORT_PHANTOM Field : public ValueMember, public DataElement
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    friend class Class;
    friend class ClassType;
    friend class MemberAnonymousSection;

    PHANTOM_DECLARE_META_CLASS(Field);

public:
    static const size_t InvalidOffset = ~size_t(0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a Data member from a value type, a name and optionals range, database
    /// mask and modifiers.
    ///
    /// \param  a_pValueType            The Data member's value type.
    /// \param  a_strName               The Data member's name.
    /// \param  a_pRange                (optional) The Data member's value range.
    /// \param  a_uiFilterMask   (optional) the Data member's database mask.
    /// \param  a_uiModifiers           (optional) the Data member's modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field(Type* a_pValueType, StringView a_strName, uint a_uiFilterMask = PHANTOM_R_FILTER_FIELD,
          Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset from base address.
    ///
    /// \return The offset from base address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline size_t getOffset() const override { return m_uiOffset; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  adjusts address of the given instance to this field's address.
    ///
    /// \param  a_pInstance   The instance address.
    ///
    /// \return The adjusted address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getAddress(void const* a_pInstance) const override { return (byte*)a_pInstance + m_uiOffset; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  adjusts address of the given instance to this field's address and cast to 'T*'.
    ///
    /// \param  a_pInstance   The instance address.
    ///
    /// \return The adjusted address casted to 'T*'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T* getAddressAs(void const* a_pInstance) const
    {
        PHANTOM_ASSERT(getValueType() == PHANTOM_TYPEOF(T));
        return reinterpret_cast<T*>(getAddress(a_pInstance));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the anonymous section associated containing this Data member.
    ///
    /// \return null if it doesn't have associated anonymous section , else the anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousSection* getMemberAnonymousSection() const { return m_pMemberAnonymousSection; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the first anonymous union embedding this Data member.
    ///
    /// \return null if it doesn't have embedding anonymous union, else the anonymous union.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousUnion* getEmbeddingMemberAnonymousUnion() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  WARNING : do this only if you know what you do.
    ///         Resets the value type of this field.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setValueType(Type* a_pType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  sets the alignment of this field (alignas in C++)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setAlignment(size_t a_Alignment);

    // reimplemented from ValueMember

    bool isWrittable() const override { return true; }
    bool isReadable() const override { return true; }

    size_t getAlignment() const override;
    size_t getSize() const override;

    void getValue(void const* a_pObject, void* a_pDest) const override;
    void setValue(void* a_pObject, void const* a_pSrc) const override;
    void setValueMoved(void* a_pObject, void* src) const override;

    Field*       asField() const override { return const_cast<Field*>(this); }
    DataElement* asDataElement() const override { return const_cast<Field*>(this); }
    void         fetchFields(Fields& a_Out) const override;

protected:
    Field(Type* a_pValueType, StringView a_strName, size_t a_uiOffset, uint a_uiFilterMask, Modifiers a_Modifiers,
          uint a_uiFlags);
    void setOffset(size_t a_uiOffset) override { m_uiOffset = a_uiOffset; }

protected:
    size_t                  m_uiOffset;
    MemberAnonymousSection* m_pMemberAnonymousSection;
    size_t                  m_Alignment = 0;
};
} // namespace lang
} // namespace phantom
