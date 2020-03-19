// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "TemplateDependantTemplateInstance.h"

#include "phantom/lang/Placeholder.h"

#include <phantom/lang/Template.h>
#include <phantom/lang/TemplateSpecialization.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemplateDependantTemplateInstance::TemplateDependantTemplateInstance(TemplateSpecialization* a_pTemplateSpecialization,
                                                                     const LanguageElements& a_Arguments,
                                                                     uint                    a_uiFlags)
    : ClassType(TypeKind::TemplateDependant,
                (a_pTemplateSpecialization && a_pTemplateSpecialization->getTemplate())
                ? a_pTemplateSpecialization->getTemplate()->getName()
                : "",
                0, 0, 0, a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pTemplate(a_pTemplateSpecialization ? a_pTemplateSpecialization->getTemplate() : nullptr),
      m_pTemplateSpecialization(a_pTemplateSpecialization),
      m_Arguments(a_Arguments)
{
    PHANTOM_ASSERT(m_pTemplate);
    addReferencedElement(m_pTemplate);
    PHANTOM_ASSERT(m_pTemplateSpecialization);
    addReferencedElement(m_pTemplateSpecialization);
    for (auto pArg : m_Arguments)
    {
        if (pArg->getOwner())
            addReferencedElement(pArg);
        else
            addElement(pArg);
    }
}

TemplateDependantTemplateInstance::TemplateDependantTemplateInstance(Template*               a_pTemplate,
                                                                     const LanguageElements& a_Arguments,
                                                                     uint                    a_uiFlags)
    : ClassType(TypeKind::TemplateDependant, a_pTemplate->getName(), 0, 0, 0,
                PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS | a_uiFlags),
      m_pTemplate(a_pTemplate),
      m_Arguments(a_Arguments)
{
    PHANTOM_ASSERT(m_pTemplate);
    for (auto pArg : m_Arguments)
    {
        if (pArg->getOwner())
            addReferencedElement(pArg);
        else
            addElement(pArg);
    }
}

bool TemplateDependantTemplateInstance::isSame(Symbol* a_pOther) const
{
    if (Type::isSame(a_pOther))
        return true;
    Placeholder*                       pPH = a_pOther->asPlaceholder();
    TemplateDependantTemplateInstance* pTDTI =
    pPH ? phantom::Object::Cast<TemplateDependantTemplateInstance>(pPH->asSymbol()) : nullptr;
    if (pTDTI == nullptr || !(pTDTI->m_pTemplate->isSame(m_pTemplate)))
        return false;
    size_t count = m_Arguments.size();
    if (count != pTDTI->m_Arguments.size())
        return false;
    for (size_t i = 0; i < count; ++i)
    {
        if (!(m_Arguments[i]->isSame(pTDTI->m_Arguments[i])))
            return false;
    }
    return true;
}

} // namespace lang
} // namespace phantom
