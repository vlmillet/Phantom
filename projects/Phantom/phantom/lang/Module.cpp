// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Module.h"

#include "Application.h"
#include "Package.h"
#include "Plugin.h"
#include "Source.h"
#include "SourceStream.h"
#include "TemplateSpecialization.h"
#include "phantom/detail/core_internal.h"
#include "phantom/detail/new.h"

#include <fstream>
#include <phantom/utils/Path.h>
#include <phantom/utils/StringUtil.h>
#include <system_error>
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include <windows.h>
#endif
/* *********************************************** */
namespace phantom
{
namespace lang
{
bool g_ReleasingPhantomModule;

#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "cleanup Module ctor arguments")
#endif
Module::Module(size_t a_PlatformHandle, StringView a_strName, StringView a_LibraryFullName,
               StringView a_DeclarationCppFullName, uint a_uiFlags)
    : Symbol(a_strName, Modifier::None, a_uiFlags | PHANTOM_R_ALWAYS_VALID),
      m_NativeHandle(a_PlatformHandle),
      m_LibraryFullName(a_LibraryFullName),
      m_DeclarationCppFullName(a_DeclarationCppFullName)
{
    Package* pDefaultPackage = PHANTOM_DEFERRED_NEW(Package)(a_strName);
    if (isNative())
        pDefaultPackage->setFlag(PHANTOM_R_FLAG_NATIVE);
    addPackage(pDefaultPackage);
    m_pAnonymousSource = PHANTOM_DEFERRED_NEW(Source)("", Modifier::None);
    if (isNative())
        m_pAnonymousSource->setFlag(PHANTOM_R_FLAG_NATIVE);
    pDefaultPackage->addSource(m_pAnonymousSource);
}

Module::Module(StringView a_strName, uint a_uiFlags /*= 0*/)
    : Module(0, a_strName, StringView(), StringView(), a_uiFlags)
{
}

void Module::terminate()
{
    setFlag(PHANTOM_R_FLAG_TERMINATED);

    if (isNative() && getName() == "Phantom")
    {
        g_ReleasingPhantomModule = true;
        PHANTOM_ASSERT(Application::Get() && Application::Get()->getMainModule() == nullptr);
        LanguageElements allElements;
        fetchElementsDeep(allElements);

        if (size_t allElementCount = allElements.size())
        {
            //             std::sort(allElements.begin(), allElements.end(), [](LanguageElement* l0, LanguageElement*
            //             l1) -> bool {
            //                 return l0->destructionPriority() < l1->destructionPriority();
            //             });

            // first pass we skip meta meta classes
            while (allElementCount--)
            {
                if (allElements[allElementCount] == Class::MetaClass() ||
                    allElements[allElementCount]->getMetaClass()->getMetaClass() == Class::MetaClass())
                    continue;
                if (allElements[allElementCount]->isNative())
                    allElements[allElementCount]->_nativeDetachElementsFromModule();
                PHANTOM_DELETE_DYN allElements[allElementCount];
            }

            allElements.clear();
            fetchElementsDeep(allElements);
            allElementCount = allElements.size();

            // first pass we skip meta meta classes
            while (allElementCount--)
            {
                if (allElements[allElementCount] == Class::MetaClass() ||
                    allElements[allElementCount]->getMetaClass() == Class::MetaClass())
                    continue;
                if (allElements[allElementCount]->isNative())
                    allElements[allElementCount]->_nativeDetachElementsFromModule();
                PHANTOM_DELETE_DYN allElements[allElementCount];
            }

            // second pass we skip only the "master" meta class (the meta class of Class)

            allElements.clear();
            fetchElementsDeep(allElements);
            allElementCount = allElements.size();

            while (allElementCount--)
            {
                if (allElements[allElementCount] == Class::MetaClass())
                    continue;
                if (allElements[allElementCount]->isNative())
                    allElements[allElementCount]->_nativeDetachElementsFromModule();
                PHANTOM_DELETE_DYN allElements[allElementCount];
            }

            // then we check we removed every body

            allElements.clear();
            PHANTOM_ASSERT(([this, &allElements]() -> bool {
                fetchElementsDeep(allElements);
                return allElements.empty();
            }()));
        }
        g_ReleasingPhantomModule = false;
    }
    else if (isNative())
    {
        StaticGlobals::Release((void*)m_NativeHandle);
    }
    m_pPlugin = nullptr;
    Symbol::terminate();
}

Module::~Module() {}

bool Module::canBeUnloaded() const
{
    for (auto it = m_Packages.begin(); it != m_Packages.end(); ++it)
    {
        if (NOT((*it)->canBeUnloaded()))
            return false;
    }
    return canBeDestroyed();
}

Package* Module::getDefaultPackage() const
{
    PHANTOM_ASSERT(m_Packages.size() && m_Packages.front()->getName() == getName());
    return m_Packages.front();
}

void Module::addPackage(Package* a_pPackage)
{
    Package* pPackage = getPackage(a_pPackage->getName());
    PHANTOM_ASSERT(pPackage == nullptr, "package already exists in this module");
    PHANTOM_ASSERT(a_pPackage->getModule() == nullptr);
    if (isNative())
        a_pPackage->m_uiFlags |= PHANTOM_R_FLAG_NATIVE;
    addElement(a_pPackage);
}

void Module::removePackage(Package* a_pPackage)
{
    removeElement(a_pPackage);
}

void Module::addDependency(Module* a_pModule)
{
    PHANTOM_ASSERT(!hasDependency(a_pModule), "dependency already exists");
    PHANTOM_ASSERT(!a_pModule->hasDependencyCascade((Module*)this), "cyclic module dependency detected");
    m_Dependencies.push_back(a_pModule);
}

void Module::removeDependency(Module* a_pModule)
{
    PHANTOM_ASSERT(hasDependency(a_pModule), "dependency does not exist");
    m_Dependencies.erase(std::find(m_Dependencies.begin(), m_Dependencies.end(), a_pModule));
}

bool Module::hasDependency(Module* a_pModule) const
{
    Modules modules;
    fetchDependencies(modules);
    return std::find(modules.begin(), modules.end(), a_pModule) != modules.end();
}

bool Module::hasDependencyCascade(Module* a_pModule) const
{
    Modules modules;
    fetchDependencies(modules);
    if (std::find(modules.begin(), modules.end(), a_pModule) != modules.end())
        return true;
    for (auto module : modules)
    {
        if (module->hasDependencyCascade(a_pModule))
            return true;
    }
    return false;
}

void Module::onElementAdded(LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(!g_ReleasingPhantomModule);
    if (a_pElement->asPackage())
    {
        m_Packages.push_back(static_cast<Package*>(a_pElement));
        PHANTOM_EMIT packageAdded(static_cast<Package*>(a_pElement));
        Application::Get()->packageAdded(static_cast<Package*>(a_pElement));
    }
}

void Module::onElementRemoved(LanguageElement* a_pElement)
{
    if (a_pElement->asPackage())
    {
        Application::Get()->packageAboutToBeRemoved(static_cast<Package*>(a_pElement));
        PHANTOM_EMIT packageAboutToBeRemoved(static_cast<Package*>(a_pElement));
        m_Packages.erase(std::find(m_Packages.begin(), m_Packages.end(), static_cast<Package*>(a_pElement)));
    }
}

bool Module::isPlugin() const
{
    return Application::Get()->getMainModule() != this;
}

StringView Module::getLibraryFullName() const
{
    return m_LibraryFullName;
}

StringView Module::getDeclarationCppFullName() const
{
    return m_DeclarationCppFullName;
}

byte* Module::getMemoryStart() const
{
    if (!isNative())
        return 0;
    return reinterpret_cast<byte*>(m_NativeHandle);
}

Package* Module::getPackage(StringView a_strName) const
{
    for (auto p : m_Packages)
    {
        if (a_strName == p->getName())
            return p;
    }
    return nullptr;
}

void Module::_registerType(hash64 a_Hash, Type* a_pType)
{
    auto& pType = m_HashToTypeMap[a_Hash];
    PHANTOM_ASSERT(pType == nullptr);
    pType = a_pType;
}

void Module::_unregisterType(hash64 a_Hash, Type* a_pType)
{
    auto found = m_HashToTypeMap.find(a_Hash);
    PHANTOM_ASSERT(found != m_HashToTypeMap.end());
    PHANTOM_ASSERT(found->second == a_pType);
    m_HashToTypeMap.erase(found);
}

void Module::getSources(Sources& a_Sources) const
{
    for (Package* p : m_Packages)
    {
        for (Source* s : p->getSources())
        {
            if (!s->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS))
                a_Sources.push_back(s);
        }
    }
}

void Module::fetchDependencies(Modules& a_Modules) const
{
    if (isNative())
    {
        a_Modules.insert(a_Modules.end(), m_Dependencies.begin(), m_Dependencies.end());
        return;
    }
}

void Module::findClasses(Classes& a_Classes, lang::Class* a_pBaseClass /*= nullptr*/,
                         bool a_bDefaultInstanciable /*= false*/) const
{
    for (Package* p : m_Packages)
    {
        for (Source* s : p->getSources())
        {
            for (Type* t : s->getTypes())
            {
                Class* pClass = t->asClass();
                if (pClass AND(a_pBaseClass ==
                               nullptr OR(pClass->isA(a_pBaseClass) AND NOT(pClass->isSame(a_pBaseClass))))
                    AND(a_bDefaultInstanciable == false OR pClass->isDefaultInstanciable()))
                    a_Classes.push_back(pClass);
            }

            for (auto t : s->getTemplateSpecializations())
            {
                Class* pClass = t->getTemplated() ? t->getTemplated()->asClass() : nullptr;
                if (pClass AND(a_pBaseClass ==
                               nullptr OR(pClass->isA(a_pBaseClass) AND NOT(pClass->isSame(a_pBaseClass))))
                    AND(a_bDefaultInstanciable == false OR pClass->isDefaultInstanciable()))
                    a_Classes.push_back(pClass);
            }
        }
    }
}

void Module::findFunctions(Functions& a_Functions, StringView a_Name /*= ""*/,
                           const Types* a_pParameterTypes /*= nullptr*/, Type* a_pReturnType /*= nullptr*/) const
{
    for (Package* p : m_Packages)
    {
        for (Source* s : p->getSources())
        {
            s->findFunctions(a_Functions, a_Name, a_pParameterTypes, a_pReturnType);
        }
    }
}

Type* Module::findType(hash64 a_Hash) const
{
    auto found = m_HashToTypeMap.find(a_Hash);
    return found == m_HashToTypeMap.end() ? nullptr : found->second;
}

namespace
{
Type* Module_findUsableType(Module const* a_pThis, hash64 a_Hash, SmallSet<Module const*>& a_Treated)
{
    if (!a_Treated.insert(a_pThis).second)
        return nullptr;
    if (Type* pType = a_pThis->findType(a_Hash))
        return pType;
    Modules deps;
    a_pThis->fetchDependencies(deps);
    for (auto pModule : deps)
        if (Type* pType = Module_findUsableType(pModule, a_Hash, a_Treated))
            return pType;
    for (auto pStartupModule : Application::Get()->getStartupModules())
    {
        if (pStartupModule == a_pThis)
            continue;
        if (Type* pType = Module_findUsableType(pStartupModule, a_Hash, a_Treated))
            return pType;
    }
    return nullptr;
}
} // namespace

Type* Module::findUsableType(hash64 a_Hash) const
{
    SmallSet<Module const*> treated;
    return Module_findUsableType(this, a_Hash, treated);
}

hash64 Module::computeHash() const
{
    StringBuffer buffer;
    getUniqueName(buffer);
    return ComputeHash(buffer.c_str(), buffer.size());
}

} // namespace lang
} // namespace phantom
