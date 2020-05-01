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
    : Type(TypeKind::Unknown, a_pTemplateSpecialization->getTemplate()->getName(), 0, 0, 0,
           a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT),
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
        addReferencedElement(pArg);
    }
}

TemplateDependantTemplateInstance::TemplateDependantTemplateInstance(Template*               a_pTemplate,
                                                                     const LanguageElements& a_Arguments,
                                                                     uint                    a_uiFlags)
    : Type(TypeKind::Unknown, a_pTemplate->getName(), 0, 0, 0, PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | a_uiFlags),
      m_pTemplate(a_pTemplate),
      m_Arguments(a_Arguments)
{
    PHANTOM_ASSERT(m_pTemplate);
    for (auto pArg : m_Arguments)
    {
        addReferencedElement(pArg);
    }
}

bool TemplateDependantTemplateInstance::isSame(Symbol* a_pOther) const
{
    if (Type::isSame(a_pOther))
        return true;
    if (a_pOther->getMetaClass() == PHANTOM_CLASSOF(TemplateDependantClassPromotion))
    {
        a_pOther = static_cast<TemplateDependantClassPromotion*>(a_pOther)->getBase();
    }
    if (a_pOther->getMetaClass() == PHANTOM_CLASSOF(TemplateDependantTemplateInstance))
    {
        TemplateDependantTemplateInstance* pTDTI = static_cast<TemplateDependantTemplateInstance*>(a_pOther);
        size_t                             count = m_Arguments.size();
        if (count != pTDTI->m_Arguments.size() || !pTDTI->m_pTemplate->isSame(m_pTemplate))
            return false;
        for (size_t i = 0; i < count; ++i)
        {
            if (!(m_Arguments[i]->isSame(pTDTI->m_Arguments[i])))
                return false;
        }
        return true;
    }
    return false;
}

void TemplateDependantTemplateInstance::getRelativeDecoration(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    a_Buf += '<';
    for (size_t i = 0; i < m_Arguments.size(); ++i)
    {
        if (i)
            a_Buf += ',';
        m_Arguments[i]->getRelativeDecoratedName(a_pTo, a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
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

void TemplateDependantTemplateInstance::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    return m_pTemplate->getRelativeName(a_pTo, a_Buf);
}

void TemplateDependantTemplateInstance::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    getRelativeName(a_pTo, a_Buf);
    getRelativeDecoration(a_pTo, a_Buf);
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

Class* TemplateDependantTemplateInstance::promoteAsClass()
{
    if (m_pAsClass == nullptr)
        m_pAsClass = New<TemplateDependantClassPromotion>(this);
    return m_pAsClass;
}

void TemplateDependantClassPromotion::visit(phantom::lang::LanguageElementVisitor* a_pVisitor,
                                            phantom::lang::VisitorData             a_Data)
{
    m_pBase->visit(a_pVisitor, a_Data); // we directly visit the base as this is what really matters
}

TemplateDependantClassPromotion::TemplateDependantClassPromotion(Type* a_pBase)
    : Class(TypeKind::Class, a_pBase->getName(), a_pBase->getModifiers(), a_pBase->getFlags()), m_pBase(a_pBase)
{
}

void TemplateDependantClassPromotion::getDecoratedName(StringBuffer& a_Buf) const
{
    return m_pBase->getDecoratedName(a_Buf);
}

void TemplateDependantClassPromotion::getQualifiedName(StringBuffer& a_Buf) const
{
    return m_pBase->getQualifiedName(a_Buf);
}

void TemplateDependantClassPromotion::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    return m_pBase->getQualifiedDecoratedName(a_Buf);
}

void TemplateDependantClassPromotion::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    m_pBase->getRelativeName(a_pTo, a_Buf);
}

void TemplateDependantClassPromotion::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    m_pBase->getRelativeDecoratedName(a_pTo, a_Buf);
}

phantom::hash64 TemplateDependantClassPromotion::computeLocalHash() const
{
    return static_cast<TemplateDependantClassPromotion*>(m_pBase)->computeLocalHash();
}

bool TemplateDependantClassPromotion::isSame(Symbol* a_pOther) const
{
    return m_pBase->isSame(a_pOther);
}

} // namespace lang
} // namespace phantom
