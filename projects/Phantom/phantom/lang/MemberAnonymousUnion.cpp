// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "MemberAnonymousUnion.h"

#include "Type.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
MemberAnonymousUnion::MemberAnonymousUnion(Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : MemberAnonymousSection(modifiers, a_uiFlags)
{
}

void MemberAnonymousUnion::computeSizeAndAlignment(size_t& a_uiSize, size_t& a_uiAlignment) const
{
    Type::AlignmentComputer().alignUnion(m_DataElements, m_uiSize, m_uiAlignment);
    a_uiSize = m_uiSize;
    a_uiAlignment = m_uiSize;
}

} // namespace lang
} // namespace phantom
