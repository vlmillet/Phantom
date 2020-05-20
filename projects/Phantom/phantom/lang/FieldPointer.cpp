// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "FieldPointer.h"

#include "Field.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
FieldPointer::FieldPointer(ClassType* a_pObjectType, Type* a_pValueType, Modifiers a_Modifiers /*= 0*/,
                           uint a_uiFlags /*= 0*/)
    : MemberPointer(TypeKind::FieldPointer, a_pObjectType, sizeof(Field*), std::alignment_of<Field*>::value,
                    a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_IMPLICIT),
      m_pValueType(a_pValueType)
{
}

FieldPointer::FieldPointer(ClassType* a_pObjectType, Type* a_pValueType, size_t a_uiSize, size_t a_uiAlignment,
                           Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : MemberPointer(TypeKind::MethodPointer, a_pObjectType, a_uiSize, a_uiAlignment, a_Modifiers,
                    a_uiFlags | PHANTOM_R_FLAG_IMPLICIT),
      m_pValueType(a_pValueType)
{
}

size_t FieldPointer::getOffset(void* a_pPointer) const
{
    return static_cast<Field*>(a_pPointer)->getOffset();
}

void FieldPointer::setValue(void* a_pPointer, void* a_pObject, void const* src) const
{
    static_cast<Field*>(a_pPointer)->setValue(a_pObject, src);
}

void FieldPointer::getValue(void* a_pPointer, void const* a_pObject, void* dest) const
{
    static_cast<Field*>(a_pPointer)->getValue(a_pObject, dest);
}

void* FieldPointer::getAddress(void* a_pPointer, void const* a_pObject) const
{
    return static_cast<Field*>(a_pPointer)->getAddress(a_pObject);
}

} // namespace lang
} // namespace phantom
