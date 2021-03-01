// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "SetClass.h"

#include "Method.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
SetClass::SetClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags)
    : ContainerClass(TypeKind::SetClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

SetClass::SetClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ContainerClass(TypeKind::SetClass, a_strName, a_Modifiers, a_uiFlags)
{
}

void SetClass::initialize()
{
    ContainerClass::initialize();
    if (!isNative())
        m_pData = new_<RTData>();
}
void SetClass::terminate()
{
    if (m_pData)
        delete_<RTData>(m_pData);
    ContainerClass::terminate();
}

void SetClass::eraseKey(void* a_pContainer, void const* a_pKey) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_pData->m_pFunc_erase)
        m_pData->m_pFunc_erase = getMethod("erase", {m_pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_erase->invoke(a_pContainer, args);
}

void SetClass::insert(void* a_pContainer, void const* a_pKey) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_pData->m_pFunc_insert)
        m_pData->m_pFunc_insert = getMethod("insert", {m_pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_insert->invoke(a_pContainer, args);
}

void SetClass::find(void const* a_pContainer, void const* a_pKey, void* a_pIt) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_pData->m_pFunc_find)
    {
        PHANTOM_VERIFY(m_pData->m_pFunc_find =
                       getMethod("find", {m_pKeyType->addConstLValueReference()}, Modifier::Const));
    }
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_find->invoke((void*)a_pContainer, args, a_pIt);
}

} // namespace lang
} // namespace phantom
