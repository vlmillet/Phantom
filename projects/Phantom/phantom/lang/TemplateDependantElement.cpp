// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "TemplateDependantElement.h"

#include "TemplateDependantType.h"

/* *********************************************** */

namespace phantom
{
namespace lang
{
TemplateDependantElement::TemplateDependantElement(LanguageElement* a_pLeft, StringView a_strName,
                                                   OptionalArrayView<LanguageElement*> a_pTemplateArguments /*= 0*/,
                                                   OptionalArrayView<LanguageElement*> a_pFunctionArguments /*= 0*/,
                                                   Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, modifiers, a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pLeft(a_pLeft),
      m_pTemplateArguments(a_pTemplateArguments ? new_<LanguageElements>(*a_pTemplateArguments) : nullptr),
      m_pFunctionArguments(a_pFunctionArguments ? new_<LanguageElements>(*a_pFunctionArguments) : nullptr)
{
    if (m_pLeft)
    {
        addReferencedElement(m_pLeft);
    }
    if (m_pTemplateArguments)
    {
        for (auto pElem : *m_pTemplateArguments)
        {
            addReferencedElement(pElem);
        }
    }
    if (m_pFunctionArguments)
    {
        for (auto pElem : *m_pFunctionArguments)
        {
            addReferencedElement(pElem);
        }
    }
}

TemplateDependantElement::~TemplateDependantElement()
{
    if (m_pTemplateArguments)
        delete_<LanguageElements>(m_pTemplateArguments);
    if (m_pFunctionArguments)
        delete_<LanguageElements>(m_pFunctionArguments);
}

Type* TemplateDependantElement::toType() const
{
    return const_cast<TemplateDependantElement*>(this)->New<TemplateDependantType>((TemplateDependantElement*)this);
}

void TemplateDependantElement::getRelativeDecoration(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (!m_pTemplateArguments)
        return;
    a_Buf += '<';
    for (size_t i = 0; i < (*m_pTemplateArguments).size(); ++i)
    {
        if (i)
            a_Buf += ',';
        (*m_pTemplateArguments)[i]->getRelativeDecoratedName(a_pTo, a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
}

void TemplateDependantElement::getDecoration(StringBuffer& a_Buf) const
{
    if (!m_pTemplateArguments)
        return;
    a_Buf += '<';
    for (size_t i = 0; i < (*m_pTemplateArguments).size(); ++i)
    {
        if (i)
            a_Buf += ',';
        (*m_pTemplateArguments)[i]->getDecoratedName(a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
}

void TemplateDependantElement::getQualifiedDecoration(StringBuffer& a_Buf) const
{
    if (!m_pTemplateArguments)
        return;
    a_Buf += '<';
    for (size_t i = 0; i < m_pTemplateArguments->size(); ++i)
    {
        if (i)
            a_Buf += ',';
        (*m_pTemplateArguments)[i]->getQualifiedDecoratedName(a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
}

void TemplateDependantElement::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    if (m_pLeft)
    {
        m_pLeft->getQualifiedDecoratedName(a_Buf);
        a_Buf += "::";
    }
    getName(a_Buf);
    getQualifiedDecoration(a_Buf);
}

void TemplateDependantElement::getDecoratedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
    getDecoration(a_Buf);
}

void TemplateDependantElement::getQualifiedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
}

void TemplateDependantElement::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (m_pLeft)
    {
        m_pLeft->getRelativeDecoratedName(a_pTo, a_Buf);
        a_Buf += "::";
    }
    getName(a_Buf);
    getRelativeDecoration(a_pTo, a_Buf);
}

void TemplateDependantElement::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (m_pLeft)
    {
        m_pLeft->getRelativeDecoratedName(a_pTo, a_Buf);
        a_Buf += "::";
    }
    getName(a_Buf);
}

} // namespace lang
} // namespace phantom
