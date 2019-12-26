// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "PlaceholderClass.h"

#include "PlaceholderType.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
PlaceholderClass::PlaceholderClass(PlaceholderType* a_pPlaceholderType)
    : PlaceholderClass(a_pPlaceholderType->getName(), a_pPlaceholderType->getModifiers(),
                       a_pPlaceholderType->getFlags())
{
}

PlaceholderClass::PlaceholderClass(StringView a_Name, Modifiers a_Modifiers, uint a_Flags)
    : Class(a_Name, 0, 0, a_Modifiers, a_Flags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT)
{
}

phantom::reflection::Placeholder* PlaceholderClass::clone(uint a_Flags) const
{
    return PHANTOM_NEW(PlaceholderClass)(getName(), getModifiers(), a_Flags);
}

} // namespace reflection
} // namespace phantom
