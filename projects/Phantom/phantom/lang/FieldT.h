// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/Field.h>
#include <phantom/lang/FieldH.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace lang
{
template<class t_ClassType, class t_ValueType>
class FieldT<t_ValueType(t_ClassType::*)> final : public Field
{
public:
    typedef FieldT<t_ValueType(t_ClassType::*)> self_type;
    typedef t_ValueType                         ValueType;
    typedef t_ClassType                         ClassType;
    typedef t_ValueType(t_ClassType::*FieldPointer);
    using ValueTypeNoCV = std::remove_cv_t<ValueType>;

public:
    FieldT(Type* a_pContentType, StringView a_strName, FieldPointer a_MemberPointer,
           uint a_uiFilterMask, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Field(
          a_pContentType, a_strName,
          (size_t) const_cast<ValueTypeNoCV*>(&(((ClassType const*)nullptr)->*a_MemberPointer)),
          a_uiFilterMask, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE),
          m_MemberPointer(a_MemberPointer)
    {
    }

    virtual void* getAddress(void const* a_pObject) const override
    {
        return const_cast<ValueTypeNoCV*>(
        &(static_cast<ClassType const*>(a_pObject)->*m_MemberPointer));
    }

    virtual void getValue(void const* a_pObject, void* a_pDst) const override
    {
        FieldH<ValueType>::GetValue(
        a_pDst,
        const_cast<ClassType*>(reinterpret_cast<ClassType const*>(a_pObject))->*m_MemberPointer);
    }

    virtual void setValue(void* a_pObject, void const* a_pSrc) const override
    {
        FieldH<ValueType>::SetValue(a_pSrc,
                                    reinterpret_cast<ClassType*>(a_pObject)->*m_MemberPointer);
    }

    virtual void setValueMoved(void* a_pObject, void* a_pSrc) const override
    {
        FieldH<ValueType>::SetValueMoved(a_pSrc,
                                         reinterpret_cast<ClassType*>(a_pObject)->*m_MemberPointer);
    }

    FieldPointer m_MemberPointer;
};

} // namespace lang
} // namespace phantom

/// @endcond
