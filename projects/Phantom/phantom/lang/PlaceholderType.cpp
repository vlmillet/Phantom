// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PlaceholderType.h"

#include "PlaceholderClass.h"
#include "PlaceholderClassType.h"
#include "TemplateSpecialization.h"

#include <phantom/detail/new.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
PlaceholderType::PlaceholderType(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(TypeKind::Placeholder, a_strName, 0, 0, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
      m_pAsClass(nullptr),
      m_pAsClassType(nullptr)
{
}

Class* PlaceholderType::asClass() const
{
    if (m_pAsClass == nullptr)
    {
        m_pAsClass = PHANTOM_NEW(PlaceholderClass)((PlaceholderType*)this);
        ((PlaceholderType*)this)->addElement(m_pAsClass);
    }
    return m_pAsClass;
}

ClassType* PlaceholderType::asClassType() const
{
    if (m_pAsClassType == nullptr)
    {
        m_pAsClassType = PHANTOM_NEW(PlaceholderClassType)((PlaceholderType*)this);
        ((PlaceholderType*)this)->addElement(m_pAsClassType);
    }
    return m_pAsClassType;
}

bool PlaceholderType::isSame(Symbol* a_pOther) const
{
    if (this == a_pOther)
        return true;
    TemplateSpecialization* pSpec = getEnclosingTemplateSpecialization();
    TemplateSpecialization* pOtherSpec = a_pOther->getEnclosingTemplateSpecialization();
    return (a_pOther->getName() ==
            getName() && a_pOther->asPlaceholder() && pSpec && pOtherSpec && pSpec->isSame(pOtherSpec));
}

phantom::lang::Placeholder* PlaceholderType::clone(uint) const
{
    return PHANTOM_NEW(PlaceholderType)(getName(), getModifiers(), 0);
}

} // namespace lang
} // namespace phantom
