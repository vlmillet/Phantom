// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PackageFolder.h"

#include "Package.h"
#include "PackageFolder.hxx"

#include <phantom/detail/core_internal.h>
#include <phantom/utils/StringUtil.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
PHANTOM_DEFINE_META_CLASS(PackageFolder);

PackageFolder::PackageFolder() : Symbol("", PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE) {}

PackageFolder::PackageFolder(StringView a_strName) : Symbol(a_strName, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE) {}

PackageFolder* PackageFolder::newPackageFolder(String a_strName)
{
    PHANTOM_ASSERT(getPackageFolder(a_strName) == nullptr);
    PackageFolder* pPF = phantom::new_<PackageFolder>(a_strName);
    pPF->rtti.instance = pPF;
    pPF->setOwner(this);
    phantom::detail::deferInstallation("phantom::lang::PackageFolder", &pPF->rtti);
    pPF->initialize();
    m_PackageFolders.push_back(pPF);
    return pPF;
}

PackageFolder::~PackageFolder() {}

void PackageFolder::terminate()
{
    for (auto& pPackage : m_Packages)
        pPackage->m_pFolder = nullptr;
    Symbol::terminate();
}

void PackageFolder::_addPackage(Package* a_pPackage)
{
    m_Packages.push_back(a_pPackage);
}

void PackageFolder::_removePackage(Package* a_pPackage)
{
    for (auto& pPackage : m_Packages)
        if (pPackage == a_pPackage)
            m_Packages.erase(&pPackage);
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
            pCurrFolder->newPackageFolder(str);
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

} // namespace lang
} // namespace phantom
