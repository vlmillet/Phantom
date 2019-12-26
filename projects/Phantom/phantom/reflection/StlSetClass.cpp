// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "StlSetClass.h"

#include "Method.h"
#include "phantom/new.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
StlSetClass::StlSetClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                         Modifiers a_Modifiers, uint a_uiFlags)
    : ContainerClass(TypeKind::StlSetClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers,
                     a_uiFlags)
{
}

StlSetClass::StlSetClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                         uint a_uiFlags /*= 0*/)
    : ContainerClass(TypeKind::StlSetClass, a_strName, a_Modifiers, a_uiFlags),
      m_Data(PHANTOM_NEW(RTData))
{
}

StlSetClass::~StlSetClass()
{
    PHANTOM_DELETE(RTData) m_Data;
}

void StlSetClass::eraseKey(void* a_pContainer, void const* a_pKey) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_erase)
        m_Data->m_pFunc_erase = getMethod("erase", {m_pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_erase->invoke(a_pContainer, args);
}

void StlSetClass::insert(void* a_pContainer, void const* a_pKey) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_insert)
        m_Data->m_pFunc_insert = getMethod("insert", {m_pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_insert->invoke(a_pContainer, args);
}

void StlSetClass::find(void const* a_pContainer, void const* a_pKey, void* a_pIt) const
{
    PHANTOM_ASSERT(m_pKeyType && m_pMappedType);
    if (!m_Data->m_pFunc_find)
    {
        PHANTOM_VERIFY(m_Data->m_pFunc_find =
                       getMethod("find", {m_pKeyType->addConstLValueReference()}, Modifier::Const));
    }
    void* args[] = {(void*)a_pKey};
    m_Data->m_pFunc_find->invoke((void*)a_pContainer, args, a_pIt);
}

} // namespace reflection
} // namespace phantom
