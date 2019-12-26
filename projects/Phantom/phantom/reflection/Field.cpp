// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Field.h"

#include "MemberAnonymousUnion.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Field::Field(Type* a_pValueType, StringView a_strName, uint a_uiFilterMask /*= 0xffffffff*/,
             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ValueMember(a_pValueType, a_strName, a_uiFilterMask, a_Modifiers, a_uiFlags),
      DataElement(this),
      m_uiOffset(InvalidOffset),
      m_pMemberAnonymousSection(nullptr)
{
}

Field::Field(Type* a_pValueType, StringView a_strName, size_t a_uiOffset, uint a_uiFilterMask,
             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ValueMember(a_pValueType, a_strName, a_uiFilterMask, a_Modifiers, a_uiFlags),
      DataElement(this),
      m_uiOffset(a_uiOffset),
      m_pMemberAnonymousSection(nullptr)
{
}

void Field::getValue(void const* a_pObject, void* a_pDest) const
{
    m_pValueType->copyConstruct(a_pDest, getAddress(a_pObject));
}

void Field::setValue(void* a_pObject, void const* a_pSrc) const
{
    m_pValueType->copyConstruct(getAddress(a_pObject), a_pSrc);
}

void Field::setValueMoved(void* a_pObject, void* a_pSrc) const
{
    m_pValueType->moveConstruct(getAddress(a_pObject), a_pSrc);
}

void Field::fetchFields(Fields& a_Out) const
{
    a_Out.push_back(const_cast<Field*>(this));
}

void Field::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    ValueMember::onReferencedElementRemoved(a_pElement);
}

MemberAnonymousUnion* Field::getEmbeddingMemberAnonymousUnion() const
{
    if (m_pMemberAnonymousSection)
    {
        if (m_pMemberAnonymousSection->asMemberAnonymousUnion())
        {
            return static_cast<MemberAnonymousUnion*>(m_pMemberAnonymousSection);
        }
        return m_pMemberAnonymousSection->getEmbeddingMemberAnonymousUnion();
    }
    return nullptr;
}

void Field::setValueType(Type* a_pType)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(a_pType);
    PHANTOM_ASSERT(m_pValueType);
    removeReferencedElement(m_pValueType);
    m_pValueType = a_pType;
    addReferencedElement(m_pValueType);
}

void Field::setAlignment(size_t a_Alignment)
{
    PHANTOM_ASSERT(getAlignment() <= a_Alignment);
    m_Alignment = a_Alignment;
}

size_t Field::getAlignment() const
{
    return m_Alignment ? m_Alignment : m_pValueType->getAlignment();
}

size_t Field::getSize() const
{
    return m_pValueType->getSize();
}

} // namespace reflection
} // namespace phantom
