// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/DelegateSignal.h>
#include <phantom/reflection/Method.h>

namespace phantom
{
namespace reflection
{
class DelegateSignal : public Method
{
public:
    DelegateSignal(StringView _name, Signature* a_pSignature) : Method(_name, a_pSignature)
    {
    }

    virtual void connect(void* _this, OpaqueDelegate _memento) = 0;
    virtual void disconnect(void* _this, OpaqueDelegate _memento) = 0;

    void connect(void* _this, void* _receiver, Method* _method)
    {
        connect(_this, _method->getOpaqueDelegate(_receiver));
    }
    void disconnect(void* _this, void* _receiver, Method* _method)
    {
        disconnect(_this, _method->getOpaqueDelegate(_receiver));
    }
};
} // namespace reflection
} // namespace phantom
