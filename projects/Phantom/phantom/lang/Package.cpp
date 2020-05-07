// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Package.h"

#include "PackageFolder.h"

#include <phantom/utils/StringUtil.h>
#ifndef __DOXYGEN__
#    include <phantom/utils/Path.h>
#endif
#include "Application.h"
#include "Module.h"
#include "Namespace.h"
#include "Source.h"

#include <phantom/detail/core_internal.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
bool Package::IsValidName(StringView a_strName)
{
    if (a_strName.empty())
        return false;
    char prevChar = '.';
    for (auto it = a_strName.begin(); it != a_strName.end(); ++it)
    {
        char c = *it;
        if (c == '.')
        {
            if (prevChar == '.')
                return false;
            prevChar = c;
            continue;
        }
        if (!((((c) >= '0') && ((c) <= '9')) || (((c | 0x20) >= 'a') && ((c | 0x20) <= 'z')) || (c) == '_'))
            return false;
        prevChar = c;
    }
    return true;
}

Package::Package(StringView a_strName) : Symbol(a_strName, 0, PHANTOM_R_ALWAYS_VALID | PHANTOM_R_INTERNAL_FLAG_SPECIAL)
{
    PHANTOM_ASSERT(IsValidName(a_strName));
    Strings folders;
    StringUtil::Split(folders, a_strName, ".");
    m_pFolder = Application::Get()->rootPackageFolder();
    for (auto it = folders.begin(); it != folders.end(); ++it)
    {
        PackageFolder* pChild = static_cast<PackageFolder*>(m_pFolder->getUniqueElement(*it));
        if (pChild)
        {
            m_pFolder = pChild;
        }
        else
        {
            m_pFolder = m_pFolder->newPackageFolder(*it);
        }
    }
    m_pFolder->_addPackage(this);
    addReferencedElement(m_pFolder);
}

Package::~Package() {}

void Package::terminate()
{
    if (m_pFolder)
        m_pFolder->_removePackage(this);
    Symbol::terminate();
}

bool Package::canBeUnloaded() const
{
    for (auto it = m_Sources.begin(); it != m_Sources.end(); ++it)
    {
        if (!((*it)->canBeUnloaded()))
            return false;
    }
    return true;
}

void Package::addArchivedSource(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource->getVisibility() == Visibility::Private);
    PHANTOM_ASSERT(getModule());
    PHANTOM_ASSERT(getSource(a_pSource->getName()) == nullptr);
    m_ArchivedSources.push_back(a_pSource);
    a_pSource->setOwner(this);
}

void Package::removeArchivedSource(Source* a_pSource)
{
    auto found = std::find(m_ArchivedSources.begin(), m_ArchivedSources.end(), a_pSource);
    PHANTOM_ASSERT(found != m_ArchivedSources.end(), "use removeArchivedSource instead");
    a_pSource->setOwner(nullptr);
    m_ArchivedSources.erase(found);
}

Module* Package::getModule() const
{
    return static_cast<Module*>(getOwner());
}

Source* Package::getSource(StringView a_strName) const
{
    for (auto it = m_Sources.begin(); it != m_Sources.end(); ++it)
    {
        if ((*it)->getName() == a_strName)
            return *it;
    }
    return nullptr;
}

Source* Package::getOrCreateSource(StringView a_strName, Visibility a_Visibility)
{
    if (Source* pSource = getSource(a_strName))
        return pSource;
    Source* pSource = newSource(a_strName, a_Visibility);
    addSource(pSource);
    return pSource;
}

Source* Package::newSource(StringView a_strName, Visibility a_Visibility)
{
    PHANTOM_ASSERT(getPackageFolder()->getPackageFolder(a_strName) == nullptr);
    PHANTOM_ASSERT(getModule());
    Source* pS = phantom::new_<Source>(a_strName);
    pS->m_pSource = pS;
    pS->setVisibility(a_Visibility);
    pS->rtti.instance = pS;
    if (dynamic_initializer_()->installed())
    {
        pS->rtti.metaClass = PHANTOM_CLASSOF(Source);
        pS->rtti.metaClass->registerInstance(pS);
    }
    else
    {
        phantom::detail::deferInstallation("phantom::lang::Source", &pS->rtti);
    }
    pS->initialize();
    return pS;
}

void Package::deleteSource(Source* a_pSource)
{
    PHANTOM_ASSERT(getSource(a_pSource->getName()) != a_pSource,
                   "use removeSource before deleteSource instead, I won't do it for you :p");
    PHANTOM_CLASSOF(Source)->unregisterInstance(a_pSource);
    a_pSource->_terminate();
    phantom::delete_<Source>(a_pSource);
}

Namespace* Package::getCounterpartNamespace() const
{
    if (m_pCPNamespace == nullptr)
    {
        String namespaceName = getName();
        StringUtil::ReplaceAll(namespaceName, ".", "::");
        m_pCPNamespace = Namespace::Global()->getOrCreateNamespace(namespaceName);
        PHANTOM_ASSERT(m_pCPNamespace);
        const_cast<Package*>(this)->addReferencedElement(m_pCPNamespace);
    }
    return m_pCPNamespace;
}

void Package::addSource(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource->m_pOwner == nullptr);
    PHANTOM_ASSERT(getSource(a_pSource->getName()) == nullptr,
                   "a source with this name already exists in this package");
    a_pSource->setOwner(this);
    m_Sources.push_back(a_pSource);
    a_pSource->setVisibility(Visibility::Public);
    PHANTOM_EMIT sourceAdded(a_pSource);
    Application::Get()->_sourceAdded(a_pSource);
}

void Package::removeSource(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource->m_pOwner == this);
    Application::Get()->_sourceAboutToBeRemoved(a_pSource);
    PHANTOM_EMIT sourceAboutToBeRemoved(a_pSource);
    a_pSource->setVisibility(Visibility::Private);
    m_Sources.erase_unsorted(std::next(std::find(m_Sources.rbegin(), m_Sources.rend(), a_pSource)).base());
    a_pSource->setOwner(nullptr);
}

hash64 Package::computeHash() const
{
    StringBuffer buffer;
    getUniqueName(buffer);
    return ComputeHash(buffer.c_str(), buffer.size());
}

void Package::getQualifiedName(StringBuffer& a_Buf) const
{
    return getName(a_Buf);
}

void Package::getUniqueName(StringBuffer& a_Buf) const
{
    return getName(a_Buf);
}

String Package::pathName() const
{
    String name = getName();
    StringUtil::ReplaceAll(name, ".", "/");
    return name;
}

void Package::getPackageFolders(PackageFolders& a_Folders) const
{
    m_pFolder->getPackageFolders(a_Folders);
}

} // namespace lang
} // namespace phantom
