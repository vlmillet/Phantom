// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
    ClassType*            pClassType = a_pLanguageElement->asClassType();
    return pClassType AND getTemplateSignature()->acceptsArguments(
    pClassType->getTemplateSpecialization()->getArguments());
}

bool PlaceholderTemplate::isSame(Symbol* a_pOther) const
{
    if (Template::isSame(a_pOther))
        return true;
    Placeholder*         pPH = a_pOther->asPlaceholder();
    PlaceholderTemplate* pPHT = pPH ? pPH->asPlaceholderTemplate() : nullptr;
    return pPHT AND      pPHT->getTemplateSignature()->isSame(getTemplateSignature());
}

Placeholder* PlaceholderTemplate::clone(uint a_Flags /*= 0*/) const
{
    return PHANTOM_NEW(PlaceholderTemplate)(getName(), getTemplateSignature()->clone(a_Flags));
}

} // namespace lang
} // namespace phantom
