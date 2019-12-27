// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include "StringView.h"

#include <phantom/reflection/CallHelpers.h>
#include <phantom/typeof>
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
    OpaqueDynDelegate(void* a_pInstance, reflection::Class* a_pClass, reflection::Method* a_pMethod);
    OpaqueDynDelegate(reflection::Function* a_pFunction);
    OpaqueDynDelegate(void* a_pInstance, reflection::Class* a_pClass, StringView a_MethodName);

    void* getThis() const
    {
        return m_pMethod ? m_pThis : nullptr;
    }
    reflection::Function* getFunction() const
    {
        return m_pMethod ? nullptr : m_pFunction;
    }
    reflection::Method* getMethod() const
    {
        return m_pMethod;
    }
    reflection::Subroutine* getSubroutine() const;

    bool isEmpty() const
    {
        return m_pFunction == nullptr;
    }

    operator bool() const
    {
        return m_pFunction != nullptr;
    }

    bool operator==(const OpaqueDynDelegate& a_Other) const
    {
        return m_pThis == a_Other.m_pThis && m_pMethod == a_Other.m_pMethod;
    }

    bool operator!=(const OpaqueDynDelegate& a_Other) const
    {
        return !operator==(a_Other);
    }

    FunctorID getID() const
    {
        return FunctorID(m_pThis, m_pMethod);
    }

    void call(void** a_pArgs) const;

    void call(void** a_pArgs, void* a_pReturnAddress) const;

private:
    bool _CheckSignature(reflection::Type* a_pRetType, reflection::TypesView a_Types) const;

private:
    reflection::Method* m_pMethod = nullptr;
    union {
        void*                 m_pThis = nullptr;
        reflection::Function* m_pFunction;
    };
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

    DynDelegate(reflection::Function* a_pFunction) : OpaqueDynDelegate(a_pFunction)
    {
        PHANTOM_ASSERT(_CheckSignature());
    }

    DynDelegate(Object a_Object, reflection::Method* a_pMethod) : OpaqueDynDelegate(a_Object, a_pMethod)
    {
        PHANTOM_ASSERT(_CheckSignature());
    }

    DynDelegate(Object a_Object, StringView a_MethodName) : OpaqueDynDelegate(a_Object, a_MethodName)
    {
        PHANTOM_ASSERT(_CheckSignature());
    }

    explicit DynDelegate(OpaqueDynDelegate a_Dgt)
    {
        m_pThis = a_Dgt.m_pThis;
        m_pMethod = a_Dgt.m_pMethod;
        PHANTOM_ASSERT(_CheckSignature());
    }

    R operator()(Params... a_Args) const
    {
        return _DynDelegate::Caller<R, Params...>::Call(*this, std::forward<Params>(a_Args)...);
    }

private:
    bool _CheckSignature() const
    {
        reflection::Types types{PHANTOM_TYPEOF(Params)...};
        reflection::Type* pRetType = PHANTOM_TYPEOF(R);
        return OpaqueDynDelegate::_CheckSignature(pRetType, types);
    }
};

namespace _DynDelegate
{
template<class R, class... Params>
R Caller<R, Params...>::Call(OpaqueDynDelegate const& a_Dgt, Params... a_Args)
{
    typename reflection::CallReturnTypeH<R>::type r{};
    void*                                         args[] = {((void*)(&a_Args))..., nullptr};
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
