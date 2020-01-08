// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/detail/newImpl.h>
#include <phantom/reflection/SequentialContainerClass.h>
#include <phantom/reflection/ContainerClassT.h>

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
	template<class Container, class ValueType,
		class = std::enable_if_t<phantom::HasContainerPushBack<Container>::value, void>>
		static void PushBack(Container* a_pContainer, ValueType* a_pValue)
	{
		a_pContainer->push_back(std::move(*a_pValue));
	}
	template<class, class>
	static void PushBack(void*, void*)
	{
		PHANTOM_ASSERT(false, "push_back not available");
	}
};

template<typename T, typename Base = SequentialContainerClass>
class SequentialContainerClassT : public ContainerClassT<T, Base>
{
    using SelfType = SequentialContainerClassT<T, Base>;
    using BaseType = ContainerClassT<T, Base>;

    typedef T             ContainerType;
    typedef PHANTOM_TYPENAME T::value_type ContainerValueType;
    typedef PHANTOM_TYPENAME T::const_iterator ContainerConstIterator;
    typedef PHANTOM_TYPENAME T::iterator ContainerIterator;

public:
    SequentialContainerClassT(StringView a_strName, Modifiers a_Modifiers = 0) : BaseType(a_strName, a_Modifiers)
    {
        this->setValueType(PHANTOM_TYPEOF(ContainerValueType));
    }

    virtual void push_back(void* a_pContainer, void const* a_pValue) const override
    {
        T*                     pContainer = static_cast<T*>(a_pContainer);
        ContainerValueType const* pValue = static_cast<ContainerValueType const*>(a_pValue);
        PushBackOrAssert::PushBack<ContainerType, ContainerValueType>(pContainer, pValue);
	}

	void push_back(void* a_pContainer, MoveArg a_pValue) const override
	{
		T*                     pContainer = static_cast<T*>(a_pContainer);
		ContainerValueType* pValue = static_cast<ContainerValueType*>(a_pValue.operator void*());
		PushBackOrAssert::PushBack<ContainerType, ContainerValueType>(pContainer, pValue);
	}

	void pop_back(void* a_pContainer) const override
	{
		T* pContainer = static_cast<T*>(a_pContainer);
		pContainer->pop_back();
	}
};

} // namespace reflection
} // namespace phantom
