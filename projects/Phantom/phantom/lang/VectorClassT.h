// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/SequentialContainerClassT.h>
#include <phantom/lang/VectorClass.h>

namespace phantom
{
namespace lang
{
template<typename T, class Base = VectorClass>
class VectorClassT : public SequentialContainerClassT<T, Base>
{
    using SelfType = VectorClassT<T, Base>;
    using BaseType = SequentialContainerClassT<T, Base>;
    typedef T                                       ContainerType;
    using ContainerConstIterator = PHANTOM_TYPENAME ContainerType::const_iterator;
    using ContainerIterator = PHANTOM_TYPENAME      ContainerType::iterator;
    using ContainerValueType = PHANTOM_TYPENAME     ContainerType::value_type;

public:
    VectorClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    void* data(void* a_pContainer) const override
    {
        T* pContainer = static_cast<T*>(a_pContainer);
        return (void*)pContainer->data();
    }

    const void* data(const void* a_pContainer) const override
    {
        const T* pContainer = static_cast<const T*>(a_pContainer);
        return (const void*)pContainer->data();
    }
    void insert(void* a_pContainer, void const* a_pIt, void const* a_pValue, void* a_pOutIt) const override
    {
        ContainerType* pContainer = static_cast<ContainerType*>(a_pContainer);
        auto           pIt = reinterpret_cast<ContainerIterator const*>(a_pIt);
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) =
        pContainer->insert(*pIt, *reinterpret_cast<ContainerValueType const*>(a_pValue));
    }
};

} // namespace lang
} // namespace phantom
