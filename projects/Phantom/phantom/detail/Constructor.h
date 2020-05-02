// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <phantom/detail/ConstructorCaller.h>
#include <phantom/detail/core.h>
#include <phantom/traits/CtorDtor.h>
#include <phantom/traits/HasVirtualDestructor.h>
#include <phantom/traits/IsDefaultConstructible.h>

namespace phantom
{
/// @cond INTERNAL

struct ConstructorOverloadTag
{
    enum Enum
    {
        Default,
        MetaElement,
        ClassGroup0,
        ClassGroup1,
    };
};

/* constructor */
namespace detail
{
enum class ConstructorSelectorId
{
    Default,
    NoDefault,
    Abstract,
    Fundamental,
};

template<typename, ConstructorSelectorId>
struct ConstructorH;

template<typename t_Ty>
struct ConstructorH<t_Ty, ConstructorSelectorId::Abstract>
{
public:
    PHANTOM_FORCEINLINE static void construct(t_Ty* a_pInstance)
    {
        PHANTOM_UNUSED(a_pInstance);
        PHANTOM_ASSERT(false, "type is abstract and cannot be constructed/destroyed");
    }
    PHANTOM_FORCEINLINE static void destroy(t_Ty* a_pInstance)
    {
        PHANTOM_UNUSED(a_pInstance);
        PHANTOM_ASSERT(false, "type is abstract and cannot be constructed/destroyed");
    }
};

template<typename t_Ty>
struct ConstructorH<t_Ty, ConstructorSelectorId::Fundamental>
{
public:
    PHANTOM_FORCEINLINE static void construct(t_Ty* a_pInstance) { *a_pInstance = t_Ty(); }
    PHANTOM_FORCEINLINE static void destroy(t_Ty*) {}
};

template<typename t_Ty>
struct ConstructorH<t_Ty, ConstructorSelectorId::NoDefault>
{
public:
    PHANTOM_FORCEINLINE static void construct(t_Ty* a_pInstance)
    {
        PHANTOM_UNUSED(a_pInstance);
        PHANTOM_ERROR(false, "type hasn't default constructor and cannot be constructed");
    }
    PHANTOM_FORCEINLINE static void destroy(t_Ty* a_pInstance) { Dtor<t_Ty>::apply(a_pInstance); }
};

template<typename t_Ty>
struct ConstructorH<t_Ty, ConstructorSelectorId::Default>
{
public:
    PHANTOM_FORCEINLINE static void construct(t_Ty* a_pInstance) { Ctor<t_Ty>::apply(a_pInstance); }
    PHANTOM_FORCEINLINE static void destroy(t_Ty* a_pInstance) { Dtor<t_Ty>::apply(a_pInstance); }
};

} // namespace detail

/// @endcond

template<typename t_Ty>
struct Constructor
    : public detail::ConstructorH<t_Ty,
                                  std::is_abstract<t_Ty>::value
                                  ? phantom::HasVirtualDestructor<t_Ty>::value
                                  ? detail::ConstructorSelectorId::NoDefault
                                  : detail::ConstructorSelectorId::Abstract // abstract
                                  : std::is_class<t_Ty>::value ? !(IsPublicOrProtectedDefaultConstructible<t_Ty>::value)
                                  ? detail::ConstructorSelectorId::NoDefault // no default constructor
                                  : detail::ConstructorSelectorId::Default
                                                               : detail::ConstructorSelectorId::Fundamental>
{
    template<typename t_OtherTy>
    struct rebind
    {
        typedef Constructor<t_OtherTy> other;
    };
};

} // namespace phantom
