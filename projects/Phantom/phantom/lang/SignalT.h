// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/Signal.h>
#include <phantom/lang/SignatureH.h>
#include <phantom/traits/indices.h>
/* *********************************************** */

HAUNT_STOP;

namespace phantom
{
namespace lang
{
template<typename t_Ty, class t_Signature>
class SignalT;

template<typename t_Ty, class... v_Params>
class SignalT<t_Ty, void(v_Params...)> : public Signal
{
public:
    typedef phantom::Signal<void(v_Params...)> SignalType;
    typedef SignalType(t_Ty::*FieldPtrType);

    SignalT(StringView a_strName, Signature* a_pSignature, FieldPtrType a_Pointer,
            Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Signal(a_strName, a_pSignature, (size_t) & (((t_Ty*)nullptr)->*a_Pointer), a_Modifiers,
                 a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_Pointer(a_Pointer)
    {
    }
    virtual Closure getClosure() const override
    {
        Closure c = MethodClosure(&SignalType::operator());
        PHANTOM_ASSERT(c.offset == 0);
        c.offset = (int)(ptrdiff_t) & (((t_Ty*)nullptr)->*m_Pointer);
        return c;
    }

    virtual bool equalsPointer(void* a_pPtrOnPtr) const override
    {
        return *(FieldPtrType*)a_pPtrOnPtr == m_Pointer;
    }

    OpaqueDelegate getOpaqueDelegate(void* a_pObject) const override
    {
        return Delegate<void(v_Params...)>(&(static_cast<t_Ty*>(a_pObject)->*m_Pointer),
                                           &SignalType::operator())
        .getOpaque();
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

    virtual void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        void* pThis = *(void**)(*a_pArgs++);
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr,
                       "expecting return address from a signal function call");
        _invoke(pThis, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

    virtual void placementCall(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(false, "expecting return address from a signal function call");
        void* pThis = *(void**)(*a_pArgs++);
        _invoke(pThis, a_pArgs, BuildIndices<sizeof...(v_Params)>{});
    }

private:
    template<size_t... Is>
    inline void _invoke(void* a_pObject, void** a_pParams, Indices<Is...>) const
    {
        (static_cast<t_Ty*>(a_pObject)->*m_Pointer)(
        ((v_Params) *
         static_cast<PHANTOM_TYPENAME std::remove_reference<v_Params>::type*>(a_pParams[Is]))...);
    }

private:
    FieldPtrType m_Pointer;
};

} // namespace lang
} // namespace phantom

/// @endcond
