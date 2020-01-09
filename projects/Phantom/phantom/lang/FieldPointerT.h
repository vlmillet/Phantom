// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/FieldPointer.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace lang
{
template<typename t_Any>
class FieldPointerT;

template<typename t_Ty, typename t_ValueTy>
class FieldPointerT<t_ValueTy(t_Ty::*)> : public FieldPointer
{
public:
    typedef t_ValueTy(t_Ty::*MemberPointer);
    typedef PHANTOM_TYPENAME std::remove_const<t_ValueTy>::type t_ValueTyNoConst;

public:
    FieldPointerT()
        : FieldPointer(PHANTOM_TYPEOF(t_Ty), PHANTOM_TYPEOF(t_ValueTy), sizeof(MemberPointer),
                       std::alignment_of<MemberPointer>::value, 0, PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual void* getAddress(void* a_pPointer, void const* a_pObject) const
    {
        return const_cast<t_ValueTyNoConst*>(
        &(static_cast<t_Ty const*>(a_pObject)->*(*static_cast<MemberPointer*>(a_pPointer))));
    }

    virtual size_t getOffset(void* a_pPointer) const
    {
        return (size_t) const_cast<t_ValueTyNoConst*>(
        &(((t_Ty const*)nullptr)->*(*static_cast<MemberPointer*>(a_pPointer))));
    }

    virtual void getValue(void* a_pPointer, void const* a_pObject, void* dest) const
    {
        FieldH<ValueType>::GetValue(dest,
                                    const_cast<t_Ty*>(reinterpret_cast<t_Ty const*>(a_pObject))
                                    ->*(*static_cast<MemberPointer*>(a_pPointer)));
    }
    virtual void setValue(void* a_pPointer, void* a_pObject, void const* src) const
    {
        FieldH<ValueType>::SetValue(
        src, reinterpret_cast<t_Ty*>(a_pObject)->*(*static_cast<MemberPointer*>(a_pPointer)));
    }
};

} // namespace lang
} // namespace phantom

/// @endcond
