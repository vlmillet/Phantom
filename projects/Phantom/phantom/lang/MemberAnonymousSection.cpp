// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "MemberAnonymousSection.h"

#include "Field.h"
#include "MemberAnonymousUnion.h"
/* *********************************************** */

namespace phantom
{
namespace lang
{
MemberAnonymousSection::MemberAnonymousSection(Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol("", modifiers, a_uiFlags), DataElement(this), m_uiSize(0), m_uiAlignment(0), m_uiOffset(0)
{
}

void MemberAnonymousSection::addField(Field* a_pField)
{
    PHANTOM_ASSERT(a_pField);
    PHANTOM_ASSERT(getEnclosingClassType());
    addReferencedElement(a_pField);
    m_Fields.push_back(a_pField);
    m_DataElements.push_back(a_pField);
    PHANTOM_ASSERT(a_pField->m_pMemberAnonymousSection == nullptr);
    a_pField->m_pMemberAnonymousSection = this;
    getEnclosingClassType()->addField(a_pField);
}

MemberAnonymousUnion* MemberAnonymousSection::getEmbeddingMemberAnonymousUnion() const
{
    MemberAnonymousSection* pSection = getOwnerSection();
    if (pSection)
    {
        if (pSection->asMemberAnonymousUnion())
        {
            return static_cast<MemberAnonymousUnion*>(pSection);
        }
        return pSection->getEmbeddingMemberAnonymousUnion();
    }
    return nullptr;
}

void MemberAnonymousSection::addMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection)
{
    PHANTOM_ASSERT(std::find(m_MemberAnonymousSections.begin(), m_MemberAnonymousSections.end(),
                             a_pMemberAnonymousSection) == m_MemberAnonymousSections.end());
    m_MemberAnonymousSections.push_back(a_pMemberAnonymousSection);
    m_DataElements.push_back(a_pMemberAnonymousSection);
    a_pMemberAnonymousSection->setOwner(this);
}

size_t MemberAnonymousSection::getSize() const
{
    if (m_uiSize == 0)
        computeSizeAndAlignment(m_uiSize, m_uiAlignment);
    return m_uiSize;
}

size_t MemberAnonymousSection::getAlignment() const
{
    if (m_uiAlignment == 0)
        computeSizeAndAlignment(m_uiSize, m_uiAlignment);
    return m_uiAlignment;
}

void MemberAnonymousSection::fetchFields(Fields& a_Out) const
{
    for (auto pDataElement : m_DataElements)
        pDataElement->fetchFields(a_Out);
}

Field* MemberAnonymousSection::getField(StringView a_strName) const
{
    for (auto it = m_Fields.begin(); it != m_Fields.end(); ++it)
    {
        if ((*it)->getName() == a_strName)
            return *it;
    }
    for (auto it = m_MemberAnonymousSections.begin(); it != m_MemberAnonymousSections.end(); ++it)
    {
        Field* pDM = (*it)->getField(a_strName);
        if (pDM)
            return pDM;
    }
    return nullptr;
}

Field* MemberAnonymousSection::getFirstField() const
{
    if (getDataElements().empty())
        return nullptr;
    auto p = getDataElements()[0];
    if (Field* pDM = p->asSymbol()->asField())
    {
        return pDM;
    }
    return static_cast<MemberAnonymousSection*>(p)->getFirstField();
}

void MemberAnonymousSection::setOffset(size_t a_uiOffset)
{
    for (auto pDataElement : m_DataElements)
    {
        pDataElement->setOffset(pDataElement->getOffset() + a_uiOffset);
    }
    m_uiOffset = a_uiOffset;
}

} // namespace lang
} // namespace phantom
