// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/ClassOf.h>
#include <phantom/reflection/TypeOf.h>
#include <phantom/traits/HasEmbeddedRtti.h>

HAUNT_STOP;

namespace phantom
{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Home made dynamic cast using reflection.
////////////////////////////////////////////////////////////////////////////////////////////////////

PHANTOM_EXPORT_PHANTOM void* dyn_cast(reflection::Class* a_pClass, void* a_pPointer);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Home made dynamic cast using mapped or nested EmbeddedRtti to resolve pointer offsets.
/// If any in or out type is not a pointer
///         the function fallback to a classic c-style cast, accounting on copy/conversion
///         constructor/operators to do the job.
///
/// \return the t_In 'in' converted into the t_Out type.
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename t_TargetPtr, typename t_Source>
PHANTOM_FORCEINLINE t_TargetPtr dyn_cast(t_Source* in);

namespace detail
{
template<typename t_Target, typename t_Source, bool t_SourceHasEmbeddedRtti>
struct dyn_castH
{
    PHANTOM_STATIC_ASSERT(std::is_class<t_Source>::value);
    PHANTOM_STATIC_ASSERT(std::is_class<t_Target>::value);
    static t_Target* apply(t_Source* a_pSource)
    {
        if (a_pSource == nullptr)
            return nullptr;
        return reinterpret_cast<t_Target*>(
        a_pSource->RTTI.metaClass->upcast(PHANTOM_CLASSOF(t_Target), a_pSource->RTTI.instance));
    }
};

template<typename t_Target, typename t_Source>
struct dyn_castH<t_Target, t_Source, false>
{
    PHANTOM_STATIC_ASSERT(std::is_void<t_Source>::value || std::is_class<t_Source>::value);
    PHANTOM_STATIC_ASSERT(std::is_class<t_Target>::value);
    static t_Target* apply(t_Source* a_pSource)
    {
        return nullptr;
    }
};

} // namespace detail

template<typename t_TargetPtr, typename t_Source>
PHANTOM_FORCEINLINE t_TargetPtr dyn_cast(t_Source* a_pSource)
{
    using SourceNoQual = PHANTOM_TYPENAME std::remove_cv<t_Source>::type;
    PHANTOM_STATIC_ASSERT(IsDataPointer<t_TargetPtr>::value);
    using TargetNoQual = PHANTOM_TYPENAME std::remove_cv<PHANTOM_TYPENAME std::remove_pointer<t_TargetPtr>::type>::type;
    return (TargetNoQual*)detail::dyn_castH < TargetNoQual, SourceNoQual,
           HasEmbeddedRtti<SourceNoQual>::value ||
           HasEmbeddedProxyRtti<SourceNoQual>::value > ::apply((SourceNoQual*)a_pSource);
}
} // namespace phantom
