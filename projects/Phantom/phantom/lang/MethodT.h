// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/MethodBaseT.h>
#include <phantom/traits/indices.h>
/* **************** Declarations ***************** */

/* *********************************************** */
//#if PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_SIGNATURE
#define TNMF_signature_param_insert Signature *a_pSignature,
#define TNMF_signature_arg_insert a_pSignature,
// #else
// #define TNMF_signature_param_insert StringView a_strSignature,
// #define TNMF_signature_arg_insert a_strSignature,
// #endif

#if PHANTOM_CUSTOM_USE_PRECISE_SIGNATURE

#    define _PHNTM_MTHD_typename_t_TyT typename Method_t_TyT
#    define _PHNTM_MTHD_t_TyT t_Ty,
#    define _PHNTM_MTHD_t_Ty_onlyT t_Ty
#    define _PHNTM_MTHD_callerT static_cast<t_Ty*>(a_pObject)
#    define _PHNTM_MTHD_caller_typeT t_Ty

#else

#    define _PHNTM_MTHD_typename_t_TyT
#    define _PHNTM_MTHD_t_TyT
#    define _PHNTM_MTHD_t_Ty_onlyT
#    define _PHNTM_MTHD_callerT static_cast< ::phantom::DummyClass*>(a_pObject)
#    define _PHNTM_MTHD_caller_typeT ::phantom::DummyClass

#endif

namespace phantom
{
namespace lang
{
template<_PHNTM_MTHD_typename_t_TyT class t_Signature>
class MethodT;

struct IsMethod
{
    static std::true_type  select(Method*);
    static std::false_type select(StaticMethod*);
};

template<_PHNTM_MTHD_typename_t_TyT typename t_ReturnType, class... v_Params>
class MethodT<_PHNTM_MTHD_t_TyT t_ReturnType (DummyClass::*)(v_Params...)> : public MethodBaseT
{
public:
    typedef PHANTOM_TYPENAME CallReturnTypeH<t_ReturnType>::type                 CallReturnType;
    typedef MethodT<_PHNTM_MTHD_t_TyT t_ReturnType (DummyClass::*)(v_Params...)> self_type;
    typedef t_ReturnType (_PHNTM_MTHD_caller_typeT::*MethodPtrType)(v_Params...);

    MethodT(StringView a_strName, TNMF_signature_param_insert MethodPtrType a_pFunc, Modifiers a_Modifiers = 0,
            uint a_uiFlags = 0)
        : MethodBaseT(a_strName, TNMF_signature_arg_insert a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_pFunc)
    {
    }

    virtual size_t computeNativeVirtualIndex() const override
    {
        PHANTOM_ASSERT(isVirtual());
        return VTableIndexOf<true>::apply(m_Pointer);
    }

    virtual Closure getClosure() const override
    {
        return MethodClosure(m_Pointer);
    }

    virtual OpaqueDelegate getOpaqueDelegate(void* a_pObject) const override
    {
        return Delegate<t_ReturnType(v_Params...)>(_PHNTM_MTHD_callerT, m_Pointer).getOpaque();
    }

    virtual void invoke(void* a_pObject, void** a_pArgs) const override
    {
        _invoke(a_pObject, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }
    virtual void invoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress) const override
    {
        _invoke(a_pObject, a_pArgs, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void call(void** a_pArgs) const override
    {
        void* pThis = *(void**)(*a_pArgs++);
        _invoke(pThis, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }
    virtual void call(void** a_pArgs, void* a_pReturnAddress) const override
    {
        void* pThis = *(void**)(*a_pArgs++);
        _invoke(pThis, a_pArgs, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }
    virtual void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        void* pThis = *(void**)(*a_pArgs++);
        if (a_Context.resultPointer()) // depending of presence of a result address, we use
                                       // different overload
            _invoke(pThis, a_pArgs, a_Context.resultPointer(), BuildIndices<sizeof...(v_Params)>{});
        else
            _invoke(pThis, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void placementCall(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        void* pThis = *(void**)(*a_pArgs++);
        _placementInvoke(pThis, a_pArgs, a_Context.resultPointer(), BuildIndices<sizeof...(v_Params)>{});
    }
    virtual void placementInvoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress) const override
    {
        _placementInvoke(a_pObject, a_pArgs, a_pReturnAddress, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual bool equalsPointer(void* a_pPtrOnPtr) const override
    {
        return *(MethodPtrType*)a_pPtrOnPtr == m_Pointer;
    }

private:
    template<size_t... Is>
    inline void _invoke(void* a_pObject, void** a_pArgs, Indices<Is...>) const
    {
        (_PHNTM_MTHD_callerT->*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pArgs[Is]))...);
    }

    template<size_t... Is>
    inline void _invoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress, Indices<Is...>) const
    {
        PHANTOM_ASSERT(a_pReturnAddress,
                       "non-void function call expected a return address, use call(void*, void**) "
                       "or call(void**) signatures if return value must be ignored");
        CallReturner<t_ReturnType>::apply(
        a_pReturnAddress,
        (_PHNTM_MTHD_callerT->*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pArgs[Is]))...));
    }
    template<size_t... Is>
    inline void _placementInvoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress, Indices<Is...>) const
    {
        new (a_pReturnAddress)
        CallReturnType(CallReturnConverter<t_ReturnType>::apply((_PHNTM_MTHD_callerT->*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pArgs[Is]))...)));
    }

private:
    MethodPtrType m_Pointer;
};

template<_PHNTM_MTHD_typename_t_TyT class... v_Params>
class MethodT<_PHNTM_MTHD_t_TyT void (DummyClass::*)(v_Params...)> : public MethodBaseT
{
public:
    typedef void (_PHNTM_MTHD_caller_typeT::*MethodPtrType)(v_Params...);
    typedef MethodT<_PHNTM_MTHD_t_TyT void (DummyClass::*)(v_Params...)> self_type;

    MethodT(StringView a_strName, TNMF_signature_param_insert MethodPtrType a_pFunc, Modifiers a_Modifiers = 0,
            uint a_uiFlags = 0)
        : MethodBaseT(a_strName, TNMF_signature_arg_insert a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_pFunc)
    {
    }

    virtual size_t computeNativeVirtualIndex() const override
    {
        PHANTOM_ASSERT(isVirtual());
        return VTableIndexOf<true>::apply(m_Pointer);
    }

    virtual Closure getClosure() const override
    {
        return MethodClosure(m_Pointer);
    }

    virtual OpaqueDelegate getOpaqueDelegate(void* a_pObject) const override
    {
        return Delegate<void(v_Params...)>(_PHNTM_MTHD_callerT, m_Pointer).getOpaque();
    }

    virtual void invoke(void* a_pObject, void** a_pArgs) const override
    {
        _invoke(a_pObject, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void call(void** a_pArgs) const override
    {
        void* pThis = *(void**)(*a_pArgs++);
        _invoke(pThis, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    using Method::call;

    void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr);
        void* pThis = *(void**)(*a_pArgs++);
        _invoke(pThis, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    using Method::placementCall;

    void placementCall(ExecutionContext&, void**) const override
    {
        PHANTOM_ASSERT(false, "expecting return address from a void function call");
    }
    virtual void placementInvoke(void*, void**, void*) const override
    {
        PHANTOM_ASSERT(false, "expecting return address from a void function call");
    }

    virtual bool equalsPointer(void* a_pPtrOnPtr) const override
    {
        return *(MethodPtrType*)a_pPtrOnPtr == m_Pointer;
    }

private:
    template<size_t... Is>
    inline void _invoke(void* a_pObject, void** a_pArgs, Indices<Is...>) const
    {
        (_PHNTM_MTHD_callerT->*m_Pointer)(
        ((v_Params) * static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pArgs[Is]))...);
    }

private:
    MethodPtrType m_Pointer;
};

} // namespace lang
} // namespace phantom

/// @endcond
