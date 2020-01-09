// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */

#include <phantom/reflection/FunctionT.h>
#include <phantom/reflection/MetaNewDelete.h>
#include <phantom/reflection/SignatureH.h>

/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
template<class Sign>
struct StaticMethodPointerSimplifier;

template<typename R, class... Ps>
struct StaticMethodPointerSimplifier<R(Ps...)>
{
    typedef R (*SourcePtr)(Ps...);
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }

#if PHANTOM_HAS_STDCALL
    typedef R(PHANTOM_CALLCONV(stdcall) * StdCallSourcePtr)(Ps...);
    typedef SimplifiedTypeT<StdCallSourcePtr> StdCallSimplifiedPtr;
    static StdCallSimplifiedPtr               Simplify(StdCallSourcePtr p)
    {
        return (StdCallSimplifiedPtr)p;
    }
#endif

#if PHANTOM_HAS_FASTCALL
    typedef R(PHANTOM_CALLCONV(fastcall) * FastCallSourcePtr)(Ps...);
    typedef SimplifiedTypeT<FastCallSourcePtr> FastCallSimplifiedPtr;
    static FastCallSimplifiedPtr               Simplify(FastCallSourcePtr p)
    {
        return (FastCallSimplifiedPtr)p;
    }
#endif
};

template<typename R, class... Ps>
struct StaticMethodPointerSimplifier<R(Ps..., ...)>
{
    typedef R (*SourcePtr)(Ps..., ...);
    typedef SimplifiedTypeT<SourcePtr> SimplifiedPtr;
    static SimplifiedPtr               Simplify(SourcePtr p)
    {
        return (SimplifiedPtr)p;
    }
};

#if defined(cdecl)
#    define _PHNTM_CDECL_WAS_DEFINED
#    pragma push_macro("cdecl")
#    undef cdecl
#endif

template<typename t_Signature>
class FunctionProviderT;

template<typename t_ReturnType, class... v_Params>
class FunctionProviderT<t_ReturnType (*)(v_Params...)> final
{
public:
#if PHANTOM_HAS_STDCALL
    static Function* CreateFunction(StringView a_strName, Signature* a_pSignature,
                                    t_ReturnType(PHANTOM_CALLCONV(stdcall) * a_pFunc)(v_Params...),
                                    Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
    {
        auto sptr = StaticMethodPointerSimplifier<t_ReturnType(v_Params...)>::Simplify(a_pFunc);
        return PHANTOM_META_NEW(FunctionT<decltype(sptr)>)(a_strName, a_pSignature, sptr,
                                                           a_Modifiers | PHANTOM_R_STATIC, a_uiFlags);
    }
#endif
    static Function* CreateFunction(StringView a_strName, Signature* a_pSignature,
                                    t_ReturnType(PHANTOM_CALLCONV(cdecl) * a_pFunc)(v_Params...),
                                    Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
    {
        auto sptr = StaticMethodPointerSimplifier<t_ReturnType(v_Params...)>::Simplify(a_pFunc);
        return PHANTOM_META_NEW(FunctionT<decltype(sptr)>)(a_strName, a_pSignature, sptr, a_Modifiers, a_uiFlags);
    }
#if PHANTOM_HAS_FASTCALL
    static Function* CreateFunction(StringView a_strName, Signature* a_pSignature,
                                    t_ReturnType(PHANTOM_CALLCONV(fastcall) * a_pFunc)(v_Params...),
                                    Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
    {
        auto sptr = StaticMethodPointerSimplifier<t_ReturnType(v_Params...)>::Simplify(a_pFunc);
        return PHANTOM_META_NEW(FunctionT<decltype(sptr)>)(a_strName, a_pSignature, sptr, a_Modifiers, a_uiFlags);
    }
#endif
};

template<typename t_ReturnType, class... v_Params>
class FunctionProviderT<t_ReturnType (*)(v_Params..., ...)> final
{
public:
    static Function* CreateFunction(StringView a_strName, Signature*                     a_pSignature,
                                    t_ReturnType (*a_pFunc)(v_Params..., ...), Modifiers a_Modifiers = 0,
                                    uint a_uiFlags = 0)
    {
        auto sptr = StaticMethodPointerSimplifier<t_ReturnType(v_Params..., ...)>::Simplify(a_pFunc);
        return PHANTOM_META_NEW(FunctionT<decltype(sptr)>)(a_strName, a_pSignature, sptr, a_Modifiers, a_uiFlags);
    }
};

#if defined(_PHNTM_CDECL_WAS_DEFINED)
#    undef _PHNTM_CDECL_WAS_DEFINED
#    pragma pop_macro("cdecl")
#endif

} // namespace reflection
} // namespace phantom

/// @endcond
