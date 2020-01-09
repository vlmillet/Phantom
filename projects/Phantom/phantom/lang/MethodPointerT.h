// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/reflection/MethodPointerBaseT.h>
#include <phantom/reflection/MethodT.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace reflection
{
#define MethodPointer_NativeSignatureProvider_self_type_TT(Ctx) SignatureH <

template<typename R, typename t_Ty, class... v_Params>
class MethodPointerT<R (t_Ty::*)(v_Params...)> : public MethodPointerBaseT<t_Ty>
{
public:
    typedef MethodPointerT<R (t_Ty::*)(v_Params...)> self_type;
    typedef R (t_Ty::*MemberPointer)(v_Params...);

    MethodPointerT()
        : MethodPointerBaseT<t_Ty>(MethodPointer_NativeSignatureProvider_self_type_TT(Ctx)
                                   R(v_Params...) > ::CreateSignature(),
                                   sizeof(MemberPointer), std::alignment_of<MemberPointer>::value,
                                   0, PHANTOM_R_FLAG_NATIVE)
    {
    }
    template<size_t... Is>
    inline void call(void* a_pPointer, void* a_pObject, void** a_pParams, Indices<Is...>) const
    {
        (_PHNTM_MTHD_callerT->*(*static_cast<MemberPointer*>(a_pPointer)))(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

    virtual void call(void* a_pPointer, void* a_pObject, void** a_pParams) const
    {
        call(a_pPointer, a_pObject, a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    template<size_t... Is>
    inline void call(void* a_pPointer, void* a_pObject, void** a_pParams, void* a_pReturnAddress,
                     Indices<Is...>) const
    {
        CallReturner<R>::apply(
        a_pReturnAddress,
        (_PHNTM_MTHD_callerT->*(*static_cast<MemberPointer*>(a_pPointer)))(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...));
    }

    virtual void call(void* a_pPointer, void* a_pObject, void** a_pParams,
                      void* a_pReturnAddress) const
    {
        call(a_pPointer, a_pObject, a_pParams, a_pReturnAddress,
             BuildIndices<sizeof...(v_Params)>{});
    }
};

template<typename t_Ty, class... v_Params>
class MethodPointerT<void (t_Ty::*)(v_Params...)> : public MethodPointerBaseT<t_Ty>
{
public:
    typedef MethodPointerT<void (t_Ty::*)(v_Params...)> self_type;
    typedef void (t_Ty::*MemberPointer)(v_Params...);

    MethodPointerT()
        : MethodPointerBaseT<t_Ty>(MethodPointer_NativeSignatureProvider_self_type_TT(Ctx) void(
                                   v_Params...) > ::CreateSignature(),
                                   sizeof(MemberPointer), std::alignment_of<MemberPointer>::value,
                                   0, PHANTOM_R_FLAG_NATIVE)
    {
    }
    template<size_t... Is>
    inline void call(void* a_pPointer, void* a_pObject, void** a_pParams, Indices<Is...>) const
    {
        (static_cast<t_Ty*>(a_pObject)->*(*static_cast<MemberPointer*>(a_pPointer)))(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

    virtual void call(void* a_pPointer, void* a_pObject, void** a_pParams) const
    {
        call(a_pPointer, a_pObject, a_pParams, BuildIndices<sizeof...(v_Params)>{});
    }

    template<size_t... Is>
    inline void call(void* a_pPointer, void* a_pObject, void** a_pParams, void* a_pReturnAddress,
                     Indices<Is...>) const
    {
        (static_cast<t_Ty*>(a_pObject)->*(*static_cast<MemberPointer*>(a_pPointer)))(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

    virtual void call(void* a_pPointer, void* a_pObject, void** a_pParams,
                      void* a_pReturnAddress) const
    {
        call(a_pPointer, a_pObject, a_pParams, a_pReturnAddress,
             BuildIndices<sizeof...(v_Params)>{});
    }
};
template<class R, class t_Ty, class... v_Params>
class MethodPointerT<R (t_Ty::*)(v_Params...) const>
    : public MethodPointerT<R (t_Ty::*)(v_Params...)>
{
};

} // namespace reflection
} // namespace phantom

/// @endcond
