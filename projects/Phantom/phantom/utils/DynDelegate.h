// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "StringView.h"

#include <phantom/detail/TypeOfFwd.h>
#include <phantom/lang/CallHelpers.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
template<class Sign>
class DynDelegate;

class PHANTOM_EXPORT_PHANTOM OpaqueDynDelegate
{
    template<class Sign>
    friend class phantom::DynDelegate;

public:
    OpaqueDynDelegate() = default;
    OpaqueDynDelegate(void* a_pInstance, lang::Class* a_pClass, lang::Method* a_pMethod);
    OpaqueDynDelegate(lang::Class* a_pClass, StringView a_MethodName);
    OpaqueDynDelegate(lang::Method* a_pMethod);
    OpaqueDynDelegate(lang::Function* a_pFunction);
    OpaqueDynDelegate(void* a_pInstance, lang::Class* a_pClass, StringView a_MethodName);

    void*             getThis() const { return m_pMethod ? m_pThis : nullptr; }
    lang::Function*   getFunction() const { return m_pMethod ? nullptr : m_pFunction; }
    lang::Method*     getMethod() const { return m_pMethod; }
    lang::Subroutine* getSubroutine() const;

    OpaqueDelegate const& getOpaqueDelegate() const& { return m_OpaqueDelegate; }
    OpaqueDelegate&&      getOpaqueDelegate() && { return std::move(m_OpaqueDelegate); }

    bool isEmpty() const { return m_pFunction == nullptr; }

    operator bool() const { return m_pFunction != nullptr; }

    bool operator==(const OpaqueDynDelegate& a_Other) const
    {
        return m_pThis == a_Other.m_pThis && m_pMethod == a_Other.m_pMethod;
    }

    bool operator!=(const OpaqueDynDelegate& a_Other) const { return !operator==(a_Other); }

    FunctorID getID() const { return FunctorID(m_pThis, m_pMethod); }

    void call(void** a_pArgs) const;

    void call(void** a_pArgs, void* a_pReturnAddress) const;

private:
    bool _CheckSignature(lang::Type* a_pRetType, lang::TypesView a_Types) const;

private:
    lang::Method* m_pMethod = nullptr;
    union {
        void*           m_pThis = nullptr;
        lang::Function* m_pFunction;
    };
    OpaqueDelegate m_OpaqueDelegate;
};

namespace _DynDelegate
{
template<class R, class... Params>
struct Caller
{
    static R Call(OpaqueDynDelegate const& a_Dgt, Params... a_Args);
};
template<class... Params>
struct Caller<void, Params...>
{
    static void Call(OpaqueDynDelegate const& a_Dgt, Params... a_Args);
};
} // namespace _DynDelegate

template<class R, class... Params>
class DynDelegate<R(Params...)> : public OpaqueDynDelegate
{
public:
    using ThisType = DynDelegate<R(Params...)>;

public:
    DynDelegate() = default;

    DynDelegate(lang::Function* a_pFunction) : OpaqueDynDelegate(a_pFunction) { PHANTOM_ASSERT(_CheckSignature()); }

    DynDelegate(void* a_pInstance, lang::Class* a_pClass, lang::Method* a_pMethod)
        : OpaqueDynDelegate(a_pInstance, a_pClass, a_pMethod)
    {
        PHANTOM_ASSERT(_CheckSignature());
    }

    DynDelegate(void* a_pInstance, lang::Class* a_pClass, StringView a_MethodName)
        : OpaqueDynDelegate(a_pInstance, a_pClass, a_MethodName)
    {
        PHANTOM_ASSERT(_CheckSignature());
    }

    DynDelegate(OpaqueDynDelegate const& a_Dgt) : OpaqueDynDelegate(a_Dgt)
    {
        PHANTOM_ASSERT(this->isEmpty() || _CheckSignature());
    }

    R operator()(Params... a_Args) const
    {
        return _DynDelegate::Caller<R, Params...>::Call(*this, std::forward<Params>(a_Args)...);
    }

private:
    bool _CheckSignature() const
    {
        lang::Types types{PHANTOM_TYPEOF(Params)...};
        lang::Type* pRetType = PHANTOM_TYPEOF(R);
        return OpaqueDynDelegate::_CheckSignature(pRetType, types);
    }
};

namespace _DynDelegate
{
template<class R, class... Params>
R Caller<R, Params...>::Call(OpaqueDynDelegate const& a_Dgt, Params... a_Args)
{
    typename lang::CallReturnTypeH<R>::type r{};
    void*                                   args[] = {((void*)(&a_Args))..., nullptr};
    a_Dgt.call(args, &r);
    return (R)r;
}

template<class... Params>
void Caller<void, Params...>::Call(OpaqueDynDelegate const& a_Dgt, Params... a_Args)
{
    void* args[] = {((void*)(&a_Args))..., nullptr};
    a_Dgt.call(args);
}
} // namespace _DynDelegate

} // namespace phantom
