// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/reflection/ClassT.h>
#include <phantom/traits/ContainerTraits.h>

namespace phantom
{
namespace reflection
{
struct EraseOrAssert
{
    template<class Container, class It,
             class = std::enable_if_t<HasContainerErase<Container>::value, void>>
    static void Erase(Container* a_pContainer, It* a_It)
    {
        // if you get compilation error here, this means your container value type don't use sfinae
        // to remove its operator= depending on its content you can specialize IsContainerErasable
        // to std::false_type to ensure compilation succeeds
        a_pContainer->erase(*a_It);
    }
    template<class, class, class>
    static void Erase(void*, void const*)
    {
        PHANTOM_ASSERT(false, "push_back not available");
    }
};

template<typename t_Ty, typename Base = ContainerClass>
class StlContainerClassT : public ClassT<t_Ty, Base>
{
    using BaseType = ClassT<t_Ty, Base>;
    typedef PHANTOM_TYPENAME t_Ty::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME t_Ty::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME t_Ty::iterator ContainerIterator;

public:
    StlContainerClassT(StringView a_strName, Modifiers a_Modifiers = 0)
        : BaseType(a_strName, a_Modifiers)
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
        return &*it;
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
        EraseOrAssert::Erase<t_Ty, ContainerValueType, ContainerIterator>(pContainer, &it);
    }

    virtual void clear(void* a_pContainer) const override
    {
        static_cast<t_Ty*>(a_pContainer)->clear();
    }

    virtual void begin(void* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) = static_cast<t_Ty*>(a_pContainer)->begin();
    }

    virtual void begin(void const* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerConstIterator*>(a_pOutIt) =
        static_cast<t_Ty const*>(a_pContainer)->begin();
    }

    virtual void end(void* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerIterator*>(a_pOutIt) = static_cast<t_Ty*>(a_pContainer)->end();
    }

    virtual void end(void const* a_pContainer, void* a_pOutIt) const override
    {
        *reinterpret_cast<ContainerConstIterator*>(a_pOutIt) =
        static_cast<t_Ty const*>(a_pContainer)->end();
    }

    void* dereferenceIterator(void* a_pIt) const override
    {
        return &*reinterpret_cast<ContainerIterator*>(a_pIt);
    }

    void advanceIterator(void* a_pIt, size_t a_N = 1) const override
    {
        ContainerIterator& it = *reinterpret_cast<ContainerIterator*>(a_pIt);
        std::advance(it, a_N);
    }

    bool compareIterators(void* a_pIt1, void* a_pIt2) const override
    {
        return *reinterpret_cast<ContainerIterator*>(a_pIt1) ==
        *reinterpret_cast<ContainerIterator*>(a_pIt2);
    }

    Type* getIteratorType() const override
    {
        return PHANTOM_TYPEOF(ContainerIterator);
    }
    Type* getConstIteratorType() const override
    {
        return PHANTOM_TYPEOF(ContainerConstIterator);
    }
};
} // namespace reflection
} // namespace phantom
