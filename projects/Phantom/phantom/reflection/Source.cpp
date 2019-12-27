// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Source.h"

#include "Alias.h"
#include "Application.h"
#include "FunctionPointer.h"
#include "FunctionType.h"
#include "Module.h"
#include "Namespace.h"
#include "Package.h"
#include "PackageFolder.h"
#include "SourceFile.h"

#include <phantom/utils/Path.h>
#ifndef __DOXYGEN__
#    include <fstream>
#    include <stdarg.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#    include <time.h>
#endif
#include "FieldPointer.h"
#include "InitializerListType.h"
#include "MethodPointer.h"
#include "phantom/detail/new.h"

#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallSet.h>
/* *********************************************** */
#define PHANTOM_DEBUG_PACKAGE 0

namespace phantom
{
namespace reflection
{
Source::Source(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, PHANTOM_R_ALWAYS_VALID | a_uiFlags), Scope(this)
{
    // PHANTOM_ASSERT(a_strName.empty() OR ((a_uiFlags&PHANTOM_R_FLAG_PRIVATE_VIS) != 0), "source
    // name must contain only letters, numbers or underscore");
}

Source::Source(Package* a_pPackage, StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers,
             (a_uiFlags & PHANTOM_R_FLAG_INVALID) ? a_uiFlags : (PHANTOM_R_ALWAYS_VALID | a_uiFlags)),
      Scope(this)
{
    // PHANTOM_ASSERT(a_strName.empty() OR ((a_uiFlags&PHANTOM_R_FLAG_PRIVATE_VIS) != 0), "source
    // name must contain only letters, numbers or underscore");
    a_pPackage->addSource(this);
}

Source::~Source()
{
    while (m_Importings.size())
    {
        m_Importings.back()->removeImport(this);
    }
    for (auto it = m_Imports.begin(); it != m_Imports.end(); ++it)
    {
        it->source->_removeImporting(this);
    }
    if (m_pSourceStream)
    {
        PHANTOM_DELETE_DYN m_pSourceStream;
    }
}

void Source::terminate()
{
    if (isNative())
    {
        if (m_pFunctionPointers)
            PHANTOM_DELETE(FunctionPointers) m_pFunctionPointers;
        if (m_pFunctionTypes)
            PHANTOM_DELETE(FunctionTypes) m_pFunctionTypes;
        if (m_pInitializerListTypes)
            PHANTOM_DELETE(InitializerListTypes) m_pInitializerListTypes;
    }
    Symbol::terminate();
}

bool Source::canBeUnloaded() const
{
    SmallSet<Module*> referencingModules;
    fetchReferencingModulesDeep(referencingModules);
    return referencingModules.empty()
    OR((referencingModules.size() == 1) AND((*referencingModules.begin()) == getModule()));
}

Package* Source::getPackage() const
{
    return static_cast<Package*>(getOwner());
}

void Source::getQualifiedName(StringBuffer&) const
{
    PHANTOM_ASSERT_NO_IMPL();
}

void Source::addScopeElement(Symbol* a_pSymbol)
{
    if (NOT(testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)) // not an archive
        AND a_pSymbol->getNamespace() == nullptr)
    {
        getPackage()->getCounterpartNamespace()->addScopeElement(a_pSymbol);
    }
    addSymbol(a_pSymbol);
    if (a_pSymbol->isNative())
    {
        if (a_pSymbol->asTemplateSpecialization() == nullptr)
        {
            PHANTOM_ASSERT(a_pSymbol->getNamespace(),
                           "any native element added at source scope must have a namespace (except "
                           "for template specializations)");
            if (a_pSymbol->getNamespace() != getPackage()->getCounterpartNamespace())
            {
#if PHANTOM_DEBUG_PACKAGE
                PHANTOM_WARNING(
                false,
                "element added to source '%s' have namespace name '%s' different from source "
                "package name '%s', this could lead to scope deduction mistakes or conflicts",
                getName().c_str(),
                static_cast<LanguageElement*>(a_pSymbol->getNamespace())->getQualifiedDecoratedName().c_str(),
                getPackage()->getName().c_str());
#endif
            }
        }
    }
}

void Source::removeScopeElement(Symbol* a_pElement)
{
    removeElement(a_pElement);
}

Source* Source::getNativeArchive() const
{
    if (isNative())
        return (Source*)this;
    return m_pNativeArchive;
}

void Source::onElementAdded(LanguageElement* a_pElement)
{
    Scope::scopedElementAdded(a_pElement);
    if (Symbol* pSymbol = a_pElement->asSymbol())
    {
        if (FieldPointer* pDMP = a_pElement->asFieldPointer())
        {
            m_FieldPointers.push_back(pDMP);
        }
        else if (MethodPointer* pMFPT = a_pElement->asMethodPointer())
        {
            m_MethodPointers.push_back(pMFPT);
        }
        else if (FunctionType* pFT = a_pElement->asFunctionType())
        {
            if (m_pFunctionTypes == nullptr)
                m_pFunctionTypes = PHANTOM_NEW(FunctionTypes);
            m_pFunctionTypes->push_back(pFT);
        }
        else if (InitializerListType* pILT = a_pElement->asInitializerListType())
        {
            if (m_pInitializerListTypes == nullptr)
                m_pInitializerListTypes = PHANTOM_NEW(InitializerListTypes);
            m_pInitializerListTypes->push_back(pILT);
        }
        else if (FunctionPointer* pFPT = a_pElement->asFunctionPointer())
        {
            if (m_pFunctionPointers == nullptr)
                m_pFunctionPointers = PHANTOM_NEW(FunctionPointers);
            m_pFunctionPointers->push_back(pFPT);
        }
    }
}

void Source::onElementRemoved(LanguageElement* a_pElement)
{
    Scope::scopedElementRemoved(a_pElement);
    if (Symbol* pSymbol = a_pElement->asSymbol())
    {
        if (pSymbol->getNamespace())
        {
            pSymbol->getNamespace()->removeScopeElement(pSymbol);
        }
    }
    if (a_pElement->asType())
    {
        if (FieldPointer* pDMP = a_pElement->asFieldPointer())
        {
            m_FieldPointers.erase(std::find(m_FieldPointers.begin(), m_FieldPointers.end(), pDMP));
        }
        else if (MethodPointer* pMFPT = a_pElement->asMethodPointer())
        {
            m_MethodPointers.erase(std::find(m_MethodPointers.begin(), m_MethodPointers.end(), pMFPT));
        }
        else if (FunctionType* pFT = a_pElement->asFunctionType())
        {
            PHANTOM_ASSERT(m_pFunctionTypes);
            m_pFunctionTypes->erase(std::find(m_pFunctionTypes->begin(), m_pFunctionTypes->end(), pFT));
            if (m_pFunctionTypes->empty())
            {
                PHANTOM_DELETE(FunctionTypes) m_pFunctionTypes;
                m_pFunctionTypes = nullptr;
            }
        }
        else if (InitializerListType* pILT = a_pElement->asInitializerListType())
        {
            PHANTOM_ASSERT(m_pInitializerListTypes);
            m_pInitializerListTypes->erase(
            std::find(m_pInitializerListTypes->begin(), m_pInitializerListTypes->end(), pILT));
            if (m_pInitializerListTypes->empty())
            {
                PHANTOM_DELETE(InitializerListTypes) m_pInitializerListTypes;
                m_pInitializerListTypes = nullptr;
            }
        }
        else if (FunctionPointer* pFP = a_pElement->asFunctionPointer())
        {
            PHANTOM_ASSERT(m_pFunctionPointers);
            m_pFunctionPointers->erase(std::find(m_pFunctionPointers->begin(), m_pFunctionPointers->end(), pFP));
            if (m_pFunctionPointers->empty())
            {
                PHANTOM_DELETE(FunctionPointers) m_pFunctionPointers;
                m_pFunctionPointers = nullptr;
            }
        }
    }
}

void Source::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Symbol::onReferencedElementRemoved(a_pElement);
}

hash64 Source::computeHash() const
{
    StringBuffer buffer;
    getUniqueName(buffer);
    return ComputeHash(buffer.c_str(), buffer.size());
}

void Source::getUniqueName(StringBuffer& a_Buf) const
{
    getPackage()->getUniqueName(a_Buf);
    a_Buf += '.';
    String suffix = m_strName.substr(0, m_strName.find_first_of('('));
    a_Buf += suffix;
}

Source* Source::getCodeLocationSource() const
{
    return const_cast<Source*>(this);
}

void Source_fetchImportedSymbols(const Symbol* a_pSymbol, Symbols& a_Symbols, SmallSet<const Symbol*>& treated)
{
    if (!treated.insert(a_pSymbol).second)
        return;
    if (Alias* pAlias = a_pSymbol->asAlias())
        a_pSymbol = pAlias->getAliasedSymbol();
    for (auto p : a_pSymbol->getElements())
    {
        Symbol* pSymbol = p->asSymbol();
        if (pSymbol AND NOT(pSymbol->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS)))
        {
            if (pSymbol->getName().empty())
            {
                Source_fetchImportedSymbols(pSymbol, a_Symbols, treated);
            }
            else
            {
                a_Symbols.push_back(pSymbol);
            }
        }
    }
}
void Source_fetchImportedSymbols(const Symbol* a_pSymbol, Symbols& a_Symbols)
{
    SmallSet<const Symbol*> treated;
    Source_fetchImportedSymbols(a_pSymbol, a_Symbols, treated);
}

MethodPointer* Source::methodPointerType(ClassType* a_pObjectType, Type* a_pReturnType, TypesView a_ParameterTypes,
                                         Modifiers a_RefQualifiers, uint a_uiFlags)
{
    for (auto pMFMP : m_MethodPointers)
    {
        if (pMFMP->getObjectType()
            ->isSame(a_pObjectType) AND pMFMP->getFunctionType()
            ->getReturnType()
            ->isSame(a_pReturnType) AND pMFMP->getFunctionType()
            ->matches(a_ParameterTypes, a_RefQualifiers, a_uiFlags))
        {
            return pMFMP;
        }
    }

    MethodPointer* pPointer = PHANTOM_DEFERRED_NEW_EX(MethodPointer)(
    a_pObjectType, functionType(a_pReturnType, a_ParameterTypes, a_RefQualifiers), a_RefQualifiers, a_uiFlags);
    addSymbol(pPointer);
    return pPointer;
}

FieldPointer* Source::fieldPointerType(ClassType* a_pObjectType, Type* a_pValueType, Modifiers a_Modifiers,
                                       uint a_uiFlags)
{
    for (auto pDMP : m_FieldPointers)
    {
        if (pDMP->getObjectType()->isSame(a_pObjectType) AND pDMP->getValueType()->isSame(a_pValueType))
        {
            return pDMP;
        }
    }
    FieldPointer* pPointer = PHANTOM_DEFERRED_NEW_EX(FieldPointer)(a_pObjectType, a_pValueType, a_Modifiers, a_uiFlags);
    m_FieldPointers.push_back(pPointer);
    addSymbol(pPointer);
    return pPointer;
}

void Source::setSourceStream(SourceStream* a_pStream)
{
    if (m_pSourceStream == a_pStream)
        return;
    PHANTOM_ASSERT(a_pStream == nullptr OR !a_pStream->m_pSource);
    if (m_pSourceStream)
        m_pSourceStream->m_pSource = nullptr;
    m_pSourceStream = a_pStream;
    if (m_pSourceStream)
        m_pSourceStream->m_pSource = this;
    PHANTOM_EMIT sourceStreamChanged(a_pStream);
}

FunctionType* Source::functionType(Type* a_pReturnType, TypesView a_ParameterTypes, Modifiers a_Modifiers, uint a_Flags)
{
    if (m_pFunctionTypes)
    {
        for (auto it = m_pFunctionTypes->begin(); it != m_pFunctionTypes->end(); ++it)
        {
            if ((*it)->getReturnType()->isSame(a_pReturnType) AND(*it)->matches(a_ParameterTypes, a_Modifiers))
            {
                return *it;
            }
        }
    }

    if (m_pFunctionTypes == nullptr)
    {
        m_pFunctionTypes = PHANTOM_NEW(FunctionTypes);
    }
    FunctionType* pType =
    FunctionType::Create(a_pReturnType, a_ParameterTypes, a_Modifiers, a_Flags | (isNative() * PHANTOM_R_FLAG_NATIVE));
    addSymbol(pType);
    return pType;
}

FunctionPointer* Source::functionPointerType(Type* a_pReturnType, ABI a_eABI, TypesView a_ParameterTypes,
                                             Modifiers a_Qualifiers, uint a_Flags)
{
    return functionPointerType(
    functionType(a_pReturnType, a_ParameterTypes, PHANTOM_R_NONE, a_Flags & PHANTOM_R_FLAG_NATIVE), a_eABI,
    a_Qualifiers, a_Flags);
}

FunctionPointer* Source::functionPointerType(FunctionType* a_pFunctionType, ABI a_eABI, Modifiers a_Qualifiers,
                                             uint a_Flags)
{
    if (m_pFunctionPointers)
    {
        for (auto it = m_pFunctionPointers->begin(); it != m_pFunctionPointers->end(); ++it)
        {
            if ((*it)->getABI() == a_eABI AND(*it)->getFunctionType()->isSame(a_pFunctionType))
            {
                return *it;
            }
        }
    }

    if (m_pFunctionPointers == nullptr)
    {
        m_pFunctionPointers = PHANTOM_NEW(FunctionPointers);
    }
    FunctionPointer* pPointer =
    FunctionPointer::Create(a_pFunctionType, a_eABI, a_Qualifiers, a_Flags | (isNative() * PHANTOM_R_FLAG_NATIVE));
    addSymbol(pPointer);
    return pPointer;
}

InitializerListType* Source::initializerListType(TypesView a_Types)
{
    if (m_pInitializerListTypes)
    {
        for (auto it = m_pInitializerListTypes->begin(); it != m_pInitializerListTypes->end(); ++it)
        {
            if ((*it)->matches(a_Types))
            {
                return *it;
            }
        }
    }
    if (m_pInitializerListTypes == nullptr)
    {
        m_pInitializerListTypes = PHANTOM_NEW(InitializerListTypes);
    }
    InitializerListType* pIT = PHANTOM_NEW(InitializerListType)(a_Types);
    if (isNative())
        pIT->setFlag(PHANTOM_R_FLAG_NATIVE);
    m_pInitializerListTypes->push_back(pIT);
    addSymbol(pIT);
    return pIT;
}

bool Source::addImport(Source* a_pSource, bool a_bStatic, bool a_bPublic)
{
    PHANTOM_ASSERT(a_pSource != this);
    if (NOT(canImport(a_pSource)))
    {
        return false;
    }
    PHANTOM_ASSERT(a_pSource);
    Import i;
    i.source = a_pSource;
    i.isPublic = a_bPublic;
    i.isStatic = a_bStatic;
    Alias* pAlias = nullptr;
    if (a_bStatic)
    {
        PackageFolders folders;
        a_pSource->getPackage()->getPackageFolder()->getPackageFolders(folders);
        for (auto it = folders.begin(); it != folders.end(); ++it)
        {
            if (it == folders.begin())
            {
                pAlias = getAlias((*it)->getName());
                if (pAlias == nullptr)
                {
                    pAlias = PHANTOM_NEW(Alias)((*it)->getName(), (a_bStatic ? PHANTOM_R_STATIC : PHANTOM_R_NONE),
                                                (a_bPublic ? PHANTOM_R_FLAG_PUBLIC_VIS : PHANTOM_R_FLAG_PROTECTED_VIS) |
                                                PHANTOM_R_FLAG_IMPLICIT);
                    addAlias(pAlias);
                }
            }
            else
            {
                auto pSubAlias = pAlias->getAlias((*it)->getName());
                if (pSubAlias == nullptr)
                {
                    pSubAlias = PHANTOM_NEW(Alias)((*it)->getName(), 0, PHANTOM_R_FLAG_IMPLICIT);
                    pAlias->addAlias(pSubAlias);
                }
                pAlias = pSubAlias;
            }
        }
        auto pLastNamedAlias = PHANTOM_NEW(Alias)(a_pSource, a_pSource->getName(), 0, PHANTOM_R_FLAG_IMPLICIT);
        if (pAlias)
            pAlias->addAlias(pLastNamedAlias);
        else
            addAlias(pLastNamedAlias);
    }
    else
    {
        addAlias(pAlias = PHANTOM_NEW(Alias)(a_pSource, "", (a_bStatic ? PHANTOM_R_STATIC : PHANTOM_R_NONE),
                                             (a_bPublic ? PHANTOM_R_FLAG_PUBLIC_VIS : PHANTOM_R_FLAG_PROTECTED_VIS) |
                                             PHANTOM_R_FLAG_IMPLICIT));
    }
    i.alias = pAlias;
    m_Imports.push_back(i);
    a_pSource->_addImporting(this);
    return true;
}

void Source::removeImport(Source* a_pSource)
{
    for (auto it = m_Imports.begin(); it != m_Imports.end(); ++it)
    {
        if (it->source == a_pSource)
        {
            PHANTOM_DELETE(Alias) it->alias;
            it->source->_removeImporting(this);
            m_Imports.erase(it);
            return;
        }
    }
    PHANTOM_UNREACHABLE();
}

bool Source::addImport(StringView a_strName, bool a_bStatic, bool a_bPublic)
{
    Source* pSource = Application::Get()->getSource(a_strName);
    if (pSource == nullptr OR pSource == this)
    {
        PHANTOM_LOG(Error, "cannot import source '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
        return false;
    }
    return addImport(pSource, a_bStatic, a_bPublic);
}

bool Source::canImport(Source* a_pSource, Access, Modifiers a_Modifiers /*= 0*/, uint /*= 0*/,
                       SmallMap<Symbol*, Symbols>* a_pCollidingSymbols /*= nullptr*/) const
{
    if (a_pSource == this)
        return false;
    if (a_Modifiers & PHANTOM_R_STATIC)
        return true;
    Symbols symbols0;
    Symbols symbols1;
    Source_fetchImportedSymbols(this, symbols0);
    Source_fetchImportedSymbols(a_pSource, symbols1);
    bool result = true;
    for (auto it0 = symbols0.begin(); it0 != symbols0.end(); ++it0)
    {
        Symbol* pSymbol0 = *it0;
        if (pSymbol0->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS))
            continue;
        for (auto it1 = symbols1.begin(); it1 != symbols1.end(); ++it1)
        {
            Symbol* pSymbol1 = *it1;
            if (pSymbol1->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS))
                continue;
            if (pSymbol0 != pSymbol1)
            {
                if (pSymbol1->getName() == pSymbol0->getName())
                {
                    if (pSymbol1->getDecoratedName() == pSymbol0->getDecoratedName())
                    {
                        if (a_pCollidingSymbols)
                        {
                            result = false;
                            (*a_pCollidingSymbols)[pSymbol0].push_back(pSymbol1);
                        }
                        else
                            return false;
                    }
                }
            }
        }
    }
    return result;
}

bool Source::hasImport(Source* a_pSource) const
{
    for (auto it = m_Imports.begin(); it != m_Imports.end(); ++it)
    {
        if (it->source == a_pSource)
            return true;
    }
    return false;
}

static void Source_getImported(const Source* _this, Sources& a_Imports, SmallSet<const Source*>& treated)
{
    if (treated.insert(_this).second)
    {
        for (Source::Import const& import : _this->getImports())
        {
            a_Imports.push_back(import.source);
            if (import.isPublic)
            {
                Source_getImported(import.source, a_Imports, treated);
            }
        }
    }
}

void Source::getImported(Sources& a_Imports) const
{
    SmallSet<const Source*> treated;
    Source_getImported(this, a_Imports, treated);
}

bool Source::_hasImported(Source* a_pSource, SmallSet<const Source*>& treated) const
{
    if (treated.insert(this).second)
    {
        for (Source::Import const& import : m_Imports)
        {
            if (import.source == a_pSource)
                return true;
            if (import.isPublic AND import.source->_hasImported(a_pSource, treated))
                return true;
        }
    }
    return false;
}

bool Source::hasImported(Source* a_pSource) const
{
    SmallSet<const Source*> treated;
    return _hasImported(a_pSource, treated);
}

void Source::_addImporting(Source* _source)
{
    m_Importings.push_back(_source);
}

void Source::_removeImporting(Source* _source)
{
    auto found = std::find(m_Importings.begin(), m_Importings.end(), _source);
    PHANTOM_ASSERT(found != m_Importings.end());
    m_Importings.erase(found);
}

void Source::clearImports()
{
    while (m_Imports.size())
    {
        removeImport(m_Imports.back().source);
    }
}

void Source::clearDependencies()
{
    while (m_Dependencies.size())
    {
        removeDependency(m_Dependencies.back());
    }
}

bool Source::hasDependencyCascade(Source* a_pSource) const
{
    SmallSet<Source*> treated;
    return _hasDependencyCascade(a_pSource, treated);
}

bool Source::_hasDependencyCascade(Source* a_pSource, SmallSet<Source*>& treated) const
{
    if (hasDependency(a_pSource))
        return true;
    for (auto pDep : m_Dependencies)
    {
        if (treated.insert(pDep).second AND pDep->_hasDependencyCascade(a_pSource, treated))
            return true;
    }
    return false;
}

void Source::addDependency(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource != this);
    PHANTOM_ASSERT(NOT(hasDependency(a_pSource)));
    PHANTOM_ASSERT(NOT(a_pSource->hasDependencyCascade(this)), "illegal recursive source dependency");
    m_Dependencies.push_back(a_pSource);
    a_pSource->m_Dependings.push_back(this);
}

bool Source::addDependency(StringView a_strName)
{
    Source* pSource = Application::Get()->getSource(a_strName);
    PHANTOM_ASSERT(pSource != this);
    if (pSource == nullptr)
    {
        PHANTOM_LOG(Error, "cannot find source '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
        return false;
    }
    addDependency(pSource);
    return true;
}

bool Source::hasDependency(Source* a_pSource) const
{
    return std::find(m_Dependencies.begin(), m_Dependencies.end(), a_pSource) != m_Dependencies.end();
}

void Source::removeDependency(Source* a_pSource)
{
    PHANTOM_ASSERT(hasDependency(a_pSource));
    a_pSource->m_Dependings.erase(std::find(a_pSource->m_Dependings.begin(), a_pSource->m_Dependings.end(), this));
    m_Dependencies.erase(std::find(m_Dependencies.begin(), m_Dependencies.end(), a_pSource));
}

void Source::_addDepending(Source* _source)
{
    m_Dependings.push_back(_source);
}

void Source::_removeDepending(Source* _source)
{
    auto found = std::find(m_Dependings.begin(), m_Dependings.end(), _source);
    PHANTOM_ASSERT(found != m_Dependings.end());
    m_Dependings.erase(found);
}

} // namespace reflection
} // namespace phantom
