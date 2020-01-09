// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "TemplateSpecialization.h"

#include "Placeholder.h"
#include "Source.h"
#include "Template.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"

#include <phantom/detail/new.h>
#include <phantom/utils/SmallSet.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
/// Full specialization constructor
TemplateSpecialization::TemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pSignature,
                                               const LanguageElements& arguments, Symbol* a_pTemplated, uint a_Flags)
    : Symbol(a_pTemplate->getName(), a_pTemplate->getModifiers(), a_Flags),
      m_pTemplate(a_pTemplate),
      m_pTemplateSignature(a_pSignature),
      m_pTemplated(a_pTemplated)
{
    // accepts only if : (template has no source) OR (template is native and we are currently
    // registering native types ) OR source is not private (not an obsolete archive stored for
    // revert while run-time building)
    PHANTOM_ASSERT(a_pTemplate->getSource() ==
                   nullptr OR(a_pTemplate->isNative())
                   OR NOT(a_pTemplate->getSource()->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)));
    m_Arguments.resize(arguments.size());
    for (size_t i = 0; i < arguments.size(); ++i)
    {
        setArgument(i, arguments[i]);
    }
    PHANTOM_ASSERT(m_pTemplated);
    addElement(m_pTemplated);
    PHANTOM_ASSERT(m_pTemplateSignature);
    addElement(m_pTemplateSignature);
    PHANTOM_ASSERT(m_pTemplate);
    addReferencedElement(m_pTemplate);
    m_pTemplate->addTemplateSpecialization(this);
    if (!isFull())
    {
        m_pTemplated->setTemplateDependant();
    }
}

/// Partial specialization constructor
TemplateSpecialization::TemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pSignature,
                                               const LanguageElements& arguments, uint a_Flags)
    : Symbol(a_pTemplate->getName(), a_pTemplate->getModifiers(), a_Flags),
      m_pTemplate(a_pTemplate),
      m_pTemplateSignature(a_pSignature)
{
    // accepts only if : (template has no source) OR (template is native and we are currently
    // registering native types ) OR source is not private (not an obsolete archive stored for
    // revert while run-time building)
    PHANTOM_ASSERT(a_pTemplate->getSource() ==
                   nullptr OR(a_pTemplate->isNative())
                   OR NOT(a_pTemplate->getSource()->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)));
    m_Arguments.resize(arguments.size());
    for (size_t i = 0; i < arguments.size(); ++i)
    {
        setArgument(i, arguments[i]);
    }
    PHANTOM_ASSERT(m_pTemplateSignature);
    addElement(m_pTemplateSignature);
    PHANTOM_ASSERT(m_pTemplate);
    addReferencedElement(m_pTemplate);
    m_pTemplate->addTemplateSpecialization(this);
}

TemplateSpecialization::TemplateSpecialization(TemplateSpecialization* a_pInstantiationSpecialization,
                                               const LanguageElements& arguments,
                                               const PlaceholderMap&   a_PlaceholderSubstitutions)
    : Symbol(a_pInstantiationSpecialization->getTemplate()->getName(), 0,
             PHANTOM_R_FLAG_IMPLICIT | PHANTOM_R_FLAG_PRIVATE_VIS) // instantiations are considered implicit
      ,
      m_pTemplate(a_pInstantiationSpecialization->getTemplate()),
      m_pInstantiationSpecialization(a_pInstantiationSpecialization),
      m_PlaceholderSubstitutions(a_PlaceholderSubstitutions)
{
#if PHANTOM_DEBUG_LEVEL
    // ensure placeholders belongs to the instantiation specialization and are not used multiple
    // times
    {
        SmallSet<Placeholder*> encountered;
        for (auto& pair : a_PlaceholderSubstitutions)
        {
            bool  found = false;
            auto& params = a_pInstantiationSpecialization->getTemplateParameters();
            for (size_t i = 0; i < params.size(); ++i)
            {
                if (params[i]->getPlaceholder()->isSame(pair.first))
                {
                    found = true;
                    break;
                }
            }
            PHANTOM_ASSERT(encountered.insert(pair.first).second == true, "duplicate of placeholder");
            PHANTOM_ASSERT(found);
        }
    }
#endif
    m_Arguments.resize(arguments.size());
    for (size_t i = 0; i < arguments.size(); ++i)
    {
        setArgument(i, arguments[i]);
    }
    addReferencedElement(m_pInstantiationSpecialization);
    PHANTOM_ASSERT(m_pTemplate);
    addReferencedElement(m_pTemplate);
    m_pTemplate->addTemplateSpecialization(this);
}

TemplateSpecialization::~TemplateSpecialization()
{
    if (isNative())
    {
        if (m_pDefaultArguments)
            PHANTOM_DELETE(LanguageElements) m_pDefaultArguments;
    }
}

void TemplateSpecialization::getDecoration(StringBuffer& a_Buf) const
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

void TemplateSpecialization::getQualifiedDecoration(StringBuffer& a_Buf) const
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

void TemplateSpecialization::getDecoratedName(StringBuffer& a_Buf) const
{
    getName(a_Buf);
    getDecoration(a_Buf);
}

void TemplateSpecialization::getQualifiedName(StringBuffer& a_Buf) const
{
    m_pTemplate->getQualifiedName(a_Buf);
}

void TemplateSpecialization::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getQualifiedName(a_Buf);
    getQualifiedDecoration(a_Buf);
}

LanguageElement* TemplateSpecialization::getArgument(StringView a_strParameterName) const
{
    size_t index = m_pTemplate->getTemplateParameterIndex(a_strParameterName);
    return (index != ~size_t(0)) ? m_Arguments[index] : nullptr;
}

Scope* TemplateSpecialization::getNamingScope() const
{
    return m_pTemplate->getNamingScope();
}

Type* TemplateSpecialization::getArgumentAsType(StringView a_strParameterName) const
{
    LanguageElement* pTemplateElement = getArgument(a_strParameterName);
    if (pTemplateElement)
        return pTemplateElement->asType();
    return nullptr;
}

void TemplateSpecialization::setArgument(size_t a_uiIndex, LanguageElement* a_pElement)
{
    size_t index = a_uiIndex;
    PHANTOM_ASSERT(!isNative() OR a_pElement,
                   "invalid template argument ; if argument is a type, check that your type has "
                   "lang declared before its use in any template signature");
    PHANTOM_ASSERT(index != ~size_t(0));
    PHANTOM_ASSERT(m_Arguments[index] == nullptr);
    if (a_pElement == nullptr)
        setInvalid();
    m_Arguments[index] = a_pElement;
    if (a_pElement)
    {
        if ((a_pElement->asPlaceholder() OR NOT(a_pElement->asType()))AND a_pElement->getOwner() == nullptr)
            addElement(a_pElement);
        else
            addReferencedElement(a_pElement);
    }
}

void TemplateSpecialization::setDefaultArgument(StringView a_strParameterName, LanguageElement* a_pElement)
{
    size_t index = m_pTemplate->getTemplateParameterIndex(a_strParameterName);
    PHANTOM_ASSERT(index != ~size_t(0), "no template parameter found matching given name '%.*s'",
                   PHANTOM_STRING_AS_PRINTF_ARG(a_strParameterName));
    setDefaultArgument(index, a_pElement);
}

void TemplateSpecialization::setDefaultArgument(size_t index, LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(index < getArgumentCount());
    PHANTOM_ASSERT(isFull() AND isNative(), "can only set default arguments for native full specializations");
    PHANTOM_ASSERT(a_pElement);
    PHANTOM_ASSERT(getDefaultArgument(index) == nullptr, "default argument already defined");
    if (m_pDefaultArguments == nullptr)
    {
        m_pDefaultArguments = PHANTOM_NEW(LanguageElements);
        m_pDefaultArguments->resize(m_pTemplate->getTemplateParameters().size(), nullptr);
    }
    (*m_pDefaultArguments)[index] = a_pElement;
    if ((a_pElement->asPlaceholder() OR NOT(a_pElement->asType()))AND a_pElement->getOwner() == nullptr)
        addElement(a_pElement);
    else
        addReferencedElement(a_pElement);
}

LanguageElement* TemplateSpecialization::getDefaultArgument(StringView a_strParameterName) const
{
    if (m_pDefaultArguments == nullptr)
        return nullptr;
    size_t index = m_pTemplate->getTemplateParameterIndex(a_strParameterName);
    if (index == ~size_t(0))
        return nullptr;
    return (*m_pDefaultArguments)[index];
}

void TemplateSpecialization::removeArgument(LanguageElement* a_pElement)
{
    if (a_pElement->getOwner() != this)
        removeReferencedElement(a_pElement);
    else
        removeElement(a_pElement);
}

phantom::lang::TemplateParameters const& TemplateSpecialization::getTemplateParameters() const
{
    return m_pTemplateSignature->getTemplateParameters();
}

void TemplateSpecialization::_updateName()
{
}

void TemplateSpecialization::onReferencedElementAdded(LanguageElement*)
{
}

void TemplateSpecialization::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (a_pElement == m_pExtendedSpecialization)
        m_pExtendedSpecialization = nullptr;
    for (auto it = m_Arguments.begin(); it != m_Arguments.end();)
    {
        if (*it == a_pElement)
        {
            it = m_Arguments.erase(it);
        }
        else
            ++it;
    }
}

bool TemplateSpecialization::matches(LanguageElementsView a_Arguments) const
{
    size_t count = a_Arguments.size();
    if (count != m_Arguments.size())
        return false;
    size_t i = 0;
    for (; i < count; ++i)
    {
        if (a_Arguments[i]
            ->asType() AND        a_Arguments[i]
            ->asPlaceholder() AND m_Arguments[i]
            ->asPlaceholder() AND m_Arguments[i]
            ->asType())
            continue;
        if (NOT(a_Arguments[i]->isSame(m_Arguments[i])))
            return false;
    }
    return true;
}

void TemplateSpecialization::checkCompleteness() const
{
    PHANTOM_ASSERT(m_pTemplate);
    PHANTOM_ASSERT(m_pTemplate->getModule() != nullptr, "Template not registered for this template specialization");
}

bool TemplateSpecialization::canBeDestroyed() const
{
    return LanguageElement::canBeDestroyed();
}

size_t TemplateSpecialization::getArgumentIndex(StringView a_strParameterName) const
{
    return m_pTemplate->getTemplateParameterIndex(a_strParameterName);
}

void TemplateSpecialization::onElementRemoved(LanguageElement* a_pElement)
{
    if (a_pElement == m_pTemplated)
    {
        m_pTemplated = nullptr;
    }
    for (auto it = m_Arguments.begin(); it != m_Arguments.end();)
    {
        if (*it == a_pElement)
        {
            it = m_Arguments.erase(it);
        }
        else
            ++it;
    }
    LanguageElement::onElementRemoved(a_pElement);
}

bool TemplateSpecialization::isEmpty() const
{
    for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it)
    {
        if (NOT((*it)->asPlaceholder()))
            return false; // not a placeholder => partial or full specialization
    }
    return true;
}

bool TemplateSpecialization::isPartial() const
{
    return m_pTemplateSignature AND m_pTemplateSignature->getTemplateParameters().size() != 0;
}

bool TemplateSpecialization::isSpecializingParameter(TemplateParameter* a_pParameter) const
{
    size_t i = m_pTemplate->getTemplateParameterIndex(a_pParameter);
    return (i != ~size_t(0)) AND(m_Arguments[i] != nullptr);
}

bool TemplateSpecialization::isVariadic() const
{
    return m_pTemplateSignature->isVariadic();
}

//
// bool TemplateSpecialization::isSpecializing( LanguageElement* a_pLanguageElement )
// {
//     if(a_pLanguageElement->m_pTemplateParameterDependencies == nullptr) return false;
//     for(auto it = a_pLanguageElement->m_pTemplateParameterDependencies->begin(); it !=
//     a_pLanguageElement->m_pTemplateParameterDependencies->end(); ++it)
//     {
//         TemplateParameter* pDependency = *it;
//         if(isSpecializingParameter(pDependency)) return true;
//         for(auto it = a_pLanguageElement->beginElements(); it !=
//         a_pLanguageElement->endElements(); ++it)
//         {
//             if(isSpecializing(*it)) return true;
//         }
//         for(auto it = a_pLanguageElement->beginReferencedElements(); it !=
//         a_pLanguageElement->endReferencedElements(); ++it)
//         {
//             if(isSpecializing(*it)) return true;
//         }
//     }
//     return false;
// }

bool TemplateSpecialization::isSame(TemplateSpecialization* a_pTemplateSpecialization) const
{
    return a_pTemplateSpecialization->getTemplate() ==
    m_pTemplate AND matches(a_pTemplateSpecialization->getArguments());
}

void TemplateSpecialization::setTemplated(Symbol* a_pTemplated)
{
    PHANTOM_ASSERT(a_pTemplated);
    PHANTOM_ASSERT(NOT(isNative()) OR m_pTemplated == nullptr);
    PHANTOM_ASSERT(m_pTemplated == nullptr, "template body has already been defined");
    m_pTemplated = a_pTemplated;
    if (!isFull())
    {
        m_pTemplated->setTemplateDependant();
    }
    addElement(m_pTemplated);
}

TemplateSpecialization* TemplateSpecialization::clone(uint a_Flags) const
{
    TemplateSignature* pSign = getTemplateSignature()->clone(a_Flags);
    LanguageElements   arguments(m_Arguments.size());
    for (size_t i = 0; i < m_Arguments.size(); ++i)
    {
        if (auto pPH = m_Arguments[i]->asPlaceholder())
        {
            TemplateParameter* pParam = pSign->getTemplateParameter(pPH->asSymbol()->getName());
            PHANTOM_ASSERT(pParam);
            arguments[i] = pParam->getPlaceholder()->asSymbol();
        }
        else
        {
            arguments[i] = m_Arguments[i];
        }
    }
    return PHANTOM_NEW(TemplateSpecialization)(m_pTemplate, pSign, arguments, a_Flags);
}

void TemplateSpecialization::setExtendedSpecialization(TemplateSpecialization* a_pExtended)
{
    if (m_pExtendedSpecialization == a_pExtended)
        return;
    if (m_pExtendedSpecialization)
    {
        removeReferencedElement(m_pExtendedSpecialization);
    }
    m_pExtendedSpecialization = a_pExtended;
    if (m_pExtendedSpecialization)
    {
        PHANTOM_ASSERT(matches(m_Arguments),
                       "extended specialization must match exactly the arguments of this specialization");
        addReferencedElement(m_pExtendedSpecialization);
    }
}

Source* TemplateSpecialization::getCodeLocationSource() const
{
    if (m_pInstantiationSpecialization)
        return m_pInstantiationSpecialization->TemplateSpecialization::getCodeLocationSource();
    return Symbol::getCodeLocationSource();
}

bool TemplateSpecialization::isFull() const
{
    return !isPartial() AND !isEmpty();
}

bool TemplateSpecialization::partialAccepts(const LanguageElements& a_Arguments, size_t& a_Score,
                                            PlaceholderMap& a_Deductions) const
{
    size_t score = 0;
    /// add default arguments
    size_t i = a_Arguments.size();
    size_t count = m_pTemplate->getTemplateParameters().size();
    if (m_pTemplate->getDefaultArgumentCount() < (count - i))
    {
        return false;
    }
    for (i = 0; i < a_Arguments.size(); ++i)
    {
        size_t subScore = 0;
        if (NOT(m_Arguments[i]->partialAccepts(a_Arguments[i], subScore, a_Deductions)))
            return false;
        score += subScore;
    }
    a_Score = score;
    return true;
}

TemplateSpecialization* TemplateSpecialization::Create(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                       const LanguageElements& a_Arguments, Symbol* a_pTemplated,
                                                       uint a_Flags)
{
    return PHANTOM_DEFERRED_NEW_EX(TemplateSpecialization)(a_pTemplate, a_pTemplateSignature, a_Arguments, a_pTemplated,
                                                           a_Flags);
}

} // namespace lang
} // namespace phantom
