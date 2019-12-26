// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "Constructor.h"
#include "allocate.h"
#include "new.h"
#include "newImpl.h"
#include "phantom_priv.h"

#include <phantom/reflection/TypeOf.h>

HAUNT_STOP;

/// @cond ADVANCED

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM DeferredNewExH
{
    PHANTOM_FORCEINLINE DeferredNewExH(reflection::Class* a_pClass) : m_pClass(a_pClass)
    {
        PHANTOM_ASSERT(m_pClass);
    }
    template<typename t_Ty>
    PHANTOM_FORCEINLINE t_Ty* operator*(t_Ty* a_pInstance)
    {
        a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance = a_pInstance;
        a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass = m_pClass;
        m_pClass->registerInstance(a_pInstance);
        return a_pInstance;
    }
    reflection::Class* m_pClass;
};

template<typename t_Ty>
struct DeferredDeleteExH
{
    PHANTOM_FORCEINLINE DeferredDeleteExH(reflection::Class* a_pClass) : m_pClass(a_pClass)
    {
        PHANTOM_ASSERT(m_pClass);
    }
    PHANTOM_FORCEINLINE void operator*(t_Ty* a_pInstance)
    {
        m_pClass->unregisterInstance(a_pInstance);
        PHANTOM_ASSERT(a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance == a_pInstance);
        PHANTOM_ASSERT(a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass == m_pClass);
        Constructor<t_Ty>::destroy(a_pInstance);
        Allocator<t_Ty>::deallocate(a_pInstance);
    }
    reflection::Class* m_pClass;
};

} // namespace phantom

/// @endcond
