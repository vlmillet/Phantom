// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "MapClass.h"

#include "Method.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
MapClass::MapClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags)
    : ContainerClass(TypeKind::MapClass, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

MapClass::MapClass(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : ContainerClass(TypeKind::MapClass, a_strName, a_Modifiers, a_uiFlags)
{
}

void MapClass::initialize()
{
    ContainerClass::initialize();
    if (!isNative())
        m_pData = new_<RTData>();
}
void MapClass::terminate()
{
    if (m_pData)
        delete_<RTData>(m_pData);
    ContainerClass::terminate();
}

Type* MapClass::getKeyType() const
{
    if (!isNative() && m_pKeyType == nullptr)
    {
        Alias* pAlias = getAlias("key_type");
        if (pAlias && pAlias->getAliasedSymbol())
        {
            Symbol* pAliased = pAlias->getAliasedSymbol();
            if (!(m_pKeyType = pAliased->asType()))
                pAlias = pAliased->asAlias();
        }
    }
    return m_pKeyType;
}

Type* MapClass::getMappedType() const
{
    if (!isNative() && m_pMappedType == nullptr)
    {
        Alias* pAlias = getAlias("mapped_type");
        if (pAlias && pAlias->getAliasedSymbol())
        {
            Symbol* pAliased = pAlias->getAliasedSymbol();
            if (!(m_pMappedType = pAliased->asType()))
                pAlias = pAliased->asAlias();
        }
    }
    return m_pMappedType;
}

void MapClass::eraseKey(void* a_pContainer, void const* a_pKey) const
{
    auto pKeyType = getKeyType();
    PHANTOM_ASSERT(pKeyType);
    if (!m_pData->m_pFunc_eraseKey)
        m_pData->m_pFunc_eraseKey = getMethod("erase", {pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_eraseKey->invoke(a_pContainer, args);
}

void MapClass::insert(void* a_pContainer, void const* a_pPair) const
{
    auto pValueType = getValueType();
    PHANTOM_ASSERT(pValueType);
    if (!m_pData->m_pFunc_insert)
        m_pData->m_pFunc_insert = getMethod("insert", {pValueType->addConstLValueReference()});
    void* args[] = {(void*)a_pPair};
    m_pData->m_pFunc_insert->invoke(a_pContainer, args);
}

void MapClass::map(void* a_pContainer, void const* a_pKey, void* a_pOutPairPointer) const
{
    auto pKeyType = getKeyType();
    PHANTOM_ASSERT(pKeyType);
    if (!m_pData->m_pFunc_map)
        m_pData->m_pFunc_map = getMethod("operator[]", {pKeyType->addConstLValueReference()});
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_map->invoke(a_pContainer, args, a_pOutPairPointer);
}

void MapClass::find(void const* a_pContainer, void const* a_pKey, void* a_pOutIt) const
{
    auto pKeyType = getKeyType();
    PHANTOM_ASSERT(pKeyType);
    if (!m_pData->m_pFunc_findc)
        m_pData->m_pFunc_findc = getMethod("find", {pKeyType->addConstLValueReference()}, Modifier::Const);
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_findc->invoke((void*)a_pContainer, args, a_pOutIt);
}

void MapClass::find(void* a_pContainer, void const* a_pKey, void* a_pOutIt) const
{
    auto pKeyType = getKeyType();
    PHANTOM_ASSERT(pKeyType);
    if (!m_pData->m_pFunc_find)
        m_pData->m_pFunc_find = getMethod("find", {pKeyType->addConstLValueReference()}, Modifier::Const);
    void* args[] = {(void*)a_pKey};
    m_pData->m_pFunc_find->invoke((void*)a_pContainer, args, a_pOutIt);
}

} // namespace lang
} // namespace phantom
