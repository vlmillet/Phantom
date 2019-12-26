// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "ClassType.h"
#include "MemberPointer.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
MemberPointer::MemberPointer(TypeKind a_TypeId, ClassType* a_pObjectType, size_t a_uiSize,
                             size_t a_uiAlignment, Modifiers a_Modifiers /*= 0*/,
                             uint a_uiFlags /*= 0*/)
    : Type(a_TypeId, "", a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags),
      m_pObjectType(a_pObjectType)
{
    if (m_pObjectType->isTemplateDependant())
        setTemplateDependant();
}

PHANTOM_DTOR MemberPointer::~MemberPointer()
{
}

} // namespace reflection
} // namespace phantom
