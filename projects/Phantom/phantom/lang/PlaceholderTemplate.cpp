// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PlaceholderTemplate.h"

#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
bool PlaceholderTemplate::accepts(LanguageElement* a_pLanguageElement) const
{
    ClassType* pClassType = a_pLanguageElement->asClassType();
    return pClassType &&
    getTemplateSignature()->acceptsArguments(pClassType->getTemplateSpecialization()->getArguments());
}

Placeholder* PlaceholderTemplate::clone(uint a_Flags /*= 0*/) const
{
    return PHANTOM_NEW(PlaceholderTemplate)(getName(), getTemplateSignature()->clone(a_Flags));
}

} // namespace lang
} // namespace phantom
