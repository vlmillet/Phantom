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

/* *********************************************** */
namespace phantom
{
namespace lang
{
PlaceholderType::PlaceholderType(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(TypeKind::Unknown, a_strName, 0, 0, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
      m_pAsClass(nullptr),
      m_pAsClassType(nullptr)
{
}

Class* PlaceholderType::toClass() const
{
    if (m_pAsClass == nullptr)
    {
        m_pAsClass = const_cast<PlaceholderType*>(this)->New<PlaceholderClass>((PlaceholderType*)this);
    }
    return m_pAsClass;
}

ClassType* PlaceholderType::toClassType() const
{
    if (m_pAsClassType == nullptr)
    {
        m_pAsClassType = const_cast<PlaceholderType*>(this)->New<PlaceholderClassType>((PlaceholderType*)this);
    }
    return m_pAsClassType;
}

Placeholder* PlaceholderType::clone(LanguageElement* a_pOwner, uint) const
{
    return a_pOwner->New<PlaceholderType>(getName(), getModifiers(), 0);
}

} // namespace lang
} // namespace phantom
