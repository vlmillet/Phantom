// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "StlVectorClass.h"

#include "Method.h"

#include <phantom/new.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
StlVectorClass::StlVectorClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                               uint a_uiFlags)
    : StlVectorClass(TypeKind::StlVectorClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

StlVectorClass::StlVectorClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : StlVectorClass(TypeKind::StlVectorClass, a_strName, a_Modifiers, a_uiFlags)
{
}

StlVectorClass::StlVectorClass(TypeKind a_TypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                               Modifiers a_Modifiers, uint a_uiFlags)
    : SequentialContainerClass(a_TypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

StlVectorClass::StlVectorClass(TypeKind a_TypeKind, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                               uint a_uiFlags /*= 0*/)
    : SequentialContainerClass(a_TypeKind, a_strName, a_Modifiers, a_uiFlags), m_pData(PHANTOM_NEW(RTData))
{
}

StlVectorClass::~StlVectorClass()
{
    PHANTOM_DELETE(RTData) m_pData;
}

void const* StlVectorClass::data(void const* a_pContainer) const
{
    if (!m_pData->m_pFunc_datac)
        m_pData->m_pFunc_datac = getMethod("data() const");
    PHANTOM_ASSERT(m_pData->m_pFunc_datac);
    void* ptr;
    m_pData->m_pFunc_datac->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void* StlVectorClass::data(void* a_pContainer) const
{
    if (!m_pData->m_pFunc_data)
        m_pData->m_pFunc_data = getMethod("data()");
    PHANTOM_ASSERT(m_pData->m_pFunc_data);
    void* ptr;
    m_pData->m_pFunc_data->invoke((void*)a_pContainer, nullptr, &ptr);
    return ptr;
}

void StlVectorClass::resize(void* a_pContainer, size_t a_Size) const
{
    if (!m_pData->m_pFunc_resize)
        m_pData->m_pFunc_resize = getMethod("resize", {PHANTOM_TYPEOF(size_t)});
    PHANTOM_ASSERT(m_pData->m_pFunc_resize);
    return m_pData->m_pFunc_resize->invoke<void>((void*)a_pContainer, a_Size);
}

void const* StlVectorClass::referenceAt(void const* a_pContainer, size_t a_uiIndex) const
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

void* StlVectorClass::referenceAt(void* a_pContainer, size_t a_uiIndex) const
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

} // namespace reflection
} // namespace phantom
