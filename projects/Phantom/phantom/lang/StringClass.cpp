// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "StringClass.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
StringClass::StringClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                         uint a_uiFlags)
    : SequentialContainerClass(TypeKind::StringClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags),
#pragma message("TODO : move m_pData init to .h")
      m_pData{}
{
}

StringClass::StringClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : SequentialContainerClass(TypeKind::StringClass, a_strName, a_Modifiers, a_uiFlags), m_pData{}
{
}

void StringClass::initialize()
{
    SequentialContainerClass::initialize();
    if (!isNative())
        m_pData = new_<RTData>();
}

void const* StringClass::data(void const* a_pContainer) const
{
    if (!m_pData->m_pFunc_datac)
        m_pData->m_pFunc_datac = getMethod("data() const");
    PHANTOM_ASSERT(m_pData->m_pFunc_datac);
    void* ptr;
    m_pData->m_pFunc_datac->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void* StringClass::data(void* a_pContainer) const
{
    if (!m_pData->m_pFunc_data)
        m_pData->m_pFunc_data = getMethod("data()");
    PHANTOM_ASSERT(m_pData->m_pFunc_data);
    void* ptr;
    m_pData->m_pFunc_data->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void StringClass::terminate()
{
    if (m_pData)
        delete_<RTData>(m_pData);
    SequentialContainerClass::terminate();
}

} // namespace lang
} // namespace phantom
