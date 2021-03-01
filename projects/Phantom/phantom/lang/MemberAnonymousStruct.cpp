// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "MemberAnonymousStruct.h"

#include "Type.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
MemberAnonymousStruct::MemberAnonymousStruct(Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : MemberAnonymousSection(modifiers, a_uiFlags)
{
}

void MemberAnonymousStruct::computeSizeAndAlignment(size_t& a_uiSize, size_t& a_uiAlignment) const
{
    Type::AlignmentComputer().alignStruct(m_DataElements, a_uiSize, a_uiAlignment);
}

} // namespace lang
} // namespace phantom
