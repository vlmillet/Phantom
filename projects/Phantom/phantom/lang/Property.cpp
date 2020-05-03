// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Property.h"

#include "Parameter.h"
#include "Signature.h"

/* *********************************************** */

namespace phantom
{
namespace lang
{
static Type* Property_ValueTypeFromParamType(Type* a_pParamType)
{
    PHANTOM_ASSERT(a_pParamType->asReference() == nullptr || a_pParamType->removeReference()->asConstType());
    return a_pParamType->removeReference()->removeQualifiers();
}

Property::Property(Type* a_pFunctionsType, StringView a_strName, uint a_uiFilterMask, Modifiers a_Modifiers /*= 0*/,
                   uint a_uiFlags /*= 0*/)
    : ValueMember(Property_ValueTypeFromParamType(a_pFunctionsType), a_strName, a_uiFilterMask, a_Modifiers, a_uiFlags),
      m_pFunctionsType(a_pFunctionsType)
{
    PHANTOM_ASSERT(m_pValueType->isCopyable());
    if ((a_Modifiers & (PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET)) ==
        (PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET))
    {
        addModifiers(PHANTOM_R_VIRTUAL);
    }
    else if ((a_Modifiers & PHANTOM_R_VIRTUAL) == PHANTOM_R_VIRTUAL)
    {
		addModifiers(Modifiers(PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET));
    }
}

Property::Property(Type* a_pFunctionsType, StringView a_strName, uint a_uiFilterMask, Modifiers a_Modifiers,
                   uint a_uiFlags, int)
    : ValueMember(Property_ValueTypeFromParamType(a_pFunctionsType), a_strName, a_uiFilterMask, a_Modifiers, a_uiFlags),
      m_pFunctionsType(a_pFunctionsType)
{
    PHANTOM_ASSERT(m_pValueType->isCopyable());
    if ((a_Modifiers & (PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET)) ==
        (PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET))
    {
		addModifiers(PHANTOM_R_VIRTUAL);
    }
    else if ((a_Modifiers & PHANTOM_R_VIRTUAL) == PHANTOM_R_VIRTUAL)
    {
		addModifiers(Modifiers(PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET));
    }
}

void Property::setSet(Method* a_pFunc)
{
    PHANTOM_ASSERT(!a_pFunc || a_pFunc->getParameters().size() == 1);
    PHANTOM_ASSERT(
    !a_pFunc || a_pFunc->getParameters()[0]->getValueType()->removeReference()->removeQualifiers() == getValueType());
    a_pFunc->setOwner(this);
    m_pSet = a_pFunc;
}

void Property::setGet(Method* a_pFunc)
{
    PHANTOM_ASSERT(!a_pFunc || a_pFunc->getReturnType()->removeReference()->removeQualifiers() == getValueType());
    a_pFunc->setOwner(this);
    m_pGet = a_pFunc;
}

void Property::getValue(void const* a_pObject, void* a_pDest) const
{
    PHANTOM_ASSERT(m_pGet);
    m_pGet->invoke((void*)a_pObject, nullptr, a_pDest);
}

void Property::setValue(void* a_pObject, void const* a_pSrc) const
{
    PHANTOM_ASSERT(m_pSet);
    void* args[1] = {(void*)a_pSrc};
    m_pSet->invoke(a_pObject, args);
}

Method* Property::addSet(StringView a_strName /*= ""*/)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(!testFlags(PHANTOM_R_FLAG_READONLY));
    PHANTOM_ASSERT(getOwner() != nullptr);
    m_pSet =
    New<Method>(a_strName.empty() ? StringView("_PHNTM_" + getName() + "_set") : a_strName,
                Signature::Create(this, PHANTOM_TYPEOF(void), getValueType()), PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
    m_pSet->m_pProperty = this;
    return m_pSet;
}

Method* Property::addGet(StringView a_strName /*= ""*/)
{
    PHANTOM_ASSERT(!isNative());
    PHANTOM_ASSERT(getOwner() != nullptr);
    m_pGet = New<Method>(a_strName.empty() ? StringView("_PHNTM_" + getName() + "_get") : a_strName,
                         Signature::Create(this, getValueType()), PHANTOM_R_CONST, PHANTOM_R_FLAG_IMPLICIT);
    m_pGet->m_pProperty = this;
    return m_pGet;
}

} // namespace lang
} // namespace phantom
