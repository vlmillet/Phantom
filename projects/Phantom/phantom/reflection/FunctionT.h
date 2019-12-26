// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/reflection/CallHelpers.h>
#include <phantom/reflection/Function.h>
#include <phantom/traits/indices.h>
/* **************** Declarations ***************** */

/* *********************************************** */
extern "C"
{
    void* __cdecl Phantom_AssemblyVarArgCall(void* func, size_t retSize, char* buffer, size_t size, size_t align);
}

namespace phantom
{
namespace reflection
{
inline void varArgCall(void* a_pFunc, void* a_pRetAddress, Type* a_pRetType, void** a_pParams, Type** explicitTypes,
                       size_t expCount, Types const& a_VarArgTypes)
{
    size_t                  varCount = a_VarArgTypes.size();
    Type::AlignmentComputer aligner;
    for (size_t i = 0; i < expCount; ++i)
    {
        aligner.push(explicitTypes[i]);
    }
    for (size_t i = 0; i < varCount; ++i)
    {
        aligner.push(a_VarArgTypes[i]);
    }
    char* buffer = (char*)alloca(aligner.align());
    aligner.reset();
    for (size_t i = 0; i < expCount; ++i)
    {
        explicitTypes[i]->copyConstruct(buffer + aligner.push(explicitTypes[i]), a_pParams[i]);
    }
    for (size_t i = 0; i < varCount; ++i)
    {
        a_VarArgTypes[i]->copy(buffer + aligner.push(a_VarArgTypes[i]), a_pParams[i]);
    }
    void* pRetAddress =
    Phantom_AssemblyVarArgCall(a_pFunc, a_pRetType->getSize(), buffer, aligner.align(), aligner.maxAlignment());
    a_pRetType->copyConstruct(a_pRetAddress, pRetAddress);
}

template<typename FunctionPointer>
class FunctionT;

template<ABI t_convention, class S>
struct OpaqueDelegateCaster
{
    template<class Fptr>
    static OpaqueDelegate cast(Fptr)
    {
        PHANTOM_WARNING(false, "'%s' convention not supported for creating opaque delegate",
                        t_convention == ABI::StdCall ? "stdcall" : ABI::FastCall ? "fastcall" : "vararg");
        return OpaqueDelegate();
    }
};

template<class S>
struct OpaqueDelegateCaster<ABI::CDecl, S>
{
    template<class Fptr>
    static OpaqueDelegate cast(Fptr ptr)
    {
        return Delegate<S>(ptr).getOpaque();
    }
};

template<class S>
struct OpaqueDelegateCaster<ABI::Win64, S>
{
    template<class Fptr>
    static OpaqueDelegate cast(Fptr ptr)
    {
        return Delegate<S>(ptr).getOpaque();
    }
};

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define _PHNTM_CALL_CONV __cdecl
#else
#    define _PHNTM_CALL_CONV
#endif
#define _PHNTM_CALL_CONV_ABI CDecl
#include "FunctionT_PerCallConv.h"
#undef _PHNTM_CALL_CONV
#undef _PHNTM_CALL_CONV_ABI

#if PHANTOM_HAS_FASTCALL
#    define _PHNTM_CALL_CONV __fastcall
#    define _PHNTM_CALL_CONV_ABI FastCall
#    include "FunctionT_PerCallConv.h"
#    undef _PHNTM_CALL_CONV
#    undef _PHNTM_CALL_CONV_ABI
#endif

#if PHANTOM_HAS_STDCALL
#    define _PHNTM_CALL_CONV __stdcall
#    define _PHNTM_CALL_CONV_ABI StdCall
#    include "FunctionT_PerCallConv.h"
#    undef _PHNTM_CALL_CONV
#    undef _PHNTM_CALL_CONV_ABI
#endif

/////// VARARG

template<typename t_ReturnType, class... v_Params>
class FunctionT<t_ReturnType (*)(v_Params..., ...)> : public Function
{
public:
    typedef t_ReturnType (*FunctionPtrType)(v_Params..., ...);
    typedef t_ReturnType(Func)(v_Params..., ...);
    typedef FunctionT<FunctionPtrType> SelfType;
    typedef PHANTOM_TYPENAME CallReturnTypeH<t_ReturnType>::type CallReturnType;

    FunctionT(StringView a_strName, Signature* a_pSignature, FunctionPtrType a_pFunc, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Function(a_strName, a_pSignature, (ABI)ABI::VarArgCall, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_pFunc)
    {
    }

    virtual Closure getClosure() const override
    {
        Closure c;
        c.address = (void*)m_Pointer;
        return c;
    }

    virtual OpaqueDelegate getOpaqueDelegate() const override
    {
        return OpaqueDelegateCaster<ABI::VarArgCall, Func>::cast(m_Pointer);
    }

    virtual void call(void**) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    template<size_t... Is>
    inline void callVarArg(void** a_pParams, Types const& a_VarArgTypes, void* a_pReturnAddress, Indices<Is...>) const
    {
        Type* explicitTypes[] = {PHANTOM_TYPEOF(PHANTOM_TYPENAME std::remove_reference<v_Params>::type)...};
        varArgCall((void*)m_Pointer, a_pReturnAddress, getReturnType(), a_pParams, explicitTypes,
                   sizeof(explicitTypes) / sizeof(Type*), a_VarArgTypes);
    }

    virtual void callVarArg(void** a_pParams, Types const& a_VarArgTypes, void* a_pReturnAddress) const override
    {
        callVarArg(a_pParams, a_VarArgTypes, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void callVarArg(ExecutionContext& a_Context, void** a_pParams, Types const& a_VarArgTypes) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() != nullptr, "expecting return address");
        callVarArg(a_pParams, a_VarArgTypes, a_Context.resultPointer(), BuildIndices<sizeof...(v_Params)>{});
    }

    template<size_t... Is>
    inline void placementCallVarArg(void** a_pParams, Types const& a_VarArgTypes, void* a_pReturnAddress,
                                    Indices<Is...>) const
    {
        Type* explicitTypes[] = {PHANTOM_TYPEOF(PHANTOM_TYPENAME std::remove_reference<v_Params>::type)...};
        varArgCall((void*)m_Pointer, a_pReturnAddress, getReturnType(), a_pParams, explicitTypes,
                   sizeof(explicitTypes) / sizeof(Type*), a_VarArgTypes);
    }

    virtual void placementCallVarArg(void** a_pParams, Types const& a_VarArgTypes,
                                     void* a_pReturnAddress) const override
    {
        placementCallVarArg(a_pParams, a_VarArgTypes, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void call(void**, void*) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    virtual void placementCall(void**, void*) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    virtual void call(ExecutionContext&, void**) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    virtual void placementCall(ExecutionContext&, void**) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

private:
    FunctionPtrType m_Pointer;
};

template<class... v_Params>
class FunctionT<void (*)(v_Params..., ...)> : public Function
{
public:
    typedef FunctionT<void (*)(v_Params..., ...)> SelfType;
    typedef void (*FunctionPtrType)(v_Params..., ...);

    FunctionT(StringView a_strName, Signature* a_pSignature, FunctionPtrType a_pFunc, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Function(a_strName, a_pSignature, (ABI)ABI::VarArgCall, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_pFunc)
    {
    }

    virtual Closure getClosure() const override
    {
        Closure c;
        c.address = (void*)m_Pointer;
        return c;
    }

    virtual OpaqueDelegate getOpaqueDelegate() const override
    {
        return OpaqueDelegateCaster<ABI::VarArgCall, void(v_Params..., ...)>::cast(m_Pointer);
    }

    template<size_t... Is>
    inline void call(void** a_pParams, Types const& a_VarArgTypes, Indices<Is...>) const
    {
        Type* explicitTypes[] = {
        phantom::reflection::TypeOf<PHANTOM_TYPENAME std::remove_reference<v_Params>::type>::object()...};

        varArgCall((void*)m_Pointer, nullptr, a_pParams, explicitTypes, sizeof(explicitTypes) / sizeof(Type*),
                   a_VarArgTypes);
    }

    virtual void call(ExecutionContext&, void**) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    virtual void placementCall(ExecutionContext&, void**) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    virtual void call(void**) const override
    {
        PHANTOM_ASSERT(false, "illegal default call on a vararg function");
    }

    virtual void callVarArg(void** a_pParams, Types const& a_VarArgTypes) const override
    {
        call(a_pParams, a_VarArgTypes, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void callVarArg(ExecutionContext& a_Context, void** a_pParams, Types const& a_VarArgTypes) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr, "expecting return address from a void function call");
        call(a_pParams, a_VarArgTypes, BuildIndices<sizeof...(v_Params)>{});
    }

private:
    FunctionPtrType m_Pointer;
};

} // namespace reflection
} // namespace phantom

/// @endcond
