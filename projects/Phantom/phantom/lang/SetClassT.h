// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/ContainerClassT.h>
#include <phantom/lang/SetClass.h>

namespace phantom
{
namespace lang
{
template<typename T, typename Base = SetClass>
class SetClassT : public ContainerClassT<T, Base>
{
public:
    using BaseType = ContainerClassT<T, Base>;
    typedef PHANTOM_TYPENAME T::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME T::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME T::iterator ContainerIterator;

public:
    SetClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    void insert(void* a_pContainer, void const* a_pKey) const override
    {
        ContainerValueType const* pKey = static_cast<ContainerValueType const*>(a_pKey);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        pContainer->insert(*pKey);
    }

    void eraseKey(void* a_pContainer, void const* a_pKey) const override
    {
        ContainerValueType const* pKey = static_cast<ContainerValueType const*>(a_pKey);
        T*                        pContainer = static_cast<T*>(a_pContainer);
        pContainer->erase(pContainer->find(*pKey));
    }

    void find(void const* a_pContainer, void const* a_pKey, void* a_pIt) const override
    {
        ContainerValueType const* pKey = static_cast<ContainerValueType const*>(a_pKey);
        T const*                  pContainer = static_cast<T const*>(a_pContainer);
        *reinterpret_cast<ContainerConstIterator*>(a_pIt) = pContainer->find(*pKey);
    }
};
} // namespace lang
} // namespace phantom
