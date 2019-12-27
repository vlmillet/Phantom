// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/detail/new.h>
#include <phantom/reflection/SequentialContainerClassT.h>
#include <phantom/reflection/StlVectorClass.h>

namespace phantom
{
namespace reflection
{
template<typename t_Ty, class Base = StlVectorClass>
class StlVectorClassT : public SequentialContainerClassT<t_Ty, Base>
{
    using SelfType = StlVectorClassT<t_Ty, Base>;
    using BaseType = SequentialContainerClassT<t_Ty, Base>;
    typedef t_Ty             ContainerType;
    typedef PHANTOM_TYPENAME t_Ty::value_type ContainerValueType;

public:
    StlVectorClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    virtual void* data(void* a_pContainer) const override
    {
        t_Ty* pContainer = static_cast<t_Ty*>(a_pContainer);
        return (void*)pContainer->data();
    }

    virtual const void* data(const void* a_pContainer) const override
    {
        const t_Ty* pContainer = static_cast<const t_Ty*>(a_pContainer);
        return (const void*)pContainer->data();
    }
};

} // namespace reflection
} // namespace phantom
