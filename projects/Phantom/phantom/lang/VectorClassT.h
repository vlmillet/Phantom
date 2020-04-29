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
    typedef T                ContainerType;
    typedef PHANTOM_TYPENAME T::value_type ContainerValueType;

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
};

} // namespace lang
} // namespace phantom
