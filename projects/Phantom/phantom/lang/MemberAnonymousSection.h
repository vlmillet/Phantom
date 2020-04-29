// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/DataElement.h>
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for member anonymous sections (unions and structs).
class PHANTOM_EXPORT_PHANTOM MemberAnonymousSection : public Symbol, public DataElement
{
    PHANTOM_DECLARE_META_CLASS(MemberAnonymousSection);

public:
    MemberAnonymousSection(Modifiers modifiers = 0, uint a_uiFlags = 0);

    MemberAnonymousSection* asMemberAnonymousSection() const override
    {
        return const_cast<MemberAnonymousSection*>(this);
    }
    DataElement* asDataElement() const override { return const_cast<MemberAnonymousSection*>(this); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the first data member accessible through this anonymous section (can be one of
    /// the sub sections).
    ///
    /// \return null if it fails, else the first data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getFirstField() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a data member to this anonymous section.
    ///
    /// \param a_pField   The data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addField(Field* a_pField);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the data member matching the given name.
    ///
    /// \param  a_strName   The data member name.
    ///
    /// \return null if no data member matched the given name, else the data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getField(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the data elements list (a data element is either a Data member, either an
    /// anonymous struct or union).
    ///
    /// \return the data elements list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const DataElements& getDataElements() const { return m_DataElements; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the data members list.
    ///
    /// \return the data members.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Fields& getFields() const { return m_Fields; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets root class type owner of the member anonymous sections' hierarchy.
    ///
    /// \return The root owner.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType* getRootOwner() const
    {
        return getOwnerSection() ? getOwnerSection()->getRootOwner() : getOwner() ? getOwner()->asClassType() : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the owner if it is another member anonymous section.
    ///
    /// \return null if owner is not a member anonymous section, else the owner section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousSection* getOwnerSection() const
    {
        return getOwner() ? getOwner()->asMemberAnonymousSection() : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the root member anonymous section.
    ///
    /// \return null if it fails, else the root section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousSection* getRootSection() const
    {
        return getOwnerSection() ? getOwnerSection()->getRootSection() : (MemberAnonymousSection*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the embedding anonymous union.
    ///
    /// \return null if no anonymous union embebbeds this section, else the anonymous union.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousUnion* getEmbeddingMemberAnonymousUnion() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a sub member anonymous section.
    ///
    /// \param [in,out] a_pMemberAnonymousSection   The member anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a sub member anonymous section.
    ///
    /// \param [in,out] a_pMemberAnonymousSection   The member anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member anonymous section at given index position.
    ///
    /// \param  a_uiIndex   The index.
    ///
    /// \return null if it fails, else the member anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousSection* getMemberAnonymousSection(size_t a_uiIndex) const
    {
        return m_MemberAnonymousSections[a_uiIndex];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member anonymous section count.
    ///
    /// \return The member anonymous section count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getMemberAnonymousSectionCount() const { return m_MemberAnonymousSections.size(); }

    size_t getSize() const override;

    size_t getAlignment() const override;

    size_t getOffset() const override { return m_uiOffset; }

    void fetchFields(Fields& a_Out) const override;

protected:
    void         setOffset(size_t a_uiOffset) override;
    virtual void computeSizeAndAlignment(size_t& a_uiSize, size_t& a_uiAlignment) const = 0;

protected:
    Fields                  m_Fields;
    MemberAnonymousSections m_MemberAnonymousSections;
    DataElements            m_DataElements;
    mutable size_t          m_uiSize;
    mutable size_t          m_uiAlignment;
    size_t                  m_uiOffset;
};

} // namespace lang
} // namespace phantom
