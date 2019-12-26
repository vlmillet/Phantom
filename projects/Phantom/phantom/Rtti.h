// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "RttiImpl.h"

HAUNT_STOP;

#include "ClassOf.h"
#include "RttiMapData.h"
#include "traits/SelectType.h"

#include <phantom/reflection/TypeOf.h>
#include <phantom/traits/HasEmbeddedRtti.h>

namespace phantom
{
namespace detail
{
template<class T>
struct RttiReturn
    : SelectType<HasEmbeddedRtti<T>::value || HasEmbeddedProxyRtti<T>::value, RttiMapData, const RttiMapData&>
{
};

template<class T, bool Embedded, bool Proxy>
struct RttiH;

template<class T>
struct RttiH<T, true, false>
{
    static RttiMapData apply(T const* a_pThis)
    {
        return RttiMapData(a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass, PHANTOM_CLASSOF(T),
                           a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance);
    }
};

template<class T>
struct RttiH<T, false, true>
{
    static RttiMapData apply(T const* a_pThis)
    {
        return RttiMapData(a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass, PHANTOM_CLASSOF(T),
                           a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance,
                           a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.dynamicDeleteFunc);
    }
};

template<class T>
struct RttiH<T, false, false>
{
    static RttiMapData const& apply(T const* a_pThis)
    {
        return Rtti::Find((void const*)a_pThis);
    }
};
} // namespace detail

template<class T>
inline typename detail::RttiReturn<T>::type Rtti::Find(T const* a_pThis)
{
    return detail::RttiH<T, HasEmbeddedRtti<T>::value, HasEmbeddedProxyRtti<T>::value>::apply(a_pThis);
}

template<class T>
inline void Rtti::Map(T const* a_pThis)
{
    Rtti::Map(a_pThis, PHANTOM_CLASSOF(T));
}

template<class T>
reflection::Class* Rtti::ClassOf(T const* a_pThis)
{
    return Find(a_pThis).getObjectClass();
}

template<class T>
void const* Rtti::MostDerivedOf(T const* a_pThis)
{
    return (void const*)Find(a_pThis).getObjectAddress();
}
template<class T>
void* Rtti::MostDerivedOf(T* a_pThis)
{
    return (void*)Find(a_pThis).getObjectAddress();
}

} // namespace phantom
