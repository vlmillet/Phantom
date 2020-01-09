// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "TemplateDependantType.h"

#include "TemplateDependantElement.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
TemplateDependantType::TemplateDependantType(TemplateDependantElement* a_pElement)
    : Type(TypeKind::TemplateDependant, a_pElement->getName(), 0, 0, a_pElement->getModifiers(),
           PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pElement(a_pElement)
{
    addElement(a_pElement);
}

} // namespace reflection
} // namespace phantom
