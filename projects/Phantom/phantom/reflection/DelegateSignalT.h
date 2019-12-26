// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include "DelegateSignal.h"
/* *********************************************** */

HAUNT_STOP;

namespace phantom
{
namespace reflection
{
template<class T, class Signature, class Mutex>
class DelegateSignalT;

template<class T, class... Params, class Mutex>
class DelegateSignalT<T, void(Params...), Mutex> : public DelegateSignal
{
public:
    typedef phantom::DelegateSignal<void(Params...), Mutex> SignalType;
    typedef SignalType(T::*SignalMemberPointer);
    DelegateSignalT(StringView _name, SignalMemberPointer _signal)
        : DelegateSignal(
          _name, Signature::Create(PHANTOM_TYPEOF(void), Types{TypeOf<Params>::object()...})),
          m_ptr(_signal)
    {
    }

    virtual void invoke(void* _this, void** _args) const override
    {
        _invoke(_this, _args, BuildIndices<sizeof...(Params)>{});
    }

    virtual OpaqueDelegate getOpaqueDelegate(void* _this) const
    {
        return Delegate<void(Params...)>(&(reinterpret_cast<T*>(_this)->*m_ptr), &SignalType::emit)
        .getOpaque();
    }

    virtual void connect(void* _this, OpaqueDelegate _opaque)
    {
        typename SignalType::Delegate dgt;
        dgt.setOpaque(_opaque);
        (reinterpret_cast<T*>(_this)->*m_ptr).connect(dgt);
    }

    virtual void disconnect(void* _this, OpaqueDelegate _memento)
    {
        typename SignalType::Delegate dgt;
        dgt.setOpaque(_memento);
        (reinterpret_cast<T*>(_this)->*m_ptr).disconnect(dgt);
    }

private:
    template<size_t... Is>
    inline void _invoke(void* _this, void** _args, Indices<Is...>) const
    {
        (reinterpret_cast<T*>(_this)->*m_ptr)(
        ((Params) * static_cast<typename std::remove_reference<Params>::type*>(_args[Is]))...);
    }

private:
    SignalMemberPointer m_ptr;
};
} // namespace reflection
} // namespace phantom
