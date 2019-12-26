// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "ObjectImpl.h"

#include <phantom/traits/HasEmbeddedRtti.h>

HAUNT_STOP;

namespace phantom
{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Home made dynamic cast using internal EmbeddedRtti to resolve pointer offsets. If any in
/// or out type is not a pointer
///         the function fallback to a classic c-style cast, accounting on copy/conversion
///         constructor/operators to do the job.
///
/// \return the t_In 'in' converted into the t_Out type.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename t_TargetPtr, typename t_Source>
PHANTOM_FORCEINLINE t_TargetPtr as(t_Source* in);

namespace detail
{
template<class t_Ty, bool t_SourceHasEmbeddedRtti>
struct mostDerivedOfH
{
    PHANTOM_STATIC_ASSERT(std::is_class<t_Ty>::value);
    static Object apply(t_Ty* a_pThis)
    {
        return Object(a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance,
                      a_pThis->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass);
    }
};

template<class t_Ty>
struct mostDerivedOfH<t_Ty, false>
{
    static Object apply(void* a_pThis);
};

} // namespace detail

template<typename t_Ty>
PHANTOM_FORCEINLINE Object mostDerivedOf(t_Ty* a_pThis)
{
    return detail::mostDerivedOfH < t_Ty,
           HasEmbeddedRtti<t_Ty>::value || HasEmbeddedProxyRtti<t_Ty>::value > ::apply(a_pThis);
}

template<typename t_Ty>
PHANTOM_FORCEINLINE Object mostDerivedOf(void* a_pThis)
{
    return detail::mostDerivedOfH<t_Ty, false>::apply(a_pThis);
}

} // namespace phantom
