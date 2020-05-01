// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
#include <phantom/utils/Signal.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace lang
{
/// \brief  Represents a package folder.
class PHANTOM_EXPORT_PHANTOM PackageFolder : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(PackageFolder);

    friend class Application;
    friend class Package;
    friend class phantom::Phantom;

public:
    PackageFolder();
    PackageFolder(StringView a_strName);
    ~PackageFolder() override;

    void terminate();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a sub package folder to this package folder.
    ///
    /// \param  a_pFolder   The package folder to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* newPackageFolder(String a_strName);

    PackageFolder* asPackageFolder() const override { return const_cast<PackageFolder*>(this); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parent folder of this package folder.
    ///
    /// \return null if no parent folder (root package folder), else the parent folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getParentFolder() const { return getOwner() ? getOwner()->asPackageFolder() : nullptr; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package associated with this package folder.
    ///
    /// \return null if no package is associated with this package folder, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Packages const& getPackages() const { return m_Packages; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package associated with this package folder.
    ///
    /// \return null if no package is associated with this package folder, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Package* getPackage(Module* a_pModule) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package folders whole hierarchy of this folder.
    ///
    /// \param [in,out] a_Folders   [in,out] a vector containing all the path folders from
    /// parent-most to this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                  getPackageFolders(PackageFolders& a_Folders) const;
    PackageFolders const& getPackageFolders() const { return m_PackageFolders; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package folder matching the given "folder1.folder2...folderN" string.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getPackageFolder(StringView a_strUniqueName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets existing or create new package folder matching the given
    /// "folder1.folder2...folderN" string.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getOrCreatePackageFolder(StringView a_strUniqueName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets source inside the packages associated with this package folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource(StringView a_strName) const;

    // reimplemented

    void getQualifiedName(StringBuffer& a_Buf) const override { getUniqueName(a_Buf); }
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { getUniqueName(a_Buf); }

    CustomAllocator const* getAllocator() const override { return &CustomAllocator::CurrentOrDefault(); }

public:
    phantom::Signal<void(PackageFolder*)> packageFolderAdded;
    phantom::Signal<void(PackageFolder*)> packageFolderAboutToBeRemoved;

private:
    void _addPackage(Package* a_pPackage);
    void _removePackage(Package* a_pPackage);
    using LanguageElement::New;
    using LanguageElement::NewDeferred;
    using LanguageElement::NewMeta;
    using LanguageElement::new_;
    using LanguageElement::delete_;

private:
    Packages       m_Packages;
    PackageFolders m_PackageFolders;
};

} // namespace lang
} // namespace phantom
