// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "VectorClass.h"

#include "Method.h"

/* *********************************************** */
namespace phantom
{
namespace lang
{
VectorClass::VectorClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                         uint a_uiFlags)
    : VectorClass(TypeKind::VectorClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

VectorClass::VectorClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : VectorClass(TypeKind::VectorClass, a_strName, a_Modifiers, a_uiFlags)
{
}

VectorClass::VectorClass(TypeKind a_TypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                         Modifiers a_Modifiers, uint a_uiFlags)
    : SequentialContainerClass(a_TypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

VectorClass::VectorClass(TypeKind a_TypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : SequentialContainerClass(a_TypeKind, a_strName, a_Modifiers, a_uiFlags), m_pData(PHANTOM_NEW(RTData))
{
}

VectorClass::~VectorClass()
{
    Delete<RTData>(m_pData);
}

void const* VectorClass::data(void const* a_pContainer) const
{
    if (!m_pData->m_pFunc_datac)
        m_pData->m_pFunc_datac = getMethod("data() const");
    PHANTOM_ASSERT(m_pData->m_pFunc_datac);
    void* ptr;
    m_pData->m_pFunc_datac->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void* VectorClass::data(void* a_pContainer) const
{
    if (!m_pData->m_pFunc_data)
        m_pData->m_pFunc_data = getMethod("data()");
    PHANTOM_ASSERT(m_pData->m_pFunc_data);
    void* ptr;
    m_pData->m_pFunc_data->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void VectorClass::resize(void* a_pContainer, size_t a_Size) const
{
    if (!m_pData->m_pFunc_resize)
        m_pData->m_pFunc_resize = getMethod("resize", {PHANTOM_TYPEOF(size_t)});
    PHANTOM_ASSERT(m_pData->m_pFunc_resize);
    return m_pData->m_pFunc_resize->invoke<void>((void*)a_pContainer, a_Size);
}

void const* VectorClass::referenceAt(void const* a_pContainer, size_t a_uiIndex) const
{
    if (!m_pData->m_pFunc_refAtC)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_refAtC = getMethod("operator[]", {PHANTOM_TYPEOF(size_t)}, Modifier::Const));
    }
    void* args[] = {&a_uiIndex};
    void* ref;
    m_pData->m_pFunc_refAtC->invoke((void*)a_pContainer, args, &ref);
    return ref;
}

void* VectorClass::referenceAt(void* a_pContainer, size_t a_uiIndex) const
{
    if (!m_pData->m_pFunc_refAt)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_refAt = getMethod("operator[]", {PHANTOM_TYPEOF(size_t)}));
    }
    void* args[] = {&a_uiIndex};
    void* ref;
    m_pData->m_pFunc_refAt->invoke(a_pContainer, args, &ref);
    return ref;
}

} // namespace lang
} // namespace phantom
