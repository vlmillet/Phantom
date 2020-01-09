// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/MethodPointer.h>
/* *********************************************** */

HAUNT_STOP;

namespace phantom
{
namespace lang
{
template<typename t_Class>
class MethodPointerBaseT : public MethodPointer
{
public:
    typedef MethodPointerBaseT<t_Class> self_type;
    typedef void (t_Class::*simplest_member_function_ptr)();
    MethodPointerBaseT(Signature* a_pSignature, size_t a_Size, size_t a_Alignment, Modifiers a_Modifiers = 0,
                       uint a_uiFlags = 0)
        : MethodPointer(PHANTOM_TYPEOF(t_Class), a_pSignature, a_Size, a_Alignment, a_Modifiers,
                        a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual void call(void* a_pPointer, void* a_pThis, void** a_pArgs) const = 0;
    virtual void call(void* a_pPointer, void* a_pThis, void** a_pArgs, void* a_pReturnAddress) const
    {
        PHANTOM_ASSERT(!a_pReturnAddress,
                       "Expecting return address from a void function call, use call(void*, void**) instead");
        call(a_pPointer, a_pThis, a_pArgs);
    }
};

} // namespace lang
} // namespace phantom

/// @endcond
