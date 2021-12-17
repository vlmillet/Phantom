// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/ArrayClass.h>
#include <phantom/lang/ClassT.h>

namespace phantom
{
namespace lang
{
template<typename T, size_t Count, class Base = ClassT<T, ArrayClass>>
class ArrayClassT : public Base
{
    using SelfType = ArrayClassT<T, Count, Base>;
    using BaseType = ClassT<T, ArrayClass>;
    typedef T                                   ContainerType;
    using ContainerValueType = PHANTOM_TYPENAME ContainerType::value_type;
    using ContainerConstIterator = ContainerValueType const*;
    using ContainerIterator = ContainerValueType*;

public:
    ArrayClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setItemType(PHANTOM_TYPEOF(ContainerValueType));
        this->setItemCount(Count);
    }

    void setItemValue(void* a_pContainer, size_t a_uiIndex, void const* a_pSrc) override
    {
        T* pContainer = reinterpret_cast<T*>(a_pContainer);
        (*pContainer)[a_uiIndex] = *reinterpret_cast<ContainerValueType const*>(a_pSrc);
    }

    void getItemValue(void const* a_pContainer, size_t a_uiIndex, void* a_pDst) override
    {
        T const* pContainer = reinterpret_cast<T const*>(a_pContainer);
        *reinterpret_cast<ContainerValueType*>(a_pDst) = (*pContainer)[a_uiIndex];
    }

    void* data(void* a_pContainer) const override
    {
        T* pContainer = reinterpret_cast<T*>(a_pContainer);
        return (void*)pContainer->data();
    }

    const void* data(const void* a_pContainer) const override
    {
        const T* pContainer = reinterpret_cast<const T*>(a_pContainer);
        return (const void*)pContainer->data();
    }

    virtual size_t size(void const* a_pContainer) const override
    {
        T const* container = reinterpret_cast<T const*>(a_pContainer);
        return container->size();
    }

    virtual void* referenceAt(void* a_pContainer, size_t a_uiIndex) const override
    {
        T* pContainer = reinterpret_cast<T*>(a_pContainer);
        return &(*pContainer)[a_uiIndex];
    }

    virtual void const* referenceAt(void const* a_pContainer, size_t a_uiIndex) const override
    {
        T const* pContainer = reinterpret_cast<T const*>(a_pContainer);
        return &(*pContainer)[a_uiIndex];
    }

    virtual void begin(void* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) = reinterpret_cast<T*>(a_pContainer)->begin();
    }

    virtual void begin(void const* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerConstIterator*>(a_pOutIt) = reinterpret_cast<T const*>(a_pContainer)->begin();
    }

    virtual void end(void* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) = reinterpret_cast<T*>(a_pContainer)->end();
    }

    virtual void end(void const* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerConstIterator*>(a_pOutIt) = reinterpret_cast<T const*>(a_pContainer)->end();
    }

    void* dereferenceIterator(void* a_pIt) const override { return &*reinterpret_cast<ContainerIterator*>(a_pIt); }

    void advanceIterator(void* a_pIt, size_t a_N = 1) const override
    {
        ContainerIterator& it = *reinterpret_cast<ContainerIterator*>(a_pIt);
        std::advance(it, a_N);
    }

    bool compareIterators(void* a_pIt1, void* a_pIt2) const override
    {
        return *reinterpret_cast<ContainerIterator*>(a_pIt1) == *reinterpret_cast<ContainerIterator*>(a_pIt2);
    }

    Type* getIteratorType() const override { return PHANTOM_TYPEOF(ContainerIterator); }
    Type* getConstIteratorType() const override { return PHANTOM_TYPEOF(ContainerConstIterator); }
};

} // namespace lang
} // namespace phantom
