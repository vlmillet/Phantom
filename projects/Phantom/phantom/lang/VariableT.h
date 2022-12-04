// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/detail/Copier.h>
#include <phantom/lang/Variable.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace lang
{
template<typename t_Ty>
class VariableT : public Variable
{
public:
    typedef typename std::remove_extent<t_Ty>::type ValueTypeNoArray;

public:
    VariableT(StringView a_strName, ValueTypeNoArray* a_pPointer, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Variable(PHANTOM_TYPEOF(t_Ty), a_strName, a_pPointer, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
    VariableT(Type* a_pContentType, StringView a_strName, ValueTypeNoArray* a_pPointer, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Variable(a_pContentType, a_strName, a_pPointer, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual void getValue(void* dest) const
    {
        ::phantom::Copier<t_Ty>::copy((ValueTypeNoArray*)dest, (const ValueTypeNoArray*)m_pAddress);
    }
    virtual void setValue(void const* src) const
    {
        ::phantom::Copier<t_Ty>::copy((ValueTypeNoArray*)m_pAddress, (const ValueTypeNoArray*)src);
    }
};

template<typename t_Ty>
class VariableT<t_Ty const> : public VariableT<t_Ty>
{
public:
    VariableT(Type* a_pContentType, StringView a_strName, const t_Ty* a_pPointer, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : VariableT<t_Ty>(a_pContentType, a_strName, const_cast<t_Ty*>(a_pPointer), a_Modifiers, a_uiFlags)
    {
    }
    VariableT(StringView a_strName, const t_Ty* a_pPointer, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : VariableT<t_Ty>(PHANTOM_TYPEOF(t_Ty const), a_strName, const_cast<t_Ty*>(a_pPointer), a_Modifiers, a_uiFlags)
    {
    }
};

template<typename t_Ty, size_t t_Size>
class VariableT<t_Ty[t_Size]> : public Variable
{
public:
    using ValueTypeNoExtent = std::remove_all_extents_t<t_Ty>;

public:
    VariableT(Type* a_pContentType, StringView a_strName, t_Ty (*a_pPointer)[t_Size], Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Variable(a_pContentType, a_strName, a_pPointer, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
    VariableT(StringView a_strName, t_Ty (*a_pPointer)[t_Size], Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Variable(PHANTOM_TYPEOF(t_Ty[t_Size]), a_strName, a_pPointer, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual void getValue(void* dest) const
    {
        ::phantom::Copier<t_Ty[t_Size]>::copy((ValueTypeNoExtent*)dest, (const ValueTypeNoExtent*)m_pAddress);
    }
    virtual void setValue(void const* src) const
    {
        ::phantom::Copier<t_Ty[t_Size]>::copy((ValueTypeNoExtent*)m_pAddress, (const ValueTypeNoExtent*)src);
    }
};

template<typename t_Ty>
class VariableT<t_Ty[]> : public Variable
{
public:
    typedef typename std::remove_extent<t_Ty>::type ValueTypeNoArray;

public:
    VariableT(Type* a_pContentType, StringView a_strName, t_Ty (*a_pPointer)[], Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : Variable(a_pContentType, a_strName, a_pPointer, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
    VariableT(StringView a_strName, t_Ty (*a_pPointer)[], Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Variable(PHANTOM_TYPEOF(t_Ty[]), a_strName, a_pPointer, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
    virtual void getValue(void* dest) const { ::phantom::Copier<t_Ty*>::copy((t_Ty**)dest, (t_Ty* const*)m_pAddress); }
    virtual void setValue(void const* src) const
    {
        ::phantom::Copier<t_Ty*>::copy((t_Ty**)m_pAddress, (t_Ty* const*)src);
    }
};

template<typename t_Ty, size_t t_Size>
class VariableT<t_Ty const[t_Size]> : public VariableT<t_Ty[t_Size]>
{
public:
    VariableT(Type* a_pContentType, StringView a_strName, t_Ty const (*a_pPointer)[t_Size], Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : VariableT<t_Ty[t_Size]>(a_pContentType, a_strName, const_cast<t_Ty (*)[t_Size]>(a_pPointer), a_Modifiers,
                                  a_uiFlags)
    {
    }
    VariableT(StringView a_strName, t_Ty const (*a_pPointer)[t_Size], Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : VariableT<t_Ty[t_Size]>(PHANTOM_TYPEOF(t_Ty)->addConst()->addArray(t_Size), a_strName,
                                  const_cast<t_Ty (*)[t_Size]>(a_pPointer), a_Modifiers, a_uiFlags)
    {
    }
};

template<typename t_Ty>
class VariableT<t_Ty const[]> : public VariableT<t_Ty[]>
{
public:
    VariableT(Type* a_pContentType, StringView a_strName, t_Ty const (*a_pPointer)[], Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0)
        : VariableT<t_Ty[]>(a_pContentType, a_strName, const_cast<t_Ty (*)[]>(a_pPointer), a_Modifiers, a_uiFlags)
    {
    }
    VariableT(StringView a_strName, t_Ty const (*a_pPointer)[], Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : VariableT<t_Ty[]>(PHANTOM_TYPEOF(t_Ty const[]), a_strName, const_cast<t_Ty (*)[]>(a_pPointer), a_Modifiers,
                            a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
};

} // namespace lang
} // namespace phantom

/// @endcond
