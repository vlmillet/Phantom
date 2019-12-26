// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/Signal.h>
#include <phantom/reflection/Symbol.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace reflection
{
/// \brief  Represents a package folder.
class PHANTOM_EXPORT_PHANTOM PackageFolder : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(PackageFolder);

    friend class Application;
    friend class Package;
    friend class phantom::Phantom;

protected:
    static PackageFolder* Create();

    /// \internal
    PackageFolder();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor.
    /// \internal
    ///
    /// \param  a_strName                   The name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder(StringView a_strName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a sub package folder to this package folder.
    ///
    /// \param  a_pFolder   The package folder to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addPackageFolder(PackageFolder* a_pFolder)
    {
        addSymbol(a_pFolder);
    }

public:
    ~PackageFolder() override;

    void terminate();

    PackageFolder* asPackageFolder() const override
    {
        return const_cast<PackageFolder*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parent folder of this package folder.
    ///
    /// \return null if no parent folder (root package folder), else the parent folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getParentFolder() const
    {
        return getOwner() ? getOwner()->asPackageFolder() : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package associated with this package folder.
    ///
    /// \return null if no package is associated with this package folder, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Packages const& getPackages() const
    {
        return m_Packages;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package associated with this package folder.
    ///
    /// \return null if no package is associated with this package folder, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Package* getPackage(Module* a_pModule) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the first package folder (index 0).
    ///
    /// \return An iterator pointing to the first package folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolders::const_iterator beginPackageFolders() const
    {
        return m_PackageFolders.begin();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the end of package folders (index last+1).
    ///
    /// \return An iterator pointing to the end of package folders.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolders::const_iterator endPackageFolders() const
    {
        return m_PackageFolders.end();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package folders whole hierarchy of this folder.
    ///
    /// \param [in,out] a_Folders   [in,out] a vector containing all the path folders from
    /// parent-most to this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void                  getPackageFolders(PackageFolders& a_Folders) const;
    PackageFolders const& getPackageFolders() const
    {
        return m_PackageFolders;
    }

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

    void getQualifiedName(StringBuffer& a_Buf) const override
    {
        getUniqueName(a_Buf);
    }
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        getUniqueName(a_Buf);
    }

public:
    phantom::Signal<void(PackageFolder*)> packageFolderAdded;
    phantom::Signal<void(PackageFolder*)> packageFolderAboutToBeRemoved;

private:
    void onElementAdded(LanguageElement* a_pElement) override;
    void onElementRemoved(LanguageElement* a_pElement) override;
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;
    void _addPackage(Package* a_pPackage);
    void _removePackage(Package* a_pPackage);

private:
    Packages       m_Packages;
    PackageFolders m_PackageFolders;
};

} // namespace reflection
} // namespace phantom
