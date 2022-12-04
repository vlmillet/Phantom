// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PlaceholderClass.h"

#include "PlaceholderType.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
PlaceholderClass::PlaceholderClass(PlaceholderType* a_pPlaceholderType)
    : PlaceholderClass(a_pPlaceholderType->getName(), a_pPlaceholderType->getModifiers(),
                       a_pPlaceholderType->getFlags())
{
    m_isPack = a_pPlaceholderType->isPack();
}

PlaceholderClass::PlaceholderClass(StringView a_Name, Modifiers a_Modifiers, uint a_Flags)
    : Class(a_Name, 0, 0, a_Modifiers, a_Flags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT)
{
}

Placeholder* PlaceholderClass::clone(LanguageElement* a_pOwner, uint a_Flags) const
{
    return a_pOwner->New<PlaceholderClass>(getName(), getModifiers(), a_Flags);
}

} // namespace lang
} // namespace phantom
