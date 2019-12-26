// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "Object.h"
#include "RttiImpl.h"
#include "mostDerivedOfImpl.h"

#include <phantom/reflection/TypeOf.h>

namespace phantom
{
namespace detail
{
template<class t_Ty>
Object mostDerivedOfH<t_Ty, false>::apply(void* a_pThis)
{
    auto&  rtti = Rtti::Find(a_pThis);
    Object obj;
    if (rtti.isNull())
    {
        obj = Object(a_pThis, (reflection::Class*)PHANTOM_TYPEOF(t_Ty));
    }
    else
    {
        obj = Object(rtti.getObjectAddress(), rtti.getObjectClass());
    }
    return obj;
};

} // namespace detail
} // namespace phantom
