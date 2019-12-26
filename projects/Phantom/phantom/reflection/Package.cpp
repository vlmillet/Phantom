// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Package.h"
#include "PackageFolder.h"

#include <phantom/StringUtil.h>
#ifndef __DOXYGEN__
#    include <phantom/Path.h>
#endif
#include "Application.h"
#include "Module.h"
#include "Namespace.h"
#include "Source.h"

#include <phantom/new.h>
#include <phantom/phantom_priv.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
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
        if (NOT((((c) >= '0') AND((c) <= '9')) OR(((c | 0x20) >= 'a') AND((c | 0x20) <= 'z'))
                OR(c) == '_'))
            return false;
        prevChar = c;
    }
    return true;
}

Package::Package(StringView a_strName)
    : Symbol(a_strName, 0, PHANTOM_R_ALWAYS_VALID), m_pNamespace(nullptr)
{
    PHANTOM_ASSERT(IsValidName(a_strName));
    String namespaceName = m_strName;
    StringUtil::ReplaceAll(namespaceName, ".", "_");
    m_pNamespace = Namespace::Global()->findOrCreateNamespace(namespaceName);
    PHANTOM_ASSERT(m_pNamespace);
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
            PackageFolder* pFolder;
            m_pFolder->addPackageFolder(pFolder = PHANTOM_DEFERRED_NEW(PackageFolder)(*it));
            m_pFolder = pFolder;
        }
    }
    addReferencedElement(m_pFolder);
    addReferencedElement(m_pNamespace);
    m_pFolder->_addPackage(this);
}

Package::~Package()
{
}

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
        if (NOT((*it)->canBeUnloaded()))
            return false;
    }
    return true;
}

void Package::addSource(Source* a_pSource)
{
    PHANTOM_ASSERT(getModule());
    PHANTOM_ASSERT(a_pSource->getOwner() == nullptr);
    PHANTOM_ASSERT(getSource(a_pSource->getName()) == nullptr);
    addElement(a_pSource);
}

void Package::addArchivedSource(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource->testFlags(PHANTOM_R_FLAG_PRIVATE_VIS));
    PHANTOM_ASSERT(getModule());
    PHANTOM_ASSERT(getSource(a_pSource->getName()) == nullptr);
    m_ArchivedSources.push_back(a_pSource);
    addElement(a_pSource);
}

void Package::onElementAdded(LanguageElement* a_pElement)
{
    if (Source* pSource = a_pElement->asSource())
    {
        if (std::find(m_ArchivedSources.begin(), m_ArchivedSources.end(), pSource) ==
            m_ArchivedSources.end())
        {
            m_Sources.push_back(pSource);
            Module* pModule = getModule();
            PHANTOM_ASSERT(pModule);
            PHANTOM_EMIT sourceAdded(pSource);
            Application::Get()->_sourceAdded(pSource);
        }
    }
}

void Package::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    if (m_pFolder == a_pElement)
        m_pFolder = nullptr;
}

void Package::onElementRemoved(LanguageElement* a_pElement)
{
    if (a_pElement->asNamespace())
    {
        m_pNamespace = nullptr;
    }
    else if (Source* pSource = a_pElement->asSource())
    {
        auto archivedFound = std::find(m_ArchivedSources.begin(), m_ArchivedSources.end(), pSource);
        if (archivedFound == m_ArchivedSources.end())
        {
            Application::Get()->_sourceAboutToBeRemoved(pSource);
            PHANTOM_EMIT sourceAboutToBeRemoved(pSource);
            PHANTOM_ASSERT(pSource->getOwner() == this);
            m_Sources.erase(std::find(m_Sources.begin(), m_Sources.end(), pSource));
            Module* pModule = getModule();
            PHANTOM_ASSERT(pModule);
        }
        else
        {
            m_ArchivedSources.erase(
            std::find(m_ArchivedSources.begin(), m_ArchivedSources.end(), pSource));
        }
    }
}

void Package::removeSource(Source* a_pSource)
{
    removeElement(a_pSource);
}

void Package::deleteSource(Source* a_pSource)
{
    PHANTOM_ASSERT(a_pSource->getOwner() == this);
    PHANTOM_DELETE_DYN a_pSource;
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

void Package::setNamespace(Namespace* a_pNamespace)
{
    PHANTOM_ASSERT((m_pNamespace == nullptr) != (a_pNamespace == nullptr));
    if (a_pNamespace == nullptr)
    {
        removeElement(a_pNamespace);
    }
    else
    {
        m_pNamespace = a_pNamespace;
        addElement(m_pNamespace);
    }
}

hash64 Package::computeHash() const
{
    StringBuffer buffer;
    getUniqueName(buffer);
    return ComputeHash(buffer.c_str(), buffer.size());
}

void Package::onAncestorChanged(LanguageElement* a_pAncestor)
{
    if (Module* pModule = a_pAncestor->asModule())
    {
        pModule->markOutdated();
    }
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

} // namespace reflection
} // namespace phantom
