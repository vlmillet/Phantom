// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/Property.h>
#include <phantom/traits/RemoveConstReference.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
template<class t_Ty, class t_ParamType, class t_ReturnType = t_ParamType, class t_SignalSign = void(t_ParamType)>
class PropertyT : public Property
{
    typedef PHANTOM_TYPENAME RemoveConstReference<t_ParamType>::type ParamTypeNoConstRef;
    typedef PHANTOM_TYPENAME RemoveConstReference<t_ReturnType>::type ReturnTypeNoConstRef;
    typedef ReturnTypeNoConstRef                                      ValueTypeNoConstRef;

    static_assert((std::is_same<ParamTypeNoConstRef, ReturnTypeNoConstRef>::value),
                  "set parameter type and get return type must be equal, modulo 'const type&'");
    static_assert(!std::is_reference<ParamTypeNoConstRef>::value,
                  "property types cannot be non-const T& references, only T const& or T");

    PHANTOM_STATIC_ASSERT(
    (std::is_same<ValueTypeNoConstRef, PHANTOM_TYPENAME RemoveConstReference<t_ParamType>::type>::value),
    "get and set functions are incompatible in this property");
    PHANTOM_STATIC_ASSERT(IsCopyable<ValueTypeNoConstRef>::value,
                          "Property effective value type (without const&) must be copyable");

public:
    typedef t_ReturnType (t_Ty::*GetterType)() const;
    typedef void (t_Ty::*SetterType)(t_ParamType);
    typedef phantom::Signal<t_SignalSign>(t_Ty::*SignalType);

public:
    PropertyT(Type* a_pFunctionsType, StringView a_strName, GetterType a_Getter, SetterType a_Setter,
              SignalType a_Signal, uint a_uiFilterMask, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Property(a_pFunctionsType, a_strName, a_uiFilterMask, a_Modifiers, PHANTOM_R_FLAG_NATIVE, 0),
          m_getter(a_Getter),
          m_setter(a_Setter),
          m_signal(a_Signal)
    {
    }

    bool isWrittable() const override
    {
        return m_setter;
    }

    bool isReadable() const override
    {
        return m_getter;
    }

    void getValue(void const* a_pObject, void* dest) const override
    {
        *((ValueTypeNoConstRef*)dest) = (reinterpret_cast<t_Ty const*>(a_pObject)->*m_getter)();
    }
    void setValue(void* a_pObject, void const* src) const override
    {
        (reinterpret_cast<t_Ty*>(a_pObject)->*m_setter)(*((ValueTypeNoConstRef*)src));
    }
    void copyValue(void* dest, void const* src) const override
    {
        (reinterpret_cast<t_Ty*>(dest)->*m_setter)((reinterpret_cast<t_Ty const*>(src)->*m_getter)());
    }

    void onAncestorChanged(LanguageElement* a_pElement) override
    {
        if (getOwner() == a_pElement)
        {
            this->setSet(getOwnerClassType()->getMethodByPtr(&m_setter));
            this->setGet(getOwnerClassType()->getMethodByPtr(&m_getter));
            auto pSignal = getOwnerClassType()->getMethodByPtr(&m_signal);
            if (pSignal && pSignal->asSignal())
                this->setSignal((Signal*)(pSignal));
        }
    }

private:
    GetterType m_getter;
    SetterType m_setter;
    SignalType m_signal;
};

template<typename t_Ty, typename t_ValueType>
class PropertyT<t_Ty, t_ValueType const, t_ValueType const, void(t_ValueType const)>
    : public PropertyT<t_Ty, t_ValueType, t_ValueType, void(t_ValueType)>
{
    typedef PropertyT<t_Ty, t_ValueType, t_ValueType, void(t_ValueType)> BaseClass;

    typedef t_ValueType const (t_Ty::*GetterType)() const;
    typedef void (t_Ty::*SetterType)(t_ValueType const);
    typedef phantom::Signal<void(t_ValueType const)>(t_Ty::*SignalType);

public:
    PropertyT(Type* a_pValueType, StringView a_strName, GetterType a_Get, SetterType a_Set, SignalType a_Signal,
              uint a_uiFilterMask, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : BaseClass(a_pValueType, a_strName, (typename BaseClass::GetterType)a_Get,
                    (typename BaseClass::SetterType)a_Set, (typename BaseClass::SignalType)a_Signal, a_uiFilterMask,
                    a_Modifiers, a_uiFlags
                    // We manage const-type attributes like no-const-type attributes : we break the
                    // const qualifier to be able to force write with "setValue"
          )
    {
    }
};

} // namespace lang
} // namespace phantom

/// @endcond
