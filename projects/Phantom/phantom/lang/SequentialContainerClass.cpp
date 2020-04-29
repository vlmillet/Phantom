// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "SequentialContainerClass.h"

#include "Method.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
SequentialContainerClass::SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize,
                                                   size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags)
    : ContainerClass(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

SequentialContainerClass::SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName,
                                                   Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ContainerClass(a_eTypeKind, a_strName, a_Modifiers, a_uiFlags), m_Data(PHANTOM_NEW(RTData))
{
}

SequentialContainerClass::~SequentialContainerClass()
{
    Delete<RTData>(m_Data);
}

void SequentialContainerClass::push_back(void* a_pContainer, void const* a_pValue) const
{
    if (!m_Data->m_pFunc_push_back)
    {
        _PHNTM_R_MTX_GUARD();
        PHANTOM_VERIFY(m_Data->m_pFunc_push_back =
                       getMethod("push_back", Types{m_pValueType->addConstLValueReference()}));
    }
    void* args[] = {(void*)a_pValue};
    m_Data->m_pFunc_push_back->invoke((void*)a_pContainer, args);
}

void SequentialContainerClass::push_back(void* a_pContainer, MoveArg a_pValue) const
{
    if (!m_Data->m_pFunc_push_back_move)
    {
        _PHNTM_R_MTX_GUARD();
        PHANTOM_VERIFY(m_Data->m_pFunc_push_back_move =
                       getMethod("push_back", Types{m_pValueType->addRValueReference()}));
    }
    void* args[] = {(void*)a_pValue};
    m_Data->m_pFunc_push_back_move->invoke((void*)a_pContainer, args);
}

void SequentialContainerClass::pop_back(void* a_pContainer) const
{
    if (!m_Data->m_pFunc_pop_back)
    {
        _PHNTM_R_MTX_GUARD();
        PHANTOM_VERIFY(m_Data->m_pFunc_pop_back = getMethod("pop_back", Types{}));
    }
    m_Data->m_pFunc_pop_back->invoke((void*)a_pContainer, nullptr);
}

} // namespace lang
} // namespace phantom
