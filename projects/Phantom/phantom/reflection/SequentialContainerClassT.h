// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/newImpl.h>
#include <phantom/reflection/SequentialContainerClass.h>
#include <phantom/reflection/StlContainerClassT.h>

namespace phantom
{
namespace reflection
{
struct PushBackOrAssert
{
    template<class Container, class ValueType,
             class = std::enable_if_t<phantom::HasContainerPushBack<Container>::value, void>>
    static void PushBack(Container* a_pContainer, ValueType const* a_pValue)
    {
        a_pContainer->push_back(*a_pValue);
    }
    template<class, class>
    static void PushBack(void*, void const*)
    {
        PHANTOM_ASSERT(false, "push_back not available");
    }
};

template<typename t_Ty, typename Base = SequentialContainerClass>
class SequentialContainerClassT : public StlContainerClassT<t_Ty, Base>
{
    using SelfType = SequentialContainerClassT<t_Ty, Base>;
    using BaseType = StlContainerClassT<t_Ty, Base>;

    typedef t_Ty             ContainerType;
    typedef PHANTOM_TYPENAME t_Ty::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME t_Ty::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME t_Ty::iterator ContainerIterator;

public:
    SequentialContainerClassT(StringView a_strName, Modifiers a_Modifiers = 0)
        : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    virtual void push_back(void* a_pContainer, void const* a_pValue) const override
    {
        t_Ty*                     pContainer = static_cast<t_Ty*>(a_pContainer);
        ContainerValueType const* pValue = static_cast<ContainerValueType const*>(a_pValue);
        PushBackOrAssert::PushBack<ContainerType, ContainerValueType>(pContainer, pValue);
    }
};

} // namespace reflection
} // namespace phantom
