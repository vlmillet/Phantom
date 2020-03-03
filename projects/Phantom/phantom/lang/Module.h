// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/MemoryContext.h>
#include <phantom/lang/Symbol.h>
#include <phantom/utils/Signal.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallSet.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Module : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Module);

    friend class phantom::lang::Application;
    friend class phantom::detail::DynamicCppInitializerH;
    friend class ModuleRegistrationInfo;
    friend class Source;
    friend class Package;
    friend class Plugin;
    friend class LanguageElement;
    friend class Type;

public:
    typedef SmallMap<Module*, size_t> Dependencies;

public:
    Module(StringView a_strName, uint a_uiFlags = 0);
    ~Module() override;

    void terminate();

    Module* asModule() const override { return (Module*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this module is a plugin
    ///
    /// \return true if a plugin.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isPlugin() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this module has an associated plugin.
    ///
    /// \return true if a plugin.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Plugin* getPlugin() const { return m_pPlugin; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the dynamic library filename associated with this module.
    ///
    /// \return empty if no file associated, else the full path name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getLibraryFullName() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the full path of the .cpp file containing the plugin or main declaration.
    ///
    /// \return empty if no file associated, else the full path name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getDeclarationCppFullName() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the memory start address of this module.
    ///
    /// \return null if it fails, else the memory start address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    byte* getMemoryStart() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets native handle or 0 if your OS don't have such a value.
    ///
    /// \return The native handle (or 0 if your OS don't have such a value)..
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getHandle() const { return (size_t)m_pBaseAddress; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets base address of this module, or null if not a native or jit module.
    ///
    /// \return The platform handle (or 0x0 if your OS don't have such a value)..
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getBaseAddress() const { return m_pBaseAddress; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets runtime JIT module base address.
    ///
    /// \return The platform handle (or 0x0 if your OS don't have such a value)..
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setBaseAddress(void* a_pBaseAddress)
    {
        PHANTOM_ASSERT(!isNative());
        m_pBaseAddress = a_pBaseAddress;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the packages owned by this module.
    ///
    /// \return the package list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Packages const& getPackages() const { return m_Packages; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this module can be unloaded.
    ///
    /// \return true if it can be unloaded, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool canBeUnloaded() const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default package of this module.
    ///
    /// \return null if it fails, else the default package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Package* getDefaultPackage() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a package matching the given name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if no package found matching the given name, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Package* getPackage(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds the given package to this module.
    ///
    /// \param [in,out] a_pPackage The package to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addPackage(Package* a_pPackage);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes the given package from this module.
    ///
    /// \param [in,out] a_pPackage The package to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removePackage(Package* a_pPackage);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the dependencies of this module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Modules const& getDependencies() const { return m_Dependencies; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add a dependency to this module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addDependency(Module* a_pModule);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Remove a dependency from this module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeDependency(Module* a_pModule);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Check if this module depends on the given module (directly).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDependency(Module* a_pModule) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Check if this module depends on the given module (recursively).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDependencyCascade(Module* a_pModule) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches all the sources of this module into a vector container.
    ///
    /// \param [in,out] a_Sources   [in,out] The fetched sources.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getSources(Sources& a_Sources) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Fetches all the dependencies of this module.
    ///
    /// \param [in,out] a_Modules   [in,out] The fetched dependencies.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void fetchDependencies(Modules& a_Modules) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find all the classes in this module derived from the given base class into a vector
    ///         container.
    ///
    /// \param [in,out] a_Classes       [in,out] The found classes.
    /// \param  a_pBaseClass            (optional) If non-null, the base class used to filter the
    ///                                 searched classes. If null, every class in the module will be
    ///                                 collected.
    /// \param  a_bDefaultInstanciable  (optional) check if the classes must be default instanciable
    /// (not abstract + default constructible).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findClasses(Classes& a_Classes, lang::Class* a_pBaseClass = nullptr,
                     bool a_bDefaultInstanciable = false) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find all the functions in this module matching the given optional name and/or
    /// signature
    ///
    /// \param [in,out] a_Classes   [in,out] The found functions.
    /// \param  a_pName             (optional) If non-empty, the name of the functions to look for.
    /// \param  a_pParameterTypes   (optional) If non-null, the parameter types of the functions to
    /// look for. \param  a_pReturnType       (optional) If non-null, the return type of the
    /// functions to look for.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findFunctions(Functions& a_Functions, StringView a_Name = "", const Types* a_pParameterTypes = nullptr,
                       Type* a_pReturnType = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the anonymous source associated with this module.
    ///
    /// \return The anonymous source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getAnonymousSource() const { return m_pAnonymousSource; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Returns a memory context you can use to bind memory with this Module and its life
    /// duration.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemoryContext& getMemoryContext() { return m_MemoryContext; }

    Type* findType(hash64 a_Hash) const;

    Type* findUsableType(hash64 a_Hash) const;

    typedef void (*FuncT)();
    void  setOnLoadFunc(FuncT func) { m_OnLoad = func; }
    void  setOnUnloadFunc(FuncT func) { m_OnUnload = func; }
    FuncT getOnLoadFunc() const { return m_OnLoad; }
    FuncT getOnUnloadFunc() const { return m_OnUnload; }

    bool isMarkedOutdated() const { return m_bOutdated; }
    void markOutdated() { m_bOutdated = true; }
    void markUpToDate() { m_bOutdated = false; }

public:
    phantom::Signal<void(Package*)> packageAdded;
    phantom::Signal<void(Package*)> packageAboutToBeRemoved;
    phantom::Signal<void()>         changed;

protected:
    hash64 computeHash() const override;

private:
    void onElementAdded(LanguageElement* a_pElement) override;
    void onElementRemoved(LanguageElement* a_pElement) override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a module from a name and a source path.
    ///
    /// \param  a_strName       The name.
    /// \param  a_strSourcePath Full pathname of the sources.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module(size_t a_NativeHandle, StringView a_strName, StringView a_LibFullName, StringView a_DeclarationCppFullName,
           uint a_uiFlags = 0);

    void _registerType(hash64 a_Hash, Type* a_pType);
    void _unregisterType(hash64 a_Hash, Type* a_pType);

private:
    Packages                m_Packages;
    void*                   m_pBaseAddress = nullptr;
    StringBuffer            m_LibraryFullName;
    StringBuffer            m_DeclarationCppFullName;
    Modules                 m_Dependencies;
    Package*                m_pAnonymousPackage = nullptr;
    Source*                 m_pAnonymousSource = nullptr;
    Plugin*                 m_pPlugin = nullptr;
    SmallMap<hash64, Type*> m_HashToTypeMap;
    FuncT                   m_OnLoad = nullptr;
    FuncT                   m_OnUnload = nullptr;
    MemoryContext           m_MemoryContext;
    bool                    m_bOutdated = false;
};

struct ModuleReverseDependencySorter
{
    bool operator()(Module* m0, Module* m1) const
    {
        if (m0 == m1)
            return m0 < m1;
        if (m0->hasDependencyCascade(m1))
            return true;
        if (m1->hasDependencyCascade(m0))
            return false;
        return m0 < m1;
    }
};

struct ModuleDependencySorter
{
    bool operator()(Module* m0, Module* m1) const
    {
        if (m0 == m1)
            return m0 < m1;
        if (m0->hasDependencyCascade(m1))
            return false;
        if (m1->hasDependencyCascade(m0))
            return true;
        return m0 < m1;
    }
};

} // namespace lang
} // namespace phantom
