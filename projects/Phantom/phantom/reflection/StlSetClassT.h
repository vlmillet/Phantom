// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/reflection/StlContainerClassT.h>
#include <phantom/reflection/StlSetClass.h>

namespace phantom
{
namespace reflection
{
template<typename t_Ty, typename Base = StlSetClass>
class StlSetClassT : public StlContainerClassT<t_Ty, Base>
{
public:
    using BaseType = StlContainerClassT<t_Ty, Base>;
    typedef PHANTOM_TYPENAME t_Ty::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME t_Ty::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME t_Ty::iterator ContainerIterator;

public:
    StlSetClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    virtual void insert(void* a_pContainer, void const* a_pKey) const override
    {
        ContainerValueType const* pKey = static_cast<ContainerValueType const*>(a_pKey);
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        pContainer->insert(*pKey);
    }

    virtual void eraseKey(void* a_pContainer, void const* a_pKey) const override
    {
        ContainerValueType const* pKey = static_cast<ContainerValueType const*>(a_pKey);
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        pContainer->erase(pContainer->find(*pKey));
    }
};
} // namespace reflection
} // namespace phantom
