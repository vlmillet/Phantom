// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ContainerClass.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a specialized version of Class for map containers.
class PHANTOM_EXPORT_PHANTOM MapClass : public ContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(MapClass);

    struct RTData
    {
        Method* m_pFunc_map = nullptr;
        Method* m_pFunc_insert = nullptr;
        Method* m_pFunc_operatorBracket = nullptr;
        Method* m_pFunc_eraseKey = nullptr;
        Method* m_pFunc_find = nullptr;
        Method* m_pFunc_findc = nullptr;
    };

protected:
    MapClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);

public:
    MapClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    void initialize();
    void terminate() override;

    Type* getKeyType() const override final;
    Type* getMappedType() const override final;
    void  setKeyType(Type* a_pType) { m_pKeyType = a_pType; }
    void  setMappedType(Type* a_pType) { m_pMappedType = a_pType; }

    virtual void eraseKey(void* a_pContainer, void const* a_pKey) const override;
    virtual void insert(void* a_pContainer, void const* a_pPair) const;
    virtual void assign(void* a_pContainer, void const* a_pPair) const;
    virtual void assign(void* a_pContainer, void const* a_pKey, void const* a_pValue) const;
    virtual void map(void* a_pContainer, void const* a_pKey, void* a_pOutPairPointer) const;
    virtual void find(void const* a_pContainer, void const* a_pKey, void* a_pOutIt) const;
    virtual void find(void* a_pContainer, void const* a_pKey, void* a_pOutIt) const;

protected:
    mutable Type*   m_pKeyType = nullptr;
    mutable Type*   m_pMappedType = nullptr;
    mutable RTData* m_pData = nullptr;
};

} // namespace lang
} // namespace phantom
