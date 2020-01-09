// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "PackageFolder.h"

#include "Package.h"
#include "PackageFolder.hxx"

#include <phantom/utils/StringUtil.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
PHANTOM_DEFINE_META_CLASS(PackageFolder);

PackageFolder* PackageFolder::Create()
{
    return PHANTOM_DEFERRED_NEW_EX(PackageFolder);
}

PackageFolder::PackageFolder() : Symbol("", PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE)
{
}

PackageFolder::PackageFolder(StringView a_strName) : Symbol(a_strName, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE)
{
}

PackageFolder::~PackageFolder()
{
}

void PackageFolder::terminate()
{
    for (auto& pPackage : m_Packages)
        pPackage->m_pFolder = nullptr;
    Symbol::terminate();
}

void PackageFolder::onElementRemoved(LanguageElement* a_pElement)
{
    auto found = std::find(m_PackageFolders.begin(), m_PackageFolders.end(), static_cast<PackageFolder*>(a_pElement));
    if (found != m_PackageFolders.end())
    {
        PHANTOM_EMIT packageFolderAboutToBeRemoved(static_cast<PackageFolder*>(a_pElement));
        m_PackageFolders.erase(found);
    }
}

void PackageFolder::_addPackage(Package* a_pPackage)
{
    m_Packages.push_back(a_pPackage);
    addReferencedElement(a_pPackage);
}

void PackageFolder::_removePackage(Package* a_pPackage)
{
    removeReferencedElement(a_pPackage);
}

void PackageFolder::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    for (auto& pPackage : m_Packages)
        if (pPackage == a_pElement)
            m_Packages.erase(&pPackage);
}

void PackageFolder::onElementAdded(LanguageElement* a_pElement)
{
    if (a_pElement->asPackageFolder())
    {
        m_PackageFolders.push_back(static_cast<PackageFolder*>(a_pElement));
        PHANTOM_EMIT packageFolderAdded(static_cast<PackageFolder*>(a_pElement));
    }
    Symbol::onElementAdded(a_pElement);
}

Package* PackageFolder::getPackage(Module* a_pModule) const
{
    for (auto pPack : m_Packages)
        if (pPack->getModule() == a_pModule)
            return pPack;
    return nullptr;
}

void PackageFolder::getPackageFolders(PackageFolders& a_Folders) const
{
    if (getParentFolder())
    {
        getParentFolder()->getPackageFolders(a_Folders);
        a_Folders.push_back(const_cast<PackageFolder*>(this));
    }
}

PackageFolder* PackageFolder::getPackageFolder(StringView a_strUniqueName) const
{
    Strings strings;
    StringUtil::Split(strings, a_strUniqueName, ".", false);
    PackageFolder* pCurrFolder = (PackageFolder*)this;
    for (auto& str : strings)
    {
        PackageFolder* pSubFolder = nullptr;
        for (auto pFolder : pCurrFolder->m_PackageFolders)
        {
            if (pFolder->getName() == str)
            {
                pSubFolder = pFolder;
                break;
            }
        }
        if (!pSubFolder)
            return nullptr;
        pCurrFolder = pSubFolder;
    }
    return pCurrFolder;
}

PackageFolder* PackageFolder::getOrCreatePackageFolder(StringView a_strUniqueName) const
{
    Strings strings;
    StringUtil::Split(strings, a_strUniqueName, ".", false);
    PackageFolder* pCurrFolder = (PackageFolder*)this;
    for (auto& str : strings)
    {
        PackageFolder* pNewFolder = pCurrFolder->getPackageFolder(str);
        if (pNewFolder == nullptr)
        {
            pCurrFolder->addPackageFolder((pNewFolder = Create()));
        }
        pCurrFolder = pNewFolder;
    }
    return pCurrFolder;
}

Source* PackageFolder::getSource(StringView a_strName) const
{
    for (Package* pPck : m_Packages)
    {
        if (Source* pSrc = pPck->getSource(a_strName))
            return pSrc;
    }
    return nullptr;
}

} // namespace reflection
} // namespace phantom
