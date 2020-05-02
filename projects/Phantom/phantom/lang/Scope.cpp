// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Scope.h"

#include "Alias.h"
#include "AnonymousStruct.h"
#include "AnonymousUnion.h"
#include "Application.h"
#include "Constant.h"
#include "Function.h"
#include "Signature.h"
#include "Template.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
#include "Variable.h"

#include <phantom/detail/core_internal.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Variables               m_EmptyVariables;
static Constants               m_EmptyConstants;
static Functions               m_EmptyFunctions;
static Types                   m_EmptyTypes;
static Templates               m_EmptyTemplates;
static TemplateSpecializations m_EmptyTemplateSpecializations;
static Aliases                 m_EmptyAliases;
static AnonymousSections       m_EmptyAnonymousSections;

Scope::Scope(LanguageElement* a_pThisElement, LanguageElement* a_pUnit)
    : m_pThisElement(a_pThisElement),
      m_pUnit(a_pUnit),
      m_Functions(a_pThisElement),
      m_Variables(a_pThisElement),
      m_Constants(a_pThisElement),
      m_Types(a_pThisElement),
      m_Templates(a_pThisElement),
      m_TemplateSpecializations(a_pThisElement),
      m_AnonymousSections(a_pThisElement),
      m_Aliases(a_pThisElement)
{
}

void Scope::initialize()
{
    auto pAlloc = asLanguageElement()->getAllocator();
    m_Functions.setAllocator(pAlloc);
    m_Variables.setAllocator(pAlloc);
    m_Constants.setAllocator(pAlloc);
    m_Types.setAllocator(pAlloc);
    m_Templates.setAllocator(pAlloc);
    m_TemplateSpecializations.setAllocator(pAlloc);
    m_AnonymousSections.setAllocator(pAlloc);
    m_Aliases.setAllocator(pAlloc);
}

void Scope::terminate()
{
	m_Functions.release();
	m_Variables.release();
	m_Constants.release();
	m_Types.release();
	m_Templates.release();
	m_TemplateSpecializations.release();
	m_AnonymousSections.release();
	m_Aliases.release();
}

Type* Scope::getType(StringView a_strName) const
{
    auto it = m_Types->begin();
    auto end = m_Types->end();
    for (; it != end; ++it)
    {
        if ((*it)->getName() == a_strName)
        {
            return *it;
        }
    }
    return nullptr;
}

Template* Scope::getTemplate(StringView a_strName) const
{
    auto it = m_Templates->begin();
    auto end = m_Templates->end();
    for (; it != end; ++it)
    {
        if ((*it)->getName() == a_strName)
        {
            return *it;
        }
    }
    return nullptr;
}

TemplateSpecialization* Scope::getTemplateSpecialization(StringView              a_strName,
                                                         const LanguageElements& a_Arguments) const
{
    for (auto p : getTemplateSpecializations())
    {
        if (p->getTemplate()->getName() == a_strName)
        {
            if (p->getArgumentCount() == a_Arguments.size())
            {
                bool ok = true;
                for (size_t i = 0; i < a_Arguments.size(); ++i)
                {
                    if (p->getArgument(i) != a_Arguments[i])
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                    return p;
            }
        }
    }
    Template* pTemplate = getTemplate(a_strName);
    if (pTemplate)
    {
        return pTemplate->getTemplateSpecialization(a_Arguments);
    }
    return nullptr;
}

void Scope::addType(Type* a_pType)
{
    if (a_pType->asClassType() || a_pType->asEnum())
    {
        m_Types->push_back(a_pType);
    }
    onScopeSymbolAdded(a_pType);
}

void Scope::addFunction(Function* a_pFunction)
{
    PHANTOM_ASSERT(acceptsSubroutine(a_pFunction));
    m_Functions->push_back(a_pFunction);
    onScopeSymbolAdded(a_pFunction);
}

void Scope::addVariable(Variable* a_pVariable)
{
    m_Variables->push_back(a_pVariable);
    onScopeSymbolAdded(a_pVariable);
}

void Scope::addConstant(Constant* a_pConstant)
{
    m_Constants->push_back(a_pConstant);
    onScopeSymbolAdded(a_pConstant);
}

void Scope::addTemplate(Template* a_pTemplate)
{
    m_Templates->push_back(a_pTemplate);
    onScopeSymbolAdded(a_pTemplate);
}

void Scope::addTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization)
{
    PHANTOM_ASSERT(a_pTemplateSpecialization->getTemplate(),
                   "template specialization must be declared in same scope as its specialized template");
    m_TemplateSpecializations->push_back(a_pTemplateSpecialization);
    onScopeSymbolAdded(a_pTemplateSpecialization);
}

void Scope::addAlias(Alias* a_pAlias)
{
    m_Aliases->push_back(a_pAlias);
    onScopeSymbolAdded(a_pAlias);
}

Alias* Scope::addAlias(Symbol* a_pSymbol, StringView a_strAlias, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
{
    PHANTOM_ASSERT(a_pSymbol);
    PHANTOM_ASSERT(m_pUnit, "a pure referencing scope cannot create new objects ; if namespace use the source instead");
    Alias* pAlias = m_pUnit->NewDeferred<Alias>(a_pSymbol, a_strAlias, a_Modifiers, a_uiFlags);
    addAlias(pAlias);
    return pAlias;
}

Template* Scope::addAliasTemplate(TemplateSignature* a_pSignature, StringView a_strAliasName,
                                  StringView a_TemplateDependantType, Modifiers a_Modifiers /*= 0*/,
                                  uint a_uiFlags /*= 0*/)
{
    PHANTOM_ASSERT(m_pUnit, "a pure referencing scope cannot create new objects ; if namespace use the source instead");

    a_pSignature->setOwner(m_pUnit);
    Type* pType = Application::Get()->findCppType(a_TemplateDependantType, a_pSignature);
    a_pSignature->setOwner(nullptr);
    if (pType == nullptr)
        return nullptr;
    Template* pTemplate = m_pUnit->New<Template>(a_pSignature, a_strAliasName, a_Modifiers, a_uiFlags);
    Alias*    pAlias = m_pUnit->New<Alias>(pType, a_strAliasName, PHANTOM_R_NONE, a_uiFlags & PHANTOM_R_FLAG_NATIVE);
    pTemplate->getEmptyTemplateSpecialization()->setTemplated(pAlias);
    PHANTOM_ASSERT(pType->getOwner()); // template dependant
    addTemplate(pTemplate);
    return pTemplate;
}

Template* Scope::addAliasTemplate(StringView a_TemplateTypes, StringView a_TemplateParams, StringView a_strAliasName,
                                  StringView a_TemplateDependantType, Modifiers a_Modifiers /*= 0*/,
                                  uint a_uiFlags /*= 0*/)
{
    PHANTOM_ASSERT(m_pUnit, "a pure referencing scope cannot create new objects ; if namespace use the source instead");
    TemplateSignature* pTS = TemplateSignature::Parse(m_pUnit, a_TemplateTypes, a_TemplateParams, asLanguageElement(),
                                                      a_uiFlags & PHANTOM_R_FLAG_NATIVE);
    if (pTS == nullptr)
        return nullptr;
    Template* pTemplate = addAliasTemplate(pTS, a_strAliasName, a_TemplateDependantType, a_Modifiers, a_uiFlags);
    if (pTemplate == nullptr)
    {
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "remove leak here (not critical because happens only on errors)")
#endif
        return nullptr;
    }
    return pTemplate;
}

Alias* Scope::getAlias(StringView a_strName) const
{
    for (auto it = m_Aliases->begin(); it != m_Aliases->end(); ++it)
    {
        if ((*it)->getName() == a_strName)
            return *it;
    }
    return nullptr;
}

LanguageElement* Scope::getAliased(StringView a_strAlias) const
{
    Alias* pAlias = getAlias(a_strAlias);
    return pAlias ? pAlias->getAliasedSymbol() : nullptr;
}

PrimitiveType* Scope::getPrimitiveType(StringView a_strName) const
{
    Type* pType = getType(a_strName);
    return pType ? pType->asPrimitiveType() : nullptr;
}

Class* Scope::getClass(StringView a_strName) const
{
    Type* pType = getType(a_strName);
    return pType ? pType->asClass() : nullptr;
}

Function* Scope::getFunction(StringView a_strName, TypesView a_Types) const
{
    auto it = m_Functions->begin();
    auto end = m_Functions->end();
    for (; it != end; ++it)
    {
        Function* pFunction = static_cast<Function*>((*it));
        if (pFunction->matches(a_strName, a_Types))
        {
            return pFunction;
        }
    }
    return nullptr;
}

void Scope::findFunctions(Functions& a_Functions, StringView a_Name /*= ""*/,
                          const Types* a_pParameterTypes /*= nullptr*/, Type* a_pReturnType /*= nullptr*/) const
{
    for (auto f : *m_Functions)
    {
        if ((a_Name.empty() || f->getName() == a_Name) &&
            (a_pReturnType == nullptr || f->getReturnType() == a_pReturnType) &&
            (a_pParameterTypes == nullptr || f->getSignature()->matches(*a_pParameterTypes)))
        {
            a_Functions.push_back(f);
        }
    }
}

Variable* Scope::getVariable(StringView a_strName) const
{
    auto it = m_Variables->begin();
    auto end = m_Variables->end();
    for (; it != end; ++it)
    {
        Variable* pVariable = static_cast<Variable*>((*it));
        if (pVariable->getName() == a_strName)
            return pVariable;
    }
    return nullptr;
}

Constant* Scope::getConstant(StringView a_strName) const
{
    auto it = m_Constants->begin();
    auto end = m_Constants->end();
    for (; it != end; ++it)
    {
        Constant* pConstant = static_cast<Constant*>((*it));
        if (pConstant->getName() == a_strName)
            return pConstant;
    }
    return nullptr;
}

void Scope::addSubroutine(Subroutine* a_pSubroutine)
{
    PHANTOM_ASSERT(a_pSubroutine);
    if (a_pSubroutine->asFunction())
        addFunction(a_pSubroutine->asFunction());
    else
        PHANTOM_ASSERT_NO_IMPL();
}

Subroutine* Scope::getSubroutine(StringView a_strName, TypesView a_ParameterTypes, Modifiers a_Modifiers) const
{
    PHANTOM_ASSERT(a_Modifiers == (a_Modifiers & PHANTOM_R_METHOD_QUAL_MASK));
    if (a_Modifiers == 0)
        return getFunction(a_strName, a_ParameterTypes);
    return nullptr;
}

Subroutine* Scope::getSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_ParameterTypes,
                                 Modifiers a_Modifiers /*= 0*/) const
{
    Subroutine* pSubroutine = getSubroutine(a_strName, a_ParameterTypes, a_Modifiers);
    if (pSubroutine && pSubroutine->getReturnType() && pSubroutine->getReturnType()->isSame(a_pReturnType))
        return pSubroutine;
    return nullptr;
}

bool Scope::acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types, Modifiers, uint,
                              Subroutines* a_pOutConflictingSubroutines /*= nullptr*/) const
{
    bool bResult = true;
    auto it = m_Functions->begin();
    auto end = m_Functions->end();
    for (; it != end; ++it)
    {
        Function*          pFunction = *it;
        ESignatureRelation eOR = pFunction->getSignatureRelationWith(
        a_pReturnType, a_strName, a_Types,
        0); // 0 because static functions consider const and non-const as conflicting
        if (eOR != e_SignatureRelation_None)
        {
            if (a_pOutConflictingSubroutines)
            {
                a_pOutConflictingSubroutines->push_back(pFunction);
                bResult = false;
            }
            else
                return false;
        }
    }
    return bResult;
}

bool Scope::acceptsSubroutine(Subroutine* a_pSubroutine, Subroutines* a_pOutConflictingSubroutines) const
{
    return acceptsSubroutine(a_pSubroutine->getReturnType(), a_pSubroutine->getName(),
                             a_pSubroutine->getParameterTypes(), a_pSubroutine->getModifiers(),
                             a_pSubroutine->getFlags(), a_pOutConflictingSubroutines);
}

void Scope::getElementDoubles(Symbol* a_pElement, Symbols& out) const
{
    size_t elementIndex = m_pThisElement->getElementIndex(a_pElement);
    PHANTOM_ASSERT(elementIndex != ~size_t(0));
    Symbol* pThisSymbol = m_pThisElement->asSymbol();
    if (pThisSymbol)
    {
        Symbols thisDoubles;
        pThisSymbol->getDoubles(thisDoubles);
        for (auto it = thisDoubles.begin(); it != thisDoubles.end(); ++it)
        {
            Symbol* pDouble = static_cast<Symbol*>((*it)->getElements()[elementIndex]);
            PHANTOM_ASSERT(pDouble->getQualifiedDecoratedName() == a_pElement->getQualifiedDecoratedName());
            out.push_back(pDouble);
        }
    }
}

Alias* Scope::getTypedef(StringView a_strTypedef) const
{
    Alias* pAlias = getAlias(a_strTypedef);
    return pAlias->getAliasedSymbol()->asType() ? pAlias : nullptr;
}

void Scope::addUsing(Symbol* a_pElement)
{
    PHANTOM_ASSERT(m_pUnit, "a pure referencing scope cannot create new objects ; if namespace use the source instead");
    addAlias(m_pUnit->New<Alias>(a_pElement, a_pElement->getName()));
}

void Scope::fetchTypesCascade(Types& a_Types) const
{
    for (size_t i = 0; i < m_Types->size(); ++i)
    {
        Type* pType = m_Types[i];
        a_Types.push_back(pType);
        if (pType->asClassType())
        {
            static_cast<ClassType*>(pType)->fetchTypesCascade(a_Types);
        }
    }
    for (size_t i = 0; i < m_TemplateSpecializations->size(); ++i)
    {
        TemplateSpecialization* pSpec = m_TemplateSpecializations[i];
        Symbol*                 pBody = pSpec->getTemplated();
        if (pBody && pBody->asClassType())
        {
            a_Types.push_back(static_cast<ClassType*>(pBody));
            static_cast<ClassType*>(pBody)->fetchTypesCascade(a_Types);
        }
    }
}

void Scope::addAnonymousSection(AnonymousSection* a_pAnonymousSection)
{
    m_AnonymousSections->push_back(a_pAnonymousSection);
    onScopeSymbolAdded(a_pAnonymousSection);
}

bool Scope::isSymbolHidden(Symbol* a_pSymbol) const
{
    return m_pThisElement->isSymbolHidden(a_pSymbol);
}

void Scope::getScopedSymbolsWithName(StringView a_Name, Symbols& a_Symbols) const
{
    Template* pTemplate = getTemplate(a_Name);
    if (pTemplate)
    {
        a_Symbols.push_back(pTemplate);
        return;
    }
    asLanguageElement()->getSymbolsWithName(a_Name, a_Symbols);
}

TemplateSpecialization* Scope::addTemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                         const LanguageElements& a_Arguments, Symbol* a_pBody)
{
    TemplateSpecialization* pTemplateSpecialization;

    if (!(m_pThisElement->isNative()) &&
        (pTemplateSpecialization = a_pTemplate->getTemplateSpecialization(a_Arguments)) &&
        pTemplateSpecialization->getModule() == m_pThisElement->getModule())
    {
        PHANTOM_ASSERT(false);
        return nullptr;
    }
    addTemplateSpecialization(
    pTemplateSpecialization = m_pUnit->NewDeferred<TemplateSpecialization>(
    a_pTemplate, a_pTemplateSignature, a_Arguments, a_pBody, a_pTemplate->getFlags() & PHANTOM_R_FLAG_NATIVE));
    pTemplateSpecialization->setFlags(PHANTOM_R_FLAG_NATIVE * m_pThisElement->isNative());
    return pTemplateSpecialization;
}

TemplateSpecialization* Scope::addTemplateSpecialization(Template* a_pTemplate, TemplateSignature* a_pTemplateSignature,
                                                         const LanguageElements& a_Arguments)
{
    TemplateSpecialization* pTemplateSpecialization;
    if (!(m_pThisElement->isNative()) &&
        (pTemplateSpecialization = a_pTemplate->getTemplateSpecialization(a_Arguments)) &&
        pTemplateSpecialization->getModule() == m_pThisElement->getModule())
    {
        PHANTOM_ASSERT(false, "template already instantiated in this module");
        return nullptr;
    }
    addTemplateSpecialization(
    pTemplateSpecialization = m_pUnit->New<TemplateSpecialization>(
    a_pTemplate, a_pTemplateSignature, a_Arguments, (PHANTOM_R_FLAG_NATIVE * m_pThisElement->isNative())));
    return pTemplateSpecialization;
}

TemplateSpecialization*
Scope::addTemplateInstantiation(TemplateSpecialization* a_pInstantiationSpecialization,
                                const LanguageElements& a_Arguments,
                                const PlaceholderMap&   a_PartialSpecializationParameterDeductions)
{
    TemplateSpecialization* pTemplateSpecialization;
    if (!(m_pThisElement->isNative()) &&
        (pTemplateSpecialization =
         a_pInstantiationSpecialization->getTemplate()->getTemplateSpecialization(a_Arguments)) &&
        pTemplateSpecialization->getModule() == m_pThisElement->getModule())
    {
        PHANTOM_ASSERT(false, "template already instantiated in this module");
        return nullptr;
    }
    addTemplateSpecialization(pTemplateSpecialization = m_pUnit->New<TemplateSpecialization>(
                              a_pInstantiationSpecialization, a_Arguments, a_PartialSpecializationParameterDeductions));
    // pTemplateSpecialization->setFlags(PHANTOM_R_FLAG_NATIVE*m_pThisElement->isNative());
    return pTemplateSpecialization;
}

void Scope::findClasses(Classes& a_Classes, Class* a_pBaseClass /*= nullptr*/,
                        bool a_bDefaultInstanciable /*= false*/) const
{
    for (size_t i = 0; i < m_Types->size(); ++i)
    {
        if (Class* pClass = (*m_Types)[i]->asClass())
        {
            if ((a_pBaseClass == nullptr || (pClass->isA(a_pBaseClass) && !(pClass->isSame(a_pBaseClass)))) &&
                (a_bDefaultInstanciable == false || pClass->isDefaultInstanciable()))
                a_Classes.push_back(pClass);
            pClass->findClasses(a_Classes, a_pBaseClass, a_bDefaultInstanciable);
        }
    }

    for (size_t i = 0; i < m_TemplateSpecializations->size(); ++i)
    {
        if (auto pTemplated = (*m_TemplateSpecializations)[i]->getTemplated())
        {
            if (Class* pClass = pTemplated->asClass())
            {
                if ((a_pBaseClass == nullptr || (pClass->isA(a_pBaseClass) && !(pClass->isSame(a_pBaseClass)))) &&
                    (a_bDefaultInstanciable == false || pClass->isDefaultInstanciable()))
                    a_Classes.push_back(pClass);
                pClass->findClasses(a_Classes, a_pBaseClass, a_bDefaultInstanciable);
            }
        }
    }
}

void Scope::removeAlias(Alias* a_pAlias)
{
    onScopeSymbolRemoving(a_pAlias);
    m_Aliases->erase_unsorted(std::next(std::find(m_Aliases->rbegin(), m_Aliases->rend(), a_pAlias)).base());
}

void Scope::removeType(Type* a_pType)
{
    onScopeSymbolRemoving(a_pType);
    m_Types->erase_unsorted(std::next(std::find(m_Types->rbegin(), m_Types->rend(), a_pType)).base());
}

void Scope::removeFunction(Function* a_pFunction)
{
    onScopeSymbolRemoving(a_pFunction);
    m_Functions->erase_unsorted(std::next(std::find(m_Functions->rbegin(), m_Functions->rend(), a_pFunction)).base());
}

void Scope::removeVariable(Variable* a_pVariable)
{
    onScopeSymbolRemoving(a_pVariable);
    m_Variables->erase_unsorted(std::next(std::find(m_Variables->rbegin(), m_Variables->rend(), a_pVariable)).base());
}

void Scope::removeConstant(Constant* a_pConstant)
{
    onScopeSymbolRemoving(a_pConstant);
    m_Constants->erase_unsorted(std::next(std::find(m_Constants->rbegin(), m_Constants->rend(), a_pConstant)).base());
}

void Scope::removeTemplate(Template* a_pTemplate)
{
    onScopeSymbolRemoving(a_pTemplate);
    m_Templates->erase_unsorted(std::next(std::find(m_Templates->rbegin(), m_Templates->rend(), a_pTemplate)).base());
}

void Scope::removeTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization)
{
    onScopeSymbolRemoving(a_pTemplateSpecialization);
    m_TemplateSpecializations->erase_unsorted(
		std::next(std::find(m_TemplateSpecializations->rbegin(), m_TemplateSpecializations->rend(), a_pTemplateSpecialization))
    .base());
}

} // namespace lang
} // namespace phantom
