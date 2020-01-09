// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/ContainerClass.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Represents a specialized version of Class for map containers.
class PHANTOM_EXPORT_PHANTOM MapClass : public ContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(MapClass);

    struct Data
    {
        Method* m_pFunc_map = nullptr;
        Method* m_pFunc_insert = nullptr;
        Method* m_pFunc_eraseKey = nullptr;
        Method* m_pFunc_find = nullptr;
        Method* m_pFunc_findc = nullptr;
    };

protected:
    MapClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                uint a_uiFlags);

public:
    MapClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ~MapClass() override;

    Type* getKeyType() const
    {
        return m_pKeyType;
    }
    Type* getMappedType() const
    {
        return m_pMappedType;
    }
    void setKeyType(Type* a_pType)
    {
        m_pKeyType = a_pType;
    }
    void setMappedType(Type* a_pType)
    {
        m_pMappedType = a_pType;
    }

    virtual void eraseKey(void* a_pContainer, void const* a_pKey) const;
    virtual void insert(void* a_pContainer, void const* a_pPair) const;
    virtual void map(void* a_pContainer, void const* a_pKey, void* a_pOutPairPointer) const;
    virtual void find(void const* a_pContainer, void const* a_pKey, void* a_pOutIt) const;
    virtual void find(void* a_pContainer, void const* a_pKey, void* a_pOutIt) const;

protected:
    Type*         m_pKeyType = nullptr;
    Type*         m_pMappedType = nullptr;
    mutable Data* m_Data = nullptr;
};

} // namespace reflection
} // namespace phantom
