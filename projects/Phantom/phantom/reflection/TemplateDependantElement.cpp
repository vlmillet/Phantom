// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "TemplateDependantElement.h"

#include "TemplateDependantType.h"

#include <phantom/detail/new.h>
/* *********************************************** */

namespace phantom
{
namespace reflection
{
TemplateDependantElement::TemplateDependantElement(LanguageElement* a_pLeft, StringView a_strName,
                                                   OptionalArrayView<LanguageElement*> a_pTemplateArguments /*= 0*/,
                                                   OptionalArrayView<LanguageElement*> a_pFunctionArguments /*= 0*/,
                                                   Modifiers modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, modifiers, a_uiFlags | PHANTOM_R_FLAG_TEMPLATE_DEPENDANT | PHANTOM_R_FLAG_PRIVATE_VIS),
      m_pLeft(a_pLeft),
      m_pTemplateArguments(a_pTemplateArguments ? PHANTOM_NEW(LanguageElements)(*a_pTemplateArguments) : nullptr),
      m_pFunctionArguments(a_pFunctionArguments ? PHANTOM_NEW(LanguageElements)(*a_pFunctionArguments) : nullptr)
{
    if (m_pLeft)
    {
        if (m_pLeft->getOwner() == nullptr)
        {
            addElement(m_pLeft);
        }
        else
        {
            addReferencedElement(m_pLeft);
        }
    }
    if (m_pTemplateArguments)
    {
        for (auto pElem : *m_pTemplateArguments)
        {
            if (pElem->getOwner())
            {
                addReferencedElement(pElem);
            }
            else
            {
                addElement(pElem);
            }
        }
    }
    if (m_pFunctionArguments)
    {
        for (auto pElem : *m_pFunctionArguments)
        {
            if (pElem->getOwner())
            {
                addReferencedElement(pElem);
            }
            else
            {
                addElement(pElem);
            }
        }
    }
}

TemplateDependantElement::~TemplateDependantElement()
{
    PHANTOM_DELETE(LanguageElements) m_pTemplateArguments;
    PHANTOM_DELETE(LanguageElements) m_pFunctionArguments;
}

Type* TemplateDependantElement::toType() const
{
    return PHANTOM_NEW(TemplateDependantType)((TemplateDependantElement*)this);
}

} // namespace reflection
} // namespace phantom
