// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include <phantom/utils/Object.h>

// --------------------------------------------------------------------------------------------
// this file contains macro helper for instancing meta elements avoiding reflection recursivity
// --------------------------------------------------------------------------------------------

/// @cond ADVANCED

#define PHANTOM_META_NEW(...) phantom::NewMetaH<__VA_ARGS__>() >> new (phantom::allocate(sizeof(__VA_ARGS__), PHANTOM_ALIGNOF(__VA_ARGS__))) __VA_ARGS__

#define PHANTOM_META_DELETE(...) phantom::DeleteMetaH<__VA_ARGS__>() >>

namespace phantom
{
template<typename t_Ty>
struct NewMetaH
{
    PHANTOM_FORCEINLINE NewMetaH() {}
    PHANTOM_FORCEINLINE t_Ty* operator>>(t_Ty* a_pInstance)
    {
        auto pMetaClass = t_Ty::MetaClass();
        a_pInstance->rtti.instance = a_pInstance;
        a_pInstance->rtti.customDeleteFunc = &DynamicDeleteMetaHelper<t_Ty>::dynamicDelete;
        a_pInstance->rtti.metaClass = pMetaClass;
        pMetaClass->registerInstance(a_pInstance);
        return a_pInstance;
    }
};

template<typename t_Ty>
struct DeleteMetaH
{
    template<typename _Tyy>
    PHANTOM_FORCEINLINE void operator>>(_Tyy* a_pInstance)
    {
        auto pMetaClass = t_Ty::MetaClass();
        pMetaClass->unregisterInstance(a_pInstance);
        PHANTOM_ASSERT(a_pInstance->rtti.instance == a_pInstance);
        PHANTOM_ASSERT(a_pInstance->rtti.customDeleteFunc == &DynamicDeleteMetaHelper<t_Ty>::dynamicDelete);
        PHANTOM_ASSERT(a_pInstance->rtti.metaClass == pMetaClass);
#if PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif
        Constructor<t_Ty>::destroy(static_cast<t_Ty*>(a_pInstance));

#if PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    pragma clang diagnostic pop
#endif
        phantom::deallocate(a_pInstance);
    }
};

template<typename t_Ty>
struct DynamicDeleteMetaHelper
{
    PHANTOM_FORCEINLINE static void dynamicDelete(void* a_pBase)
    {
        PHANTOM_META_DELETE(t_Ty) reinterpret_cast<t_Ty*>(a_pBase);
    }
};
} // namespace phantom

/// @endcond
