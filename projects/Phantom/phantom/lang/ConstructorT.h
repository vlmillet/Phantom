// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/detail/ConstructorCaller.h>
#include <phantom/lang/CallHelpers.h>
#include <phantom/lang/ClassType.h>
#include <phantom/lang/Constructor.h>
#include <phantom/lang/NativeVTableSizeComputer.h>
#include <phantom/lang/SignatureH.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
/* ************* t_Ty Declarations ************** */

/* *********************************************** */

template<typename t_Class, typename t_Signature, bool t_is_default_constructible_and_polymorphic>
struct ConstructorVTableHacksT
{
    static size_t getVTableSize()
    {
        return NativeVTableSizeComputer<t_Class, t_Signature>::apply();
    }
};

template<typename t_Class, typename t_Signature>
struct ConstructorVTableHacksT<t_Class, t_Signature, false>
{
    static size_t getVTableSize()
    {
        return 0;
    }
};

template<typename t_Class, typename t_Signature>
class ConstructorT final : public Constructor
{
public:
    typedef ConstructorT<t_Class, t_Signature> self_type;
    ConstructorT(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Constructor(a_strName, a_pSignature, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

    Closure getClosure() const override
    {
        return MethodClosure(&ConstructorCaller<t_Class, t_Signature>::closure);
    }

    void invoke(void* a_pThis, void** a_ppArgs, void* a_pReturnAddress) const override
    {
        PHANTOM_ASSERT(!a_pReturnAddress,
                       "Expecting return address from a constructor call, use "
                       "placementCall(void*, void**) instead");
        ConstructorCaller<t_Class, t_Signature>::apply(a_pThis, a_ppArgs);
    }

    void placementInvoke(void* a_pThis, void** a_ppArgs, void* a_pReturnAddress) const override
    {
        PHANTOM_ASSERT(!a_pReturnAddress,
                       "Expecting return address from a constructor call, use "
                       "placementCall(void*, void**) instead");
        ConstructorCaller<t_Class, t_Signature>::apply(a_pThis, a_ppArgs);
    }

    void call(void** a_ppArgs, void* a_pReturnAddress) const override
    {
        ConstructorCaller<t_Class, t_Signature>::apply(a_pReturnAddress, a_ppArgs);
    }

    using Constructor::placementCall;

    void placementCall(void** a_ppArgs, void* a_pReturnAddress) const override
    {
        ConstructorCaller<t_Class, t_Signature>::apply(a_pReturnAddress, a_ppArgs);
    }

    void invoke(void* a_pThis, void** a_ppArgs) const override
    {
        ConstructorCaller<t_Class, t_Signature>::apply(a_pThis, a_ppArgs);
    }

    virtual void call(void** a_ppArgs) const override
    {
        void* pThis = *a_ppArgs++; /// 'this' pointer is stored as '*this', a reference, so getting address of
                                   /// argument is getting address of *this (&*this) which is in fact 'this'
        ConstructorCaller<t_Class, t_Signature>::apply(pThis, a_ppArgs);
    }

    typedef ConstructorVTableHacksT<t_Class, t_Signature,
                                    std::is_polymorphic<t_Class>::value && !std::is_abstract<t_Class>::value
                                                                        &&::phantom::IsDefaultConstructible<t_Class>::value>
    hacker;

    void** getNativeVTablePointer() const override
    {
        return nullptr /*hacker::getVTablePointer()*/;
    }
    size_t getNativeVTableSize() const override
    {
        return hacker::getVTableSize();
    }
};

} // namespace lang
} // namespace phantom

/// @endcond
