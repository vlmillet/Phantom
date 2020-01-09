// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include "NativeVTableInspector.h"

#include <phantom/lang/Class.h>
#include <type_traits>
/* *********************************************** */

#define PHANTOM_ACCURATE_NATIVE_VTABLE_INSPECTOR_SIGNATURES 0

namespace phantom
{
namespace lang
{
template<typename t_Ty, typename t_Signature>
struct NativeVTableSizeComputer;

#if PHANTOM_ACCURATE_NATIVE_VTABLE_INSPECTOR_SIGNATURES
#    define VTableInspectorT_X VTableInspectorT<void(void)>()
#else
#    define VTableInspectorT_X VTableInspectorGeneric()
#endif

template<typename t_Ty, bool t_IsFinal, typename... Args>
struct NativeVTableSizeComputerH;

template<typename t_Ty, typename... Args>
struct NativeVTableSizeComputerH<t_Ty, false, Args...> : public t_Ty
{
    PHANTOM_STATIC_ASSERT(std::is_polymorphic<t_Ty>::value, "t_Ty must be polymorphic");
    NativeVTableSizeComputerH()
        : t_Ty(
          (*(typename std::remove_reference<typename std::remove_cv<Args>::type>::type*)nullptr)...)
    {
    }
    virtual void _PHNTM_VTableSizeComputer()
    {
    }
    static size_t apply()
    {
        return VTableInspectorT_X.getIndexOf(
        &NativeVTableSizeComputerH<t_Ty, false, void(Args...)>::_PHNTM_VTableSizeComputer);
    }
};

template<typename t_Ty, typename... Args>
struct NativeVTableSizeComputerH<t_Ty, true, Args...>
{
    virtual void _PHNTM_VTableSizeComputer()
    {
    }
    static size_t apply()
    {
        return ~size_t(0);
    }
};

template<typename t_Ty, typename... Args>
struct NativeVTableSizeComputer<t_Ty, void(Args...)>
    : public NativeVTableSizeComputerH<t_Ty, std::is_final<t_Ty>::value, Args...>
{
};

namespace detail
{
template<typename t_Ty, bool t_is_default_constructible>
struct VTableSizeOfDefaultConstructibleH
{
    static size_t apply(const void*)
    {
        return NativeVTableSizeComputer<t_Ty, void()>::apply();
    }
};

template<typename t_Ty>
struct VTableSizeOfDefaultConstructibleH<t_Ty, false>
{
    static size_t apply(const Class* a_pClass)
    {
        auto& ctors = a_pClass->getConstructors();
        if (ctors.size())
            return ctors.front()->getNativeVTableSize();
        // PHANTOM_WARNING(false, "No constructor declared in lang of %s, cannot extract
        // vtable information", a_pClass->getQualifiedDecoratedName().c_str());
        return 0;
    };
};

template<typename t_Ty, bool t_is_polymorphic>
struct VTableSizeOfH
    : public VTableSizeOfDefaultConstructibleH<
      t_Ty, ::phantom::IsDefaultConstructible<t_Ty>::value AND !std::is_abstract<t_Ty>::value>
{
};

template<typename t_Ty>
struct VTableSizeOfH<t_Ty, false>
{
    static size_t apply(const void*)
    {
        return 0;
    }
};
}; // namespace detail

template<typename t_Ty>
struct VTableSizeOf : detail::VTableSizeOfH<t_Ty, std::is_polymorphic<t_Ty>::value>
{
};

} // namespace lang
} // namespace phantom

namespace phantom
{
template<typename t_Ty>
size_t virtualMethodCountOf()
{
    return phantom::lang::VTableSizeOf<t_Ty>::apply(PHANTOM_PRECISE_TYPEOF(t_Ty));
}
} // namespace phantom

#undef VTableInspectorT_X

/// @endcond
