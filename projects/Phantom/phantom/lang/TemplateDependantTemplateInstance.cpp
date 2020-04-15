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
    : ClassType(TypeKind::ClassType, a_pTemplateSpecialization->getTemplate()->getName(), 0, 0, 0,
                a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pTemplate(a_pTemplateSpecialization->getTemplate()),
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
    : ClassType(TypeKind::ClassType, a_pTemplate->getName(), 0, 0, 0,
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

void TemplateDependantTemplateInstance::getDecoration(StringBuffer& a_Buf) const
{
    a_Buf += '<';
    for (size_t i = 0; i < m_Arguments.size(); ++i)
    {
        if (i)
            a_Buf += ',';
        m_Arguments[i]->getRelativeDecoratedName(m_pTemplate, a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
}

void TemplateDependantTemplateInstance::getQualifiedDecoration(StringBuffer& a_Buf) const
{
    a_Buf += '<';
    for (size_t i = 0; i < m_Arguments.size(); ++i)
    {
        if (i)
            a_Buf += ',';
        m_Arguments[i]->getQualifiedDecoratedName(a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
}

void TemplateDependantTemplateInstance::getDecoratedName(StringBuffer& a_Buf) const
{
    if (m_strName.empty())
        formatAnonymousName(a_Buf);
    else
        getName(a_Buf);
    getDecoration(a_Buf);
}

void TemplateDependantTemplateInstance::getQualifiedName(StringBuffer& a_Buf) const
{
    return m_pTemplate->getQualifiedName(a_Buf);
}

void TemplateDependantTemplateInstance::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getQualifiedName(a_Buf);
    getQualifiedDecoration(a_Buf);
}

void TemplateDependantTemplateInstance::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    LanguageElement::getRelativeDecoratedName(a_pTo, a_Buf);
}

hash64 TemplateDependantTemplateInstance::computeLocalHash() const
{
    hash64     h = 0;
    StringView n = getName();
    if (n.empty())
    {
        CombineHash(h, hash64(this));
    }
    else
    {
        h = ComputeHash(n.data(), n.size());
    }
    for (auto pArg : m_Arguments)
    {
        CombineHash(h, pArg->asSymbol()->getHash());
    }
    return h;
}

} // namespace lang
} // namespace phantom
