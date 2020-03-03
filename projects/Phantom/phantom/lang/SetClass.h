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
/// \brief  Base base for set containers class representation (set, multiset, ...).
class PHANTOM_EXPORT_PHANTOM SetClass : public ContainerClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(SetClass);

    struct RTData
    {
        Method* m_pFunc_insert = nullptr;
        Method* m_pFunc_find = nullptr;
        Method* m_pFunc_erase = nullptr;
        Method* m_pFunc_begin = nullptr;
        Method* m_pFunc_end = nullptr;
        Method* m_pFunc_iteq = nullptr;
        Method* m_pFunc_itpp = nullptr;
        Method* m_pFunc_itderef = nullptr;
    };

protected:
    SetClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);

public:
    SetClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ~SetClass() override;

    Type* getKeyType() const { return m_pValueType; }

    virtual void insert(void* a_pContainer, void const* a_pKey) const;
    virtual void eraseKey(void* a_pContainer, void const* a_pKey) const;
    virtual void find(void const* a_pContainer, void const* a_pKey, void* a_pIt) const;

protected:
    Type*           m_pKeyType = nullptr;
    Type*           m_pMappedType = nullptr;
    mutable RTData* m_Data = nullptr;
};

} // namespace lang
} // namespace phantom
