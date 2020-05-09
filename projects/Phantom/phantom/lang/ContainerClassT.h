// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/ClassT.h>
#include <phantom/traits/ContainerTraits.h>

namespace phantom
{
namespace lang
{
template<class Container, class It, bool HasErase>
struct _EraseOrAssert
{
    static void Erase(Container* a_pContainer, It const* a_It)
    {
        // if you get compilation error here, this means your container value type don't use sfinae
        // to remove its operator= depending on its content you can specialize IsContainerErasable
        // to std::false_type to ensure compilation succeeds
        a_pContainer->erase(*a_It);
    }
};
template<class Container, class It>
struct _EraseOrAssert<Container, It, false>
{
    static void Erase(Container* a_pContainer, It const* a_It) { PHANTOM_ASSERT(false, "push_back not available"); }
};

template<class Container, class It>
struct EraseOrAssert : _EraseOrAssert<Container, It, HasContainerErase<Container>::value>
{
};

template<typename t_Ty, typename Base = ContainerClass>
class ContainerClassT : public ClassT<t_Ty, Base>
{
    using BaseType = ClassT<t_Ty, Base>;
    typedef PHANTOM_TYPENAME t_Ty::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME t_Ty::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME t_Ty::iterator ContainerIterator;

public:
    ContainerClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    virtual size_t size(void const* a_pContainer) const override
    {
        t_Ty const* container = static_cast<t_Ty const*>(a_pContainer);
        return container->size();
    }

    virtual void* referenceAt(void* a_pContainer, size_t a_uiIndex) const override
    {
        t_Ty* pContainer = static_cast<t_Ty*>(a_pContainer);
        auto  it = pContainer->begin();
        std::advance(it, a_uiIndex);
        return (void*)&*it;
    }

    virtual void const* referenceAt(void const* a_pContainer, size_t a_uiIndex) const override
    {
        t_Ty const* pContainer = static_cast<t_Ty const*>(a_pContainer);
        auto        it = pContainer->begin();
        std::advance(it, a_uiIndex);
        return &*it;
    }

    virtual void eraseAt(void* a_pContainer, size_t a_uiIndex) const override
    {
        t_Ty* pContainer = static_cast<t_Ty*>(a_pContainer);
        auto  it = pContainer->begin();
        std::advance(it, a_uiIndex);
        EraseOrAssert<t_Ty, ContainerIterator>::Erase(pContainer, &it);
    }

    virtual void erase(void* a_pContainer, void const* a_pIt) const override
    {
        t_Ty*                    pContainer = static_cast<t_Ty*>(a_pContainer);
        ContainerIterator const* pIt = reinterpret_cast<ContainerIterator const*>(a_pIt);
        EraseOrAssert<t_Ty, ContainerIterator>::Erase(pContainer, pIt);
    }

    virtual void clear(void* a_pContainer) const override { static_cast<t_Ty*>(a_pContainer)->clear(); }

    virtual void begin(void* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) = static_cast<t_Ty*>(a_pContainer)->begin();
    }

    virtual void begin(void const* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerConstIterator*>(a_pOutIt) = static_cast<t_Ty const*>(a_pContainer)->begin();
    }

    virtual void end(void* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) = static_cast<t_Ty*>(a_pContainer)->end();
    }

    virtual void end(void const* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerConstIterator*>(a_pOutIt) = static_cast<t_Ty const*>(a_pContainer)->end();
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
