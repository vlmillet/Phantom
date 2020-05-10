// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
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
Module::Module(size_t a_NativeHandle, size_t a_NativeImageSize, StringView a_strName, StringView a_LibraryFullName,
               StringView a_DeclarationCppFullName, uint a_uiFlags)
    : Symbol(a_strName, Modifier::None, a_uiFlags | PHANTOM_R_INTERNAL_FLAG_SPECIAL | PHANTOM_R_ALWAYS_VALID),
      m_pBaseAddress((void*)a_NativeHandle),
      m_ImageSize(a_NativeImageSize),
      m_LibraryFullName(a_LibraryFullName),
      m_DeclarationCppFullName(a_DeclarationCppFullName),
      m_Allocator(isNative() ? 65536 : 1024)
{
}

Module::Module(StringView a_strName, uint a_uiFlags /*= 0*/)
    : Module(0, 0, a_strName, StringView(), StringView(), a_uiFlags | PHANTOM_R_INTERNAL_FLAG_SPECIAL)
{
}

void Module::terminate()
{
    m_pPlugin = nullptr;
    Symbol::terminate();
}

Module::~Module()
{
    if (isNative())
    {
        StaticGlobals::Release(m_pBaseAddress, reinterpret_cast<uint8_t*>(m_pBaseAddress) + m_ImageSize);
    }
}

void Module::initialize()
{
    Symbol::initialize();
    Package* pDefaultPackage = newPackage(getName());
    if (isNative())
        pDefaultPackage->setFlag(PHANTOM_R_FLAG_NATIVE);
    m_pAnonymousSource = pDefaultPackage->getOrCreateSource("default");
    if (isNative())
        m_pAnonymousSource->setFlag(PHANTOM_R_FLAG_NATIVE);
}

bool Module::canBeUnloaded() const
{
    for (auto it = m_Packages.begin(); it != m_Packages.end(); ++it)
    {
        if (!((*it)->canBeUnloaded()))
            return false;
    }
    return canBeDestroyed();
}

Package* Module::getDefaultPackage() const
{
    PHANTOM_ASSERT(m_Packages.size() && m_Packages.front()->getName() == getName());
    return m_Packages.front();
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

int Module::getDependencyLevel() const
{
    int maxLevel = 0;
    for (auto pDep : m_Dependencies)
    {
        int level = pDep->getDependencyLevel() + 1;
        if (level > maxLevel)
            maxLevel = level;
    }
    return maxLevel;
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
    return reinterpret_cast<byte*>(m_pBaseAddress);
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

Package* Module::getOrCreatePackage(StringView a_strName)
{
    if (Package* pPck = getPackage(a_strName))
        return pPck;
    return newPackage(a_strName);
}

Package* Module::newPackage(StringView a_strName)
{
    Package* pPck = phantom::new_<Package>(a_strName);
    pPck->rtti.instance = pPck;
    if (dynamic_initializer_()->installed())
    {
        pPck->rtti.metaClass = PHANTOM_CLASSOF(Package);
        pPck->rtti.metaClass->registerInstance(pPck);
    }
    else
    {
        phantom::detail::deferInstallation("phantom::lang::Package", &pPck->rtti);
    }
    pPck->setOwner(this);
    pPck->initialize();
    m_Packages.push_back(pPck);
    PHANTOM_EMIT packageAdded(pPck);
    Application::Get()->packageAdded(pPck);
    return pPck;
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
            if (s->getVisibility() != Visibility::Private)
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
            s->findClasses(a_Classes, a_pBaseClass, a_bDefaultInstanciable);
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
