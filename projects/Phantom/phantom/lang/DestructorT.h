// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include "ExecutionContext.h"

#include <phantom/lang/Destructor.h>
#include <phantom/lang/NativeVTableInspector.h>
#include <phantom/lang/Signature.h>
#include <phantom/traits/HasVirtualDestructor.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace lang
{
template<typename t_Ty>
class DestructorT : public Destructor
{
    typedef DestructorT<t_Ty> self_type;

public:
    DestructorT(StringView a_strName, Signature* a_pSignature)
        : Destructor(a_strName, a_pSignature, Modifiers(HasVirtualDestructor<t_Ty>::value ? PHANTOM_R_VIRTUAL : 0),
                     PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual size_t computeNativeVirtualIndex() const override { return phantom::virtualDestructorIndex<t_Ty>(); }

    virtual Closure getClosure() const override { return MethodClosure(&self_type::closure_wrapper); }

    OpaqueDelegate getOpaqueDelegate(void* a_pObject) const override
    {
        typedef void (t_Ty::*ptr)();
        return Delegate<void()>(reinterpret_cast<t_Ty*>(a_pObject), (ptr)&self_type::closure_wrapper).getOpaque();
    }

    virtual void invoke(void* a_pInstance, void**) const override { Dtor<t_Ty>::apply(a_pInstance); }

    virtual void invoke(void* a_pInstance, void**, void*) const override { Dtor<t_Ty>::apply(a_pInstance); }

    virtual void call(void** a_pArgs) const override { Dtor<t_Ty>::apply(*(void**)(a_pArgs[0])); }

    virtual void call(void** a_pArgs, void*) const override { Dtor<t_Ty>::apply(*(void**)(a_pArgs[0])); }

    virtual void call(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr, "expecting return address from a void function call");
        Dtor<t_Ty>::apply(*(void**)(a_pArgs[0]));
    }

    virtual void placementCall(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        PHANTOM_ASSERT(a_Context.resultPointer() == nullptr, "expecting return address from a void function call");
        Dtor<t_Ty>::apply(*(void**)(a_pArgs[0]));
    }

private:
    void closure_wrapper() { Dtor<t_Ty>::apply(this); }
};

} // namespace lang
} // namespace phantom

/// @endcond
