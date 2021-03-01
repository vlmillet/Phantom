// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "TemplateDependantType.h"

#include "TemplateDependantElement.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemplateDependantType::TemplateDependantType(TemplateDependantElement* a_pElement)
    : Type(TypeKind::Unknown, a_pElement->getName(), 0, 0, a_pElement->getModifiers(),
           PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
      m_pElement(a_pElement)
{
}

void TemplateDependantType::getDecoratedName(StringBuffer& a_Buf) const
{
    return m_pElement->getDecoratedName(a_Buf);
}

void TemplateDependantType::getQualifiedName(StringBuffer& a_Buf) const
{
    return m_pElement->getQualifiedName(a_Buf);
}

void TemplateDependantType::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    return m_pElement->getRelativeName(a_pTo, a_Buf);
}

void TemplateDependantType::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    return m_pElement->getQualifiedDecoratedName(a_Buf);
}

void TemplateDependantType::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    return m_pElement->getRelativeDecoratedName(a_pTo, a_Buf);
}

} // namespace lang
} // namespace phantom
