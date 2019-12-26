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
/// \brief  Represents a package.
class PHANTOM_EXPORT_PHANTOM Package : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Package);

    friend class Source;
    friend class Namespace;
    friend class Module;
    friend class Application;
    friend class phantom::detail::DynamicCppInitializerH;
    friend class PackageFolder;

public:
    static bool IsValidName(StringView a_strName);

public:
    Package(StringView a_strName);
    ~Package() override;

    void terminate();

    Package* asPackage() const override
    {
        return const_cast<Package*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source with given name.
    ///
    /// \param  a_strSourceName   The source name.
    ///
    /// \return null if it fails, else the source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package folder associated with this package
    ///
    /// \return the package folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getPackageFolder() const
    {
        return m_pFolder;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the first source owned by this package.
    ///
    /// \return An iterator pointing to the first source owned by this package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Sources const& getSources()
    {
        return m_Sources;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a source to this package. The source must not conflict with the present ones
    ///         and must not have already an owner.
    ///
    /// \param  The source to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addSource(Source* a_pSource);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the counterpart namespace of this package (the namespace with qualified name
    /// isSame to this package name).
    ///
    /// \return The counterpart namespace of this package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Namespace* getCounterpartNamespace() const
    {
        return m_pNamespace;
    }

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        return getQualifiedName(a_Buf);
    }

    void getQualifiedName(StringBuffer& a_Buf) const override;

    bool canBeUnloaded() const override;

    void getUniqueName(StringBuffer& a_Buf) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the path name of this package ('.' replaced by '/').
    ///
    /// \return .
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    String pathName() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package folders whole hierarchy of this package's folder.
    ///
    /// \param [in,out] a_Folders   [in,out] a vector containing all the path folders from
    /// parent-most to this package's folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getPackageFolders(PackageFolders& a_Folders) const;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \fn Package::sourceAdded(Source*)
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Protected Signals
    /// @{
    phantom::Signal<void(Source*)> sourceAdded;
    /// @}
    /// /////////////////////////////////////////////////////////////////////////////////////////////////
    /// \fn Package::sourceAboutToBeRemoved(Source*)
    /// /////////////////////////////////////////////////////////////////////////////////////////////////
    /// Protected Signals
    /// @{
    phantom::Signal<void(Source*)> sourceAboutToBeRemoved;
    /// @}

private:
    void           addArchivedSource(Source* a_pSource);
    void           deleteSource(Source* a_pSource);
    void           removeSource(Source* a_pSource);
    void           onElementAdded(LanguageElement* a_pElement) override;
    void           onReferencedElementRemoved(LanguageElement* a_pElement) override;
    void           onElementRemoved(LanguageElement* a_pElement) override;
    void           setNamespace(Namespace* a_pNamespace);
    hash64         computeHash() const override;
    void           onAncestorChanged(LanguageElement* a_pAncestor) override;

protected:
    Namespace*     m_pNamespace;
    PackageFolder* m_pFolder;
    Sources        m_Sources;
    Sources        m_ArchivedSources;
};

} // namespace reflection
} // namespace phantom
