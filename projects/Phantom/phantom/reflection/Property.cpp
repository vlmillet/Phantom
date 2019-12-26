// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Property.h"

#include "Parameter.h"
#include "Signature.h"
#include "phantom/new.h"

#include <phantom/reflection/Signal.h>
/* *********************************************** */

namespace phantom
{
namespace reflection
{
static Type* Property_ValueTypeFromParamType(Type* a_pParamType)
{
    PHANTOM_ASSERT(a_pParamType->asReference() == nullptr OR a_pParamType->removeReference()->asConstType());
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
        m_Modifiers |= PHANTOM_R_VIRTUAL;
    }
    else if ((a_Modifiers & PHANTOM_R_VIRTUAL) == PHANTOM_R_VIRTUAL)
    {
        m_Modifiers |= Modifiers(PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET);
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
        m_Modifiers |= PHANTOM_R_VIRTUAL;
    }
    else if ((a_Modifiers & PHANTOM_R_VIRTUAL) == PHANTOM_R_VIRTUAL)
    {
        m_Modifiers |= Modifiers(PHANTOM_R_VIRTUAL_GET | PHANTOM_R_VIRTUAL_SET);
    }
    if (m_pSignal)
    {
        addReferencedElement(m_pSignal);
        m_pSignal->m_pProperty = this;
    }
}

PHANTOM_DTOR Property::~Property()
{
}

void Property::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    ValueMember::onReferencedElementRemoved(a_pElement);
}

void Property::onElementRemoved(LanguageElement* a_pElement)
{
    ValueMember::onElementRemoved(a_pElement);
    if (m_pSignal == a_pElement)
        m_pSignal = nullptr;
    else if (m_pGet == a_pElement)
        m_pGet = nullptr;
    else if (m_pSet == a_pElement)
        m_pSet = nullptr;
}

void Property::setSet(Method* a_pFunc)
{
    PHANTOM_ASSERT(!a_pFunc || a_pFunc->getParameters().size() == 1);
    PHANTOM_ASSERT(
    !a_pFunc || a_pFunc->getParameters()[0]->getValueType()->removeReference()->removeQualifiers() == getValueType());
    m_pSet = a_pFunc;
}

void Property::setGet(Method* a_pFunc)
{
    PHANTOM_ASSERT(!a_pFunc || a_pFunc->getReturnType()->removeReference()->removeQualifiers() == getValueType());
    m_pGet = a_pFunc;
}

void Property::setSignal(Signal* a_pFunc)
{
    PHANTOM_ASSERT(
    !a_pFunc ||
    (a_pFunc->getParameters().empty() ||
     (a_pFunc->getParameters().size() == 1 &&
      a_pFunc->getParameters()[0]->getValueType()->removeReference()->removeQualifiers() == getValueType())));
    m_pSignal = a_pFunc;
}

void Property::getValue(void const* a_pObject, void* a_pDest) const
{
    if (m_pGet == nullptr)
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "No get member function defined for this property");
    }
    m_pGet->invoke((void*)a_pObject, nullptr, a_pDest);
}

void Property::setValue(void* a_pObject, void const* a_pSrc) const
{
    if (m_pSet == nullptr)
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "No set member function defined for this property");
    }
    void* args[1] = {(void*)a_pSrc};
    m_pSet->invoke(a_pObject, args);
}

void Property::onAncestorChanged(LanguageElement* a_pOwner)
{
    ValueMember::onAncestorChanged(a_pOwner);
    if (getOwner() == a_pOwner)
    {
        if (m_pGet AND m_pGet->getOwner() == nullptr)
        {
            static_cast<ClassType*>(getOwner())->addMethod(m_pGet);
        }
        else
        {
            PHANTOM_ASSERT(m_pGet == nullptr OR getOwner() == m_pGet->getOwner());
        }
        if (m_pSet AND m_pSet->getOwner() == nullptr)
        {
            static_cast<ClassType*>(getOwner())->addMethod(m_pSet);
        }
        else
        {
            PHANTOM_ASSERT(m_pSet == nullptr OR getOwner() == m_pSet->getOwner());
        }
        if (m_pSignal AND m_pSignal->getOwner() == nullptr)
        {
            static_cast<ClassType*>(getOwner())->addMethod(m_pSignal);
        }
        else
        {
            PHANTOM_ASSERT(m_pSignal == nullptr OR getOwner() == m_pSignal->getOwner());
        }
    }
}

Method* Property::addSet(StringView a_strName /*= ""*/)
{
    if (isNative())
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "cannot add custom property functions to native properties");
        return nullptr;
    }
    if (testFlags(PHANTOM_R_FLAG_READONLY))
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "cannot add set function to read only properties");
        return nullptr;
    }
    if (getOwner() == nullptr)
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "property must have an owner before begin add member functions");
        return nullptr;
    }
    addReferencedElement(m_pSet =
                         PHANTOM_NEW(Method)(a_strName.empty() ? StringView("_PHNTM_" + m_strName + "_set") : a_strName,
                                             Signature::Create(PHANTOM_TYPEOF(void), getValueType()), 0,
                                             PHANTOM_R_FLAG_PRIVATE_VIS | PHANTOM_R_FLAG_IMPLICIT));
    getOwner()->addElement(m_pSet);
    m_pSet->m_pProperty = this;
    return m_pSet;
}

Signal* Property::addSignal(StringView a_strName /*= ""*/)
{
    if (isNative())
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "cannot add custom property functions to native properties");
        return nullptr;
    }
    if (getOwner() == nullptr)
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "property must have an owner before begin add member functions");
        return nullptr;
    }
    addReferencedElement(
    m_pSignal = PHANTOM_NEW(Signal)(a_strName.empty() ? StringView("_PHNTM_" + m_strName + "_signal") : a_strName,
                                    Signature::Create(PHANTOM_TYPEOF(void), getValueType()), 0,
                                    PHANTOM_R_FLAG_PRIVATE_VIS | PHANTOM_R_FLAG_IMPLICIT));
    getOwner()->addElement(m_pSignal);
    m_pSignal->m_pProperty = this;
    return m_pSignal;
}

Method* Property::addGet(StringView a_strName /*= ""*/)
{
    if (isNative())
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "cannot add custom property functions to native properties");
        return nullptr;
    }
    if (getOwner() == nullptr)
    {
        PHANTOM_THROW_EXCEPTION(RuntimeException, "property must have an owner before begin add member functions");
        return nullptr;
    }
    addReferencedElement(m_pGet =
                         PHANTOM_NEW(Method)(a_strName.empty() ? StringView("_PHNTM_" + m_strName + "_get") : a_strName,
                                             Signature::Create(getValueType()), PHANTOM_R_CONST,
                                             PHANTOM_R_FLAG_PRIVATE_VIS | PHANTOM_R_FLAG_IMPLICIT));
    getOwner()->addElement(m_pGet);
    m_pGet->m_pProperty = this;
    return m_pGet;
}

} // namespace reflection
} // namespace phantom
