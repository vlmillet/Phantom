// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "TemplateSpecialization.h"

#include "Placeholder.h"
#include "Source.h"
#include "Template.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"

#include <phantom/utils/SmallSet.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
/// Full specialization constructor
TemplateSpecialization::TemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pSignature,
                                               const LanguageElements& a_Arguments, Symbol* a_pTemplated, uint a_Flags)
    : Symbol(a_pTemplate->getName(), a_pTemplate->getModifiers(), a_Flags),
      m_pTemplate(a_pTemplate),
      m_pTemplateSignature(a_pSignature),
      m_pTemplated(a_pTemplated)
{
    // accepts only if : (template has no source) || (template is native and we are currently
    // registering native types ) || source is not private (not an obsolete archive stored for
    // revert while run-time building)
    PHANTOM_ASSERT(a_pTemplate->getSource() == nullptr || (a_pTemplate->isNative()) ||
                   !(a_pTemplate->getSource()->getVisibility() == Visibility::Private));
    m_Arguments.resize(a_Arguments.size());
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        setArgument(i, a_Arguments[i]);
    }
    PHANTOM_ASSERT(m_pTemplated);
    m_pTemplated->addFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM);
    PHANTOM_ASSERT(m_pTemplateSignature);
    PHANTOM_ASSERT(m_pTemplate);
    if (!isFull())
    {
        m_pTemplated->setTemplateDependant();
    }
}

/// Partial specialization constructor
TemplateSpecialization::TemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pSignature,
                                               const LanguageElements& a_Arguments, uint a_Flags)
    : Symbol(a_pTemplate->getName(), a_pTemplate->getModifiers(), a_Flags),
      m_pTemplate(a_pTemplate),
      m_pTemplateSignature(a_pSignature)
{
    // accepts only if : (template has no source) || (template is native and we are currently
    // registering native types ) || source is not private (not an obsolete archive stored for
    // revert while run-time building)
    PHANTOM_ASSERT(a_pTemplate->getSource() == nullptr || (a_pTemplate->isNative()) ||
                   !(a_pTemplate->getSource()->getVisibility() == Visibility::Private));
    m_Arguments.resize(a_Arguments.size());
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        setArgument(i, a_Arguments[i]);
    }
    PHANTOM_ASSERT(m_pTemplateSignature);
    PHANTOM_ASSERT(m_pTemplate);
}

TemplateSpecialization::TemplateSpecialization(TemplateSpecialization*     a_pInstantiationSpecialization,
                                               const LanguageElements&     a_Arguments,
                                               const TemplateSubstitution& a_ArgumentSubstitution)
    : Symbol(a_pInstantiationSpecialization->getTemplate()->getName(), 0,
             PHANTOM_R_FLAG_IMPLICIT) // instantiations are considered implicit
      ,
      m_pTemplate(a_pInstantiationSpecialization->getTemplate()),
      m_pInstantiationSpecialization(a_pInstantiationSpecialization),
      m_ArgumentSubstitution(a_ArgumentSubstitution)
{
#if PHANTOM_DEBUG_LEVEL
    // ensure placeholders belongs to the instantiation specialization and are not used multiple
    // times
    {
        SmallSet<Placeholder*> encountered;
        auto                   substCount = a_ArgumentSubstitution.size();
        for (size_t i = 0; i < substCount; ++i)
        {
            bool   found = false;
            auto&  params = a_pInstantiationSpecialization->getTemplateParameters();
            size_t j = 0;
            for (; j < params.size(); ++j)
            {
                if (params[j]->getPlaceholder()->asSymbol()->isSame(
                    a_ArgumentSubstitution.getPlaceholder(i)->asSymbol()))
                {
                    found = true;
                    break;
                }
            }
            PHANTOM_ASSERT(found);
            PHANTOM_ASSERT(params[j]->isPack() ||
                           encountered.insert(a_ArgumentSubstitution.getPlaceholder(i)).second == true,
                           "duplicate of placeholder");
        }
    }
#endif
    m_Arguments.resize(a_Arguments.size());
    for (size_t i = 0; i < a_Arguments.size(); ++i)
    {
        setArgument(i, a_Arguments[i]);
    }
}

void TemplateSpecialization::initialize()
{
    Symbol::initialize();
    m_ArgumentSubstitution.setInstantiation(this);
    if (m_pTemplateSignature)
        m_pTemplateSignature->setOwner(this);
    m_pTemplate->addTemplateSpecialization(this);
    if (m_pTemplated)
    {
        m_pTemplated->setOwner(this);
    }
    for (auto pArg : m_Arguments)
        addReferencedElement(pArg);
    addReferencedElement(m_pTemplate);
    if (m_pInstantiationSpecialization)
        addReferencedElement(m_pInstantiationSpecialization);
}

void TemplateSpecialization::terminate()
{
    if (isNative())
    {
        if (m_pDefaultArguments)
            delete_<LanguageElements>(m_pDefaultArguments);
    }
    Symbol::terminate();
}

hash64 TemplateSpecialization::getDecorationHash() const
{
    hash64 h = '<'; // just to differentiate '<>' from ''
    for (size_t i = 0; i < m_Arguments.size(); ++i)
    {
        if (Placeholder* ph = m_Arguments[i]->asPlaceholder())
        {
            CombineHash(h, ph->getRelativeHash(const_cast<TemplateSpecialization*>(this)));
        }
        else
        {
            CombineHash(h, m_Arguments[i]->asSymbol()->getHash());
        }
    }
    return h;
}

void TemplateSpecialization::getRelativeDecoration(LanguageElement* a_pTo, StringBuffer& a_Buf) const
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

void TemplateSpecialization::getDecoration(StringBuffer& a_Buf) const
{
    a_Buf += '<';
    for (size_t i = 0; i < m_Arguments.size(); ++i)
    {
        if (i)
            a_Buf += ',';
        m_Arguments[i]->getDecoratedName(a_Buf);
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

void TemplateSpecialization::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    getTemplate()->getRelativeName(a_pTo, a_Buf);
    getRelativeDecoration(a_pTo, a_Buf);
}

void TemplateSpecialization::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    getTemplate()->getRelativeName(a_pTo, a_Buf);
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

LanguageElement* TemplateSpecialization::getNamingScope() const
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
    PHANTOM_ASSERT(a_pElement,
                   "invalid template argument ; if argument is a type, check that your type has "
                   "lang declared before its use in any template signature");
    PHANTOM_ASSERT(index != ~size_t(0));
    PHANTOM_ASSERT(m_Arguments[index] == nullptr);
    m_Arguments[index] = a_pElement;
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
    PHANTOM_ASSERT(a_pElement);
    PHANTOM_ASSERT(getDefaultArgument(index) == nullptr, "default argument already defined");
    if (m_pDefaultArguments == nullptr)
    {
        m_pDefaultArguments = new_<LanguageElements>(getAllocator());
        m_pDefaultArguments->resize(m_pTemplate->getTemplateParameters().size(), nullptr);
    }
    (*m_pDefaultArguments)[index] = a_pElement;
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

phantom::lang::TemplateParameters const& TemplateSpecialization::getTemplateParameters() const
{
    return m_pTemplateSignature->getTemplateParameters();
}

void TemplateSpecialization::_updateName() {}

#if 0
template<class T0, class T1, class T2>
struct Tpl
{
};
template<class T0, class T1, class T2>
struct Tpl<T0, T1, T2> // ERROR : == template decl
{
};
template<class T0, class T1, class T2>
struct Tpl<T1, T2, T0> // OK : same names but order changed
{
};
template<class TA, class TB, class TC>
struct Tpl<TB, TC, TA> // ERROR : not same names but order haven't changed
{
};
#endif

bool TemplateSpecialization::matches(LanguageElementsView a_Arguments) const
{
    size_t count = a_Arguments.size();
    if (count != m_Arguments.size())
        return false;
    for (size_t i = 0; i < count; ++i)
    {
        Type* pThisType = m_Arguments[i]->asType();
        Type* pOtherType{};
        if (pThisType && pThisType->asPlaceholder() && (pOtherType = a_Arguments[i]->asType()) &&
            pOtherType->asPlaceholder())
        {
            PHANTOM_ASSERT(pThisType->getOwner() && pThisType->getOwner()->asTemplateParameter());
            PHANTOM_ASSERT(pOtherType->getOwner() && pOtherType->getOwner()->asTemplateParameter());
            TemplateParameter* pThisParam = static_cast<TemplateParameter*>(pThisType->getOwner());
            TemplateParameter* pOtherParam = static_cast<TemplateParameter*>(pOtherType->getOwner());
            if (pThisParam->getIndex() != pOtherParam->getIndex())
                return false;
        }
        else if (!(a_Arguments[i]->isSame(m_Arguments[i])))
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

bool TemplateSpecialization::isEmpty() const
{
    for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it)
    {
        if (!((*it)->asPlaceholder()))
            return false; // not a placeholder => partial or full specialization
    }
    return !getTemplate()->getTemplateSignature()->isVariadic() ||
    m_Arguments.back()->asPlaceholder()->asSymbol()->isSame(
    getTemplate()->getTemplateParameters().back()->getPlaceholder()->asSymbol());
}

bool TemplateSpecialization::isPartial() const
{
    return m_pTemplateSignature && m_pTemplateSignature->getTemplateParameters().size() != 0;
}

bool TemplateSpecialization::isSpecializingParameter(TemplateParameter* a_pParameter) const
{
    size_t i = m_pTemplate->getTemplateParameterIndex(a_pParameter);
    return (i != ~size_t(0)) && (m_Arguments[i] != nullptr);
}

bool TemplateSpecialization::isVariadic() const
{
    return m_pTemplateSignature->isVariadic();
}

bool TemplateSpecialization::isSame(TemplateSpecialization* a_pTemplateSpecialization) const
{
    return a_pTemplateSpecialization->getTemplate() == m_pTemplate &&
    matches(a_pTemplateSpecialization->getArguments());
}

void TemplateSpecialization::setTemplated(Symbol* a_pTemplated)
{
    if (m_pTemplated == a_pTemplated)
        return; // TODO : remove this, this is done for template functions which need to be setTemplated before
                // "Semantic::instantiateTemplate" setTemplated call
    PHANTOM_ASSERT(a_pTemplated);
    PHANTOM_ASSERT(!(isNative()) || m_pTemplated == nullptr);
    PHANTOM_ASSERT(m_pTemplated == nullptr, "template body has already been defined");
    m_pTemplated = a_pTemplated;
    m_pTemplated->addFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM);
    if (!isFull())
    {
        m_pTemplated->setTemplateDependant();
    }
    m_pTemplated->setOwner(this);
}

TemplateSpecialization* TemplateSpecialization::clone(LanguageElement* a_pOwner, uint a_Flags) const
{
    TemplateSignature* pSign = getTemplateSignature()->clone(a_pOwner, a_Flags);
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
    return a_pOwner->New<TemplateSpecialization>(m_pTemplate, pSign, arguments, a_Flags);
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
    return !isPartial() && !isEmpty();
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
        if (!(m_Arguments[i]->partialAccepts(a_Arguments[i], subScore, a_Deductions)))
            return false;
        score += subScore;
    }
    a_Score = score;
    return true;
}

} // namespace lang
} // namespace phantom
