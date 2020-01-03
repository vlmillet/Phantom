// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "PlaceholderClassType.h"

#include "PlaceholderType.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
PlaceholderClassType::PlaceholderClassType(PlaceholderType* a_pPlaceholderType)
    : PlaceholderClassType(a_pPlaceholderType->getName(), a_pPlaceholderType->getModifiers(),
                           a_pPlaceholderType->getFlags())
{
}

PlaceholderClassType::PlaceholderClassType(StringView a_Name, Modifiers a_Modifiers, uint a_Flags)
    : ClassType(TypeKind::TemplateDependant, a_Name, 0, 0, a_Modifiers, a_Flags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT)
{
}

Placeholder* PlaceholderClassType::clone(uint a_Flags) const
{
    return PHANTOM_NEW(PlaceholderClassType)(getName(), getModifiers(), a_Flags);
}

} // namespace reflection
} // namespace phantom
