// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Method.h"
#include "SequentialContainerClass.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
SequentialContainerClass::SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName,
                                                   size_t a_uiSize, size_t a_uiAlignment,
                                                   Modifiers a_Modifiers, uint a_uiFlags)
    : ContainerClass(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

SequentialContainerClass::SequentialContainerClass(TypeKind a_eTypeKind, StringView a_strName,
                                                   Modifiers a_Modifiers /*= 0*/,
                                                   uint      a_uiFlags /*= 0*/)
    : ContainerClass(a_eTypeKind, a_strName, a_Modifiers, a_uiFlags), m_Data(PHANTOM_NEW(RTData))
{
}

SequentialContainerClass::~SequentialContainerClass()
{
    PHANTOM_DELETE(RTData) m_Data;
}

void SequentialContainerClass::push_back(void* a_pContainer, void const* a_pValue) const
{
    if (!m_Data->m_pFunc_push_back)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_push_back =
                       getMethod("push_back", Types{m_pValueType->addConstLValueReference()}));
    }
    void* args[] = {(void*)a_pValue};
    m_Data->m_pFunc_push_back->invoke((void*)a_pContainer, args);
}

} // namespace reflection
} // namespace phantom
