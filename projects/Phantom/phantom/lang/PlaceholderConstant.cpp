// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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

bool PlaceholderConstant::isSame(Symbol* a_pOther) const
{
    if (Constant::isSame(a_pOther))
        return true;
    Placeholder*         pPH = a_pOther->asPlaceholder();
    PlaceholderConstant* pPHC = pPH ? pPH->asPlaceholderConstant() : nullptr;
    return pPHC AND      pPHC->m_pValueType->isSame(m_pValueType);
}

Constant* PlaceholderConstant::cloneImpl() const
{
    return PHANTOM_NEW(PlaceholderConstant)(getValueType(), getName(), getModifiers(), 0);
}

Placeholder* PlaceholderConstant::clone(uint a_Flags) const
{
    return PHANTOM_NEW(PlaceholderConstant)(getValueType(), getName(), getModifiers(), a_Flags);
}

} // namespace lang
} // namespace phantom
