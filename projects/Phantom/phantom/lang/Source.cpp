// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
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
#include "Method.h"
#include "MethodPointer.h"

#include <phantom/detail/core_internal.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallSet.h>
/* *********************************************** */
#define PHANTOM_DEBUG_PACKAGE 0

#if PHANTOM_CONSISTENCY_CHECK_ENABLED
#    define PHANTOM_CONSISTENCY_CHECK_ASSERT PHANTOM_ASSERT
#    define PHANTOM_CONSISTENCY_CHECK_CODE(...) __VA_ARGS__
#else
#    define PHANTOM_CONSISTENCY_CHECK_ASSERT(...)
#    define PHANTOM_CONSISTENCY_CHECK_CODE(...)
#endif

namespace phantom
{
namespace lang
{
Source::Source(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, PHANTOM_R_ALWAYS_VALID | a_uiFlags), Scope(this, this), m_pAlloc(&m_RTAllocator)
{
}

void Source::initialize()
{
    if (isNative()) // native sources won't have any block content, so way less memory consumption compared to full
                    // languages, so allocations will better fit at a module level in term of space
        m_pAlloc = &getModule()->m_Allocator;

    m_CustomAlloc.allocFunc = CustomAllocator::AllocFunc(this, &Source::_alloc);
    m_CustomAlloc.reallocFunc = CustomAllocator::ReallocFunc(this, &Source::_relloc);
    m_CustomAlloc.deallocFunc = CustomAllocator::DeallocFunc(this, &Source::_dealloc);

    Symbol::initialize();
    Scope::initialize();
}

void Source::terminate()
{
    size_t i = m_CreatedElements.size();
    // -- first invoke terminate to cleanup inter-dependencies
    while (i--)
    {
        PHANTOM_CONSISTENCY_CHECK_CODE(size_t sb = m_CreatedElements.size(););
        m_CreatedElements[i]->rtti.metaClass->unregisterInstance(m_CreatedElements[i]->rtti.instance);
        m_CreatedElements[i]->_terminate();
        PHANTOM_CONSISTENCY_CHECK_ASSERT(sb == m_CreatedElements.size());
    }
    Symbol::terminate();
    Scope::terminate();
}

void* Source::PlacementInit(Class* a_pClass, void* a_pInstance)
{
    PHANTOM_ASSERT(a_pClass->isA(PHANTOM_CLASSOF(LanguageElement)));
    _NewH(a_pClass->cast<LanguageElement>(a_pInstance), a_pClass, a_pInstance);
    a_pClass->getMethodCascade("initialize", TypesView{})->invoke<void>(a_pInstance);
    return a_pInstance;
}

Source::~Source()
{
    // -- then invoke destructor to finalize destruction
    size_t i = m_CreatedElements.size();
    while (i--)
    {
        PHANTOM_CONSISTENCY_CHECK_CODE(size_t sb = m_CreatedElements.size());
        m_CreatedElements[i]->~LanguageElement();
        PHANTOM_CONSISTENCY_CHECK_ASSERT(sb == m_CreatedElements.size());
    }
    // ensure we deallocate Smallvector content before destroying allocator
    // TODO : reorder members in Source so that it can't happen
    m_CreatedElements = decltype(m_CreatedElements){};
    m_Elements = decltype(m_Elements){};
    // -- deallocation will be made by destructing allocator
}

void Source::Delete(LanguageElement* a_pElem)
{
    PHANTOM_ASSERT(a_pElem->m_pSource == this);
    PHANTOM_CONSISTENCY_CHECK_ASSERT(
    (m_uiFlags & (PHANTOM_R_INTERNAL_FLAG_TERMINATING | PHANTOM_R_INTERNAL_FLAG_TERMINATED)) == 0);
    a_pElem->rtti.metaClass->unregisterInstance(a_pElem->rtti.instance);
    a_pElem->_terminate();
    a_pElem->~LanguageElement();
    // memset(a_pElem->rtti.instance, 0xDC, a_pElem->rtti.metaClass->getSize());
}

void Source::_NewH(LanguageElement* a_pElem, Class* a_pClass, void* a_pMD)
{
    PHANTOM_CONSISTENCY_CHECK_ASSERT(std::find(m_CreatedElements.begin(), m_CreatedElements.end(), a_pElem) ==
                                     m_CreatedElements.end());

    m_CreatedElements.push_back(a_pElem);
    a_pElem->m_pSource = this;
    a_pElem->rtti.instance = a_pMD;
    a_pElem->rtti.metaClass = a_pClass;
    a_pElem->rtti.metaClass->registerInstance(a_pMD);
}

void Source::_NewDeferredH(LanguageElement* a_pElem, Class* a_pClass, void* a_pMD, StringView a_QN)
{
    if (dynamic_initializer_()->installed())
        return _NewH(a_pElem, a_pClass, a_pMD);

    PHANTOM_CONSISTENCY_CHECK_ASSERT(std::find(m_CreatedElements.begin(), m_CreatedElements.end(), a_pElem) ==
                                     m_CreatedElements.end()); // ASSERT_DEBUG
    m_CreatedElements.push_back(a_pElem);
    a_pElem->m_pSource = this;
    a_pElem->rtti.instance = a_pMD;
    phantom::detail::deferInstallation(a_QN, &a_pElem->rtti);
}

bool Source::canBeUnloaded() const
{
    return true;
    //     SmallSet<Module*> referencingModules;
    //     fetchReferencingModulesDeep(referencingModules);
    //     return referencingModules.empty() ||
    //     ((referencingModules.size() == 1) && ((*referencingModules.begin()) == getModule()));
}

Package* Source::getPackage() const
{
    return static_cast<Package*>(getOwner());
}

void Source::getQualifiedName(StringBuffer&) const
{
    PHANTOM_ASSERT_NO_IMPL();
}

void Source::onScopeSymbolAdded(Symbol* a_pSymbol)
{
    a_pSymbol->setOwner(this);
    a_pSymbol->setVisibility(Visibility::Public);
    if (a_pSymbol->getNamespace() == nullptr)
    {
        a_pSymbol->setNamespace(getPackage()->getCounterpartNamespace());
    }
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

void Source::onScopeSymbolRemoving(Symbol* a_pSymbol)
{
    a_pSymbol->setVisibility(Visibility::Private);
    a_pSymbol->setOwner(nullptr);
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

MethodPointer* Source::methodPointerType(ClassType* a_pObjectType, Type* a_pReturnType, TypesView a_ParameterTypes,
                                         Modifiers a_RefQualifiers, uint a_uiFlags)
{
    for (auto pMFMP : m_MethodPointers)
    {
        if (pMFMP->getObjectType()->isSame(a_pObjectType) &&
            pMFMP->getFunctionType()->getReturnType()->isSame(a_pReturnType) &&
            pMFMP->getFunctionType()->matches(a_ParameterTypes, a_RefQualifiers, a_uiFlags))
        {
            return pMFMP;
        }
    }

    MethodPointer* pPointer = NewDeferred<MethodPointer>(
    a_pObjectType, functionType(a_pReturnType, a_ParameterTypes, a_RefQualifiers), a_RefQualifiers, a_uiFlags);
    m_MethodPointers.push_back(pPointer);
    return pPointer;
}

FieldPointer* Source::fieldPointerType(ClassType* a_pObjectType, Type* a_pValueType, Modifiers a_Modifiers,
                                       uint a_uiFlags)
{
    for (auto pDMP : m_FieldPointers)
    {
        if (pDMP->getObjectType()->isSame(a_pObjectType) && pDMP->getValueType()->isSame(a_pValueType))
        {
            return pDMP;
        }
    }
    FieldPointer* pPointer = NewDeferred<FieldPointer>(a_pObjectType, a_pValueType, a_Modifiers, a_uiFlags);
    m_FieldPointers.push_back(pPointer);
    return pPointer;
}

void Source::setSourceStream(SourceStream* a_pStream)
{
    if (m_pSourceStream == a_pStream)
        return;
    m_pSourceStream = a_pStream;
    PHANTOM_EMIT sourceStreamChanged(a_pStream);
}

FunctionType* Source::functionType(Type* a_pReturnType, TypesView a_ParameterTypes, Modifiers a_Modifiers, uint a_Flags)
{
    for (auto pFT : m_FunctionTypes)
    {
        if (pFT->getReturnType()->isSame(a_pReturnType) && pFT->matches(a_ParameterTypes, a_Modifiers))
        {
            return pFT;
        }
    }
    FunctionType* pType = NewDeferred<FunctionType>(a_pReturnType, a_ParameterTypes, a_Modifiers,
                                                    a_Flags | (isNative() * PHANTOM_R_FLAG_NATIVE));
    m_FunctionTypes.push_back(pType);
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
    for (auto pFP : m_FunctionPointers)
    {
        if (pFP->getABI() == a_eABI && pFP->getFunctionType()->isSame(a_pFunctionType))
            return pFP;
    }

    FunctionPointer* pPointer =
    NewDeferred<FunctionPointer>(a_pFunctionType, a_eABI, a_Qualifiers, a_Flags | (isNative() * PHANTOM_R_FLAG_NATIVE));
    m_FunctionPointers.push_back(pPointer);
    return pPointer;
}

InitializerListType* Source::initializerListType(TypesView a_Types)
{
    for (auto pIT : m_InitializerListTypes)
    {
        if (pIT->matches(a_Types))
        {
            return pIT;
        }
    }
    InitializerListType* pIT = New<InitializerListType>(a_Types);
    if (isNative())
        pIT->setFlag(PHANTOM_R_FLAG_NATIVE);
    m_InitializerListTypes.push_back(pIT);
    return pIT;
}

bool Source::addImport(Symbol* a_pSymbol, bool a_bStatic, bool a_bPublic)
{
    PHANTOM_ASSERT(a_pSymbol != this);
    if (!(canImport(a_pSymbol, a_bPublic ? Access::Public : Access::Private,
                    Modifiers(a_bStatic ? Modifier::Static : 0))))
    {
        return false;
    }
    PHANTOM_ASSERT(a_pSymbol);
    Import i;
    i.symbol = a_pSymbol;
    i.isPublic = a_bPublic;
    i.isStatic = a_bStatic;
    Alias* pAlias = nullptr;
    if (a_bStatic)
    {
        PackageFolders folders;
        a_pSymbol->getPackage()->getPackageFolder()->getPackageFolders(folders);
        for (auto it = folders.begin(); it != folders.end(); ++it)
        {
            if (it == folders.begin())
            {
                pAlias = getAlias((*it)->getName());
                if (pAlias == nullptr)
                {
                    pAlias = New<Alias>((*it)->getName(), (a_bStatic ? PHANTOM_R_STATIC : PHANTOM_R_NONE),
                                        PHANTOM_R_FLAG_IMPLICIT);
                    pAlias->setVisibility(a_bPublic ? Visibility::Public : Visibility::Protected);
                    addAlias(pAlias);
                }
            }
            else
            {
                auto pSubAlias = pAlias->getAlias((*it)->getName());
                if (pSubAlias == nullptr)
                {
                    pSubAlias = New<Alias>((*it)->getName(), PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
                    pAlias->addAlias(pSubAlias);
                }
                pAlias = pSubAlias;
            }
        }
        auto pLastNamedAlias = New<Alias>(a_pSymbol, a_pSymbol->getName(), PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
        if (pAlias)
            pAlias->addAlias(pLastNamedAlias);
        else
            addAlias(pLastNamedAlias);
    }
    else
    {
        addAlias(pAlias =
                 New<Alias>(a_pSymbol, "", (a_bStatic ? PHANTOM_R_STATIC : PHANTOM_R_NONE), PHANTOM_R_FLAG_IMPLICIT));
        pAlias->setVisibility(a_bPublic ? Visibility::Public : Visibility::Protected);
    }
    i.alias = pAlias;
    m_Imports.push_back(i);
    return true;
}

bool Source::addImport(StringView a_strName, bool a_bStatic, bool a_bPublic)
{
    Source* pSource = Application::Get()->getSource(a_strName);
    if (pSource == nullptr || pSource == this)
    {
        PHANTOM_LOG(Error, "cannot import source '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
        return false;
    }
    return addImport(pSource, a_bStatic, a_bPublic);
}

void Source_fetchImportedSymbols(const Symbol* a_pSymbol, Symbols& a_Symbols, SmallSet<const Symbol*>& treated,
                                 Visibility _maxVisibility)
{
    if (!treated.insert(a_pSymbol).second)
        return;
    for (auto p : a_pSymbol->getElements())
    {
        Symbol* pSymbol = p->asSymbol();
        if (pSymbol // is a symbol && is publicly imported
            && (pSymbol->getVisibility() <= _maxVisibility) && pSymbol->asTemplateSpecialization() == nullptr)
        {
            if (pSymbol->getName().empty())
            {
                if (Alias* pAlias = pSymbol->asAlias())
                    pSymbol = pAlias->getAliasedSymbol();
                Source_fetchImportedSymbols(pSymbol, a_Symbols, treated, Visibility::Public);
            }
            else
            {
                a_Symbols.push_back(pSymbol);
            }
        }
    }
}
void Source_fetchImportedSymbols(const Symbol* a_pSymbol, Symbols& a_Symbols, Visibility _maxVisibility)
{
    SmallSet<const Symbol*> treated;
    Source_fetchImportedSymbols(a_pSymbol, a_Symbols, treated, _maxVisibility);
}

bool Source::canImport(Symbol* a_pSymbol, Access, Modifiers a_Modifiers /*= 0*/, uint /*= 0*/,
                       SmallMap<Symbol*, Symbols>* a_pCollidingSymbols /*= nullptr*/) const
{
    if (a_pSymbol == this)
        return false;
    if (a_pSymbol->asSubroutine())
        return false;
    if (a_Modifiers & PHANTOM_R_STATIC)
        return true;
    Symbols symbols0;
    Symbols symbols1;
    Source_fetchImportedSymbols(this, symbols0, Visibility::Protected);
    Source_fetchImportedSymbols(a_pSymbol, symbols1, Visibility::Public);
    bool result = true;
    for (Symbol* pSymbol0 : symbols0)
    {
        for (Symbol* pSymbol1 : symbols1)
        {
            if (pSymbol0 != pSymbol1)
            {
                if (pSymbol1->getName() == pSymbol0->getName())
                {
                    bool sr0 = pSymbol0->asSubroutine();
                    bool sr1 = pSymbol1->asSubroutine();
                    if (sr0 && sr1)
                    {
                        if (pSymbol0->getDecoratedName() != pSymbol1->getDecoratedName())
                            continue;
                    }
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
    return result;
}

bool Source::hasImport(Symbol* a_pSym) const
{
    for (auto it = m_Imports.begin(); it != m_Imports.end(); ++it)
    {
        if (it->symbol == a_pSym)
            return true;
    }
    return false;
}

static void Source_getImported(const Symbol* _this, Symbols& a_Imports, SmallSet<const Symbol*>& treated)
{
    if (treated.insert(_this).second)
    {
        if (Source* pThisSource = _this->asSource())
            for (Source::Import const& import : pThisSource->getImports())
            {
                a_Imports.push_back(import.symbol);
                if (import.isPublic)
                {
                    Source_getImported(import.symbol, a_Imports, treated);
                }
            }
    }
}

void Source::getImported(Symbols& a_Imports) const
{
    SmallSet<const Symbol*> treated;
    Source_getImported(this, a_Imports, treated);
}

bool Source::_hasImported(Symbol* a_pSymbol, SmallSet<const Source*>& treated) const
{
    if (treated.insert(this).second)
    {
        for (Source::Import const& import : m_Imports)
        {
            if (import.symbol == a_pSymbol)
                return true;
            if (Source* importSource = import.symbol->asSource())
                if (import.isPublic && importSource->_hasImported(a_pSymbol, treated))
                    return true;
        }
    }
    return false;
}

bool Source::hasImported(Symbol* a_pSource) const
{
    SmallSet<const Source*> treated;
    return _hasImported(a_pSource, treated);
}

Module* Source::getModule() const
{
    return getPackage()->getModule();
}

} // namespace lang
} // namespace phantom
