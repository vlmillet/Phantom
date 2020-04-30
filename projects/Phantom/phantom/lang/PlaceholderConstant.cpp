// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PlaceholderConstant.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
bool PlaceholderConstant::accepts(LanguageElement* a_pLanguageElement) const
{
    return a_pLanguageElement->removeExpression()->asConstant() != nullptr;
}

Constant* PlaceholderConstant::cloneImpl(LanguageElement* a_pOwner) const
{
    return New<PlaceholderConstant>(getValueType(), getName(), getModifiers(), 0);
}

Placeholder* PlaceholderConstant::clone(uint a_Flags) const
{
    return New<PlaceholderConstant>(getValueType(), getName(), getModifiers(), a_Flags);
}

} // namespace lang
} // namespace phantom
