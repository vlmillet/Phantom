// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "Constructor.h"

#include <phantom/detail/TypeOfFwd.h>
#include <phantom/utils/Object.h>

HAUNT_STOP;
namespace phantom
{
namespace detail
{
template<class t_Ty, bool t_IsObject>
struct NewHH;

// Embedded rtti

template<class t_Ty>
struct NewHH<t_Ty, true>
{
    PHANTOM_FORCEINLINE static t_Ty* apply(t_Ty* a_pInstance)
    {
        a_pInstance->rtti.instance = a_pInstance;
        a_pInstance->rtti.metaClass = (reflection::Class*)TypeOf<t_Ty>(TypeOfTag<t_Ty>());
        a_pInstance->rtti.metaClass->registerInstance(a_pInstance);
        return a_pInstance;
    }
};

// Nothing special (default C++ behavior)

template<class t_Ty>
struct NewHH<t_Ty, false>
{
    PHANTOM_FORCEINLINE static t_Ty* apply(t_Ty* a_pInstance) { return a_pInstance; }
};

template<class t_Ty>
struct NewH
{
    PHANTOM_FORCEINLINE t_Ty* operator*(t_Ty* a_pInstance)
    {
        return NewHH<t_Ty, IsObject<t_Ty>::value>::apply(a_pInstance);
    }
};

template<bool StaticChecks, class t_Ty, bool t_IsObject>
struct PlacementDeleteHH;

// Embedded rtti

template<bool StaticChecks, class t_Ty>
struct PlacementDeleteHH<StaticChecks, t_Ty, true>
{
    static void apply(t_Ty* a_pInstance);
};

// Nothing (default C++ behavior)

template<bool StaticChecks, class t_Ty>
struct PlacementDeleteHH<StaticChecks, t_Ty, false>
{
    static_assert(!StaticChecks || !std::has_virtual_destructor<t_Ty>::value,
                  "calling 'static' delete on a class with virtual destructor is forbidden");
    PHANTOM_FORCEINLINE static void apply(t_Ty* a_pInstance) { Constructor<t_Ty>::destroy(a_pInstance); }
};

template<bool StaticChecks, typename t_Ty>
struct DeleteH
{
    PHANTOM_FORCEINLINE void operator*(t_Ty* a_pInstance)
    {
        if (a_pInstance)
        {
            PlacementDeleteHH<StaticChecks, t_Ty, IsObject<t_Ty>::value>::apply(a_pInstance);
            phantom::deallocate(a_pInstance);
        }
    }
};

template<typename t_Ty>
struct PlacementDeleteH
{
    PHANTOM_FORCEINLINE void operator*(t_Ty* a_pInstance)
    {
        PlacementDeleteHH<true, t_Ty, IsObject<t_Ty>::value>::apply(a_pInstance);
    }
};

template<typename t_Ty>
struct NewNH
{
    PHANTOM_FORCEINLINE NewNH(size_t N) : N(N)
    {
    }
    PHANTOM_FORCEINLINE t_Ty* operator*(t_Ty* a_pInstances)
    {
        t_Ty* pInstance = a_pInstances;
        while (N--)
        {
            NewH<t_Ty>() * new (pInstance) t_Ty;
            pInstance++;
        }
        return a_pInstances;
    }
    size_t N;
};

template<typename t_Ty>
struct DeleteNH
{
    PHANTOM_FORCEINLINE      DeleteNH(size_t N) : N(N) {}
    PHANTOM_FORCEINLINE void operator*(t_Ty* a_pInstances)
    {
        t_Ty*  pInstance = a_pInstances;
        size_t i = N;
        while (i--)
        {
            DeleteH<true, t_Ty>() * pInstance;
            pInstance->~t_Ty();
            pInstance++;
        }
        Allocator<t_Ty>::deallocate(a_pInstances, N);
    }
    size_t N;
};

template<class t_Ty, bool t_has_embed_rtti>
struct DeleteDynHH
{
    static_assert(std::is_same<t_Ty, t_Ty*>::value, "invalid case of dynamic deletion");
};

// ProxyRtti case
template<class t_Ty>
struct DeleteDynHH<t_Ty, true>
{
    static void apply(t_Ty* a_pInstance)
    {
        auto class_ = a_pInstance->rtti.metaClass;
        auto ptr_ = a_pInstance->rtti.instance;
        if (auto dfunc = a_pInstance->rtti.customDeleteFunc)
        {
            dfunc(ptr_);
        }
        else
        {
            class_->unregisterInstance(a_pInstance);
            class_->deleteInstance(a_pInstance);
        }
    }
};

struct DeleteDynH
{
    template<class t_Ty>
    void operator*(t_Ty* a_pInstance)
    {
        if (a_pInstance)
        {
            DeleteDynHH<t_Ty, IsObject<t_Ty>::value>::apply(a_pInstance);
        }
    }
};

struct VirtualDeleteH
{
    template<class t_Ty>
    void operator*(t_Ty* a_pInstance)
    {
        if (a_pInstance)
            a_pInstance->PHANTOM_CUSTOM_VIRTUAL_DELETE();
    }
};

template<typename t_Ty>
struct StackNewH
{
    PHANTOM_FORCEINLINE StackNewH(t_Ty* a_pInstance) : m_pInstance(a_pInstance) { NewH<t_Ty>() * a_pInstance; }
    PHANTOM_FORCEINLINE ~StackNewH() { PlacementDeleteH<t_Ty>() * m_pInstance; }
    t_Ty* m_pInstance;
};

template<typename t_Ty>
struct DynamicDeleter
{
    PHANTOM_FORCEINLINE static void dynamicDelete(void* a_pInstance)
    {
        PlacementDeleteHH<false, t_Ty, IsObject<t_Ty>::value>::apply(reinterpret_cast<t_Ty*>(a_pInstance));
        Allocator<t_Ty>::deallocate(reinterpret_cast<t_Ty*>(a_pInstance));
    }
};
} // namespace detail

template<class T, class ... Args>
T* New(Args&&... a_Args)
{
	return detail::NewH<T>() * new (allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
}

template<class T, class... Args>
T* PlacementNew(T* a_pObj, Args&&... a_Args)
{
	return detail::NewH<T>() * new (a_pObj) T(std::forward<Args>(a_Args)...);
}

template<class T, bool Checks = true>
void Delete(T*&& a_pPtr)
{
	return detail::DeleteH<Checks, T>() * a_pPtr;
}

template<class T, bool Checks = true>
void Delete(T*& a_pPtr)
{
	return detail::DeleteH<Checks, T>() * a_pPtr;
}

template<class T>
void DeleteAuto(T* a_pObj)
{
	detail::DeleteDynH()* a_pObj;
}

} // namespace phantom
