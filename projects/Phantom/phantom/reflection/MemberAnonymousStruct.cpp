// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "MemberAnonymousStruct.h"
#include "Type.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
MemberAnonymousStruct::MemberAnonymousStruct(Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : MemberAnonymousSection(modifiers, a_uiFlags)
{
}

void MemberAnonymousStruct::computeSizeAndAlignment(size_t& a_uiSize, size_t& a_uiAlignment) const
{
    Type::AlignmentComputer().alignStruct(m_DataElements, a_uiSize, a_uiAlignment);
}

} // namespace reflection
} // namespace phantom
