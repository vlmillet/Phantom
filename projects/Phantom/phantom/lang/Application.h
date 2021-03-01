// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/MemoryContext.h>
#include <phantom/lang/Symbol.h>
#include <phantom/utils/Signal.h>
#include <phantom/utils/SmallMap.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
class ModuleRegistrationInfo;
}

namespace lang
{
class SourceFile;
class Undefineds;
class Main;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \internal
////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief  Plugin entry to store informations of plugin files.

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class  Application
///
/// \brief  Represents the application itself and allows you to manipulate it by loading/unloading
/// modules(plugins), compiling custom source code at runtime, etc...
////////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM Application : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Application);

    friend struct auto_dll_loader;
    friend class Main;
    friend class Namespace;
    friend class Package;
    friend class Module;
    friend class Plugin;
    friend class Source;
    friend class SourceFile;
    friend class phantom::detail::DynamicCppInitializerH;
    friend struct phantom::lang::BuiltInTypes;
    template<typename>
    friend class IntegralTypeT;
    template<typename>
    friend class ArithmeticTypeT;
    template<typename>
    friend class FundamentalTypeT;
    friend class MemberPointer;
    friend class Reference;
    friend class Pointer;
    friend class Enum;

public:
    typedef SmallMap<Module*, size_t>                                             LoadedLibraryModules;
    typedef SmallMap<String, SmallMap<Module*, size_t> >                          LoadedLibraries;
    typedef Delegate<Expression*(StringView a_strExp, LanguageElement* a_pScope)> CppExpressionParser;
    typedef Delegate<Variant(Expression*)>                                        CppExpressionEvaluator;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  returns a singleton representing the current running application.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    static Application* Get();

public:
    Application();
    ~Application() override;

    void initialize();
    void terminate();

    Source* getDefaultSource() const { return m_pDefaultSource; }

    void                setCppExpressionParser(CppExpressionParser a_Parser);
    CppExpressionParser getCppExpressionParser();

    void                   setCppExpressionEvaluator(CppExpressionEvaluator a_Parser);
    CppExpressionEvaluator getCppExpressionEvaluator();

    Expression* cppExpression(StringView a_strExp, LanguageElement* a_pScope) const;
    Variant     evalExpression(Expression* a_pExp) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find a c++ symbol with its full qualified name
    /// \return true if it succeeds to load the file, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool findCppSymbols(StringView a_Text, Symbols& a_Symbols, StringBuffer* a_pLastError = nullptr);
    bool findCppSymbols(StringView a_Text, Symbols& a_Symbols, LanguageElement* a_pScope,
                        StringBuffer* a_pLastError = nullptr);

    Symbol*   findCppSymbol(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Type*     findCppType(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Template* findCppTemplate(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Template* findCppTemplate(StringView a_Text, StringBuffer* a_pLastError = nullptr);
    Class*    findCppClass(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Symbol*   findCppSymbol(StringView a_Text, StringBuffer* a_pLastError = nullptr);
    Type*     findCppType(StringView a_Text, StringBuffer* a_pLastError = nullptr);
    Class*    findCppClass(StringView a_Text, StringBuffer* a_pLastError = nullptr);

    Symbol*   findCppSymbolCached(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Type*     findCppTypeCached(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Template* findCppTemplateCached(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Template* findCppTemplateCached(StringView a_Text, StringBuffer* a_pLastError = nullptr);
    Class*    findCppClassCached(StringView a_Text, LanguageElement* a_pScope, StringBuffer* a_pLastError = nullptr);
    Symbol*   findCppSymbolCached(StringView a_Text, StringBuffer* a_pLastError = nullptr);
    Type*     findCppTypeCached(StringView a_Text, StringBuffer* a_pLastError = nullptr);
    Class*    findCppClassCached(StringView a_Text, StringBuffer* a_pLastError = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the internal module representing the main application (on windows the .exe
    /// content, on mac the .dmg, etc...).
    ///
    /// \return The main module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getMainModule() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the a module or create a new one
    ///
    /// \return The found or new module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getOrCreateModule(StringView a_name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets loaded native modules' count.
    ///
    /// \return The count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Modules getNativeModules() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query the loaded module associated with the given shared library path (a .dll on
    /// windows for example)
    ///
    /// \param  a_strPath   path of the library file.
    ///
    /// \return the matching module if any, else nullptr.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getSharedLibraryModule(StringView a_strPath) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Try to retrieve the module associated with the given call stack context.
    ///
    /// \param [in,out] ebp If non-null, the EBP (base pointer) register value.
    /// \param  stackLevel  (optional) the stack level.
    /// \param  eip         (optional) [in,out] The EIP (instruction pointer) register value.
    ///
    /// \return null if it fails to find a corresponding module, else the corresponding module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* nativeModuleFromStackContext(const byte* ebp, uint stackLevel = 1, byte* epc = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Try to retrieve the module associated with the given program counter.
    ///
    /// \param [in,out] pc  The Program Counter.
    ///
    /// \return null if it fails to find a corresponding module, else the corresponding module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* nativeModuleFromProgramCounter(const byte* pc);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default module path where translated modules will be exported as
    /// new projects.
    ///
    /// \param  a_strPath   The default export path.
    ///
    /// \return true if it succeeds to set and create the export path if needed, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool setDefaultExportPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default export path where translated modules will be exported as
    /// new projects.
    ///
    /// \return The default export path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getDefaultExportPath() const { return m_DefaultExportPath; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default plugin path where runtime plugin files will be added/removed.
    ///
    /// \param  a_strPath   The default plugin path.
    ///
    /// \return true if it succeeds to set and create the plugin path if needed, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool setDefaultPluginPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default plugin path where runtime plugin files will be added/removed.
    ///
    /// \return The default plugin path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getDefaultPluginPath() const { return m_DefaultPluginPath; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a plugin path where to look for plugins to load/unload.
    ///
    /// \param  a_strPath   The plugin path to add.
    ///
    /// \return true if succeeds to add, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addPluginPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a plugin path.
    ///
    /// \param  a_strPath   The plugin path to remove.
    ///
    /// \return true if succeeds to remove, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool removePluginPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given plugin path is already defined.
    ///
    /// \param  a_strPath   The plugin path.
    ///
    /// \return true if already defined, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasPluginPath(StringView a_strPath) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default binary path where runtime binary files will be added/removed.
    ///
    /// \param  a_strPath   The default binary path.
    ///
    /// \return true if it succeeds to set and create the binary path if needed, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool setDefaultBinaryPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default binary path where runtime binary files will be added/removed.
    ///
    /// \return The default binary path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getDefaultBinaryPath() const { return m_DefaultBinaryPath; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a binary path where to look for binaries to load/unload.
    ///
    /// \param  a_strPath   The binary path to add.
    ///
    /// \return true if succeeds to add, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addBinaryPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a binary path.
    ///
    /// \param  a_strPath   The binary path to remove.
    ///
    /// \return true if succeeds to remove, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool removeBinaryPath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the given binary path is already defined.
    ///
    /// \param  a_strPath   The binary path.
    ///
    /// \return true if already defined, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasBinaryPath(StringView a_strPath) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a built-in type by its name.
    ///
    /// \param  a_strName   The built-in type name (ex : int, float...).
    ///
    /// \return null if it fails, else the built in type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getBuiltInType(StringView a_strDecoratedName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the list of c++ built-in types.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Types const& getBuiltInTypes() const { return m_BuiltInTypes; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a module from its unique name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if no module match the given name, else the module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getModule(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the modules list containing every symbol lang for each loaded library.
    ///
    /// \return The modules list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Modules const& getModules() const { return m_Modules; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the modules that were loaded automatically at startup by static linking.
    ///         These modules have a lifetime equals to the application lifetime (except if the user
    ///         manually unload them with OS functions, which leads to undefined behavior.
    ///
    /// \return The startup modules list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Modules const& getStartupModules() const { return m_StartupModules; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the module matching the given handle.
    ///
    /// \param  a_ModuleHandle   The module handle.
    ///
    /// \return null if no module match the given handle, else the module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getModuleByHandle(size_t a_ModuleHandle) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a plugin from its name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if no plugin match the given name, else the plugin.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Plugin* getPlugin(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the plugins available for load/unload.
    ///
    /// \return the list of plugins.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Plugins const& getPlugins() const { return m_Plugins; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a plugin to the application, creating the plugin file if it doesn't exist or
    ///         loading it otherwise.
    ///
    /// \param  a_pPlugin   The plugin instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addPlugin(Plugin* a_pPlugin);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a plugin from the application, creating the plugin file if it doesn't exist
    /// or
    ///         loading it otherwise.
    ///
    /// \param  a_pPlugin   The plugin instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removePlugin(Plugin* a_pPlugin);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a module (do not add it yet)
    ///
    /// \param  a_strName   The module name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* newModule(StringView a_strName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a module (do not add it yet)
    ///
    /// \param  a_strName   The module name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void deleteModule(Module* a_pMod);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Registers a module to the application
    ///
    /// \param  a_pModule   The module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addModule(Module* a_pModule);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Unregisters a module from the application
    ///
    /// \param  a_pModule   The module.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeModule(Module* a_pModule);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Collect every source of this application
    ///
    /// \param  a_Sources   The collected sources.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getSources(Sources& a_Sources) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source repository path (source repository is where you can manipulate your
    /// source files, not to be confound with the module path's source folder where only valid
    /// sources are stored for backups and restorations of the module).
    ///
    /// \return The source repository path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Strings& getSourcePaths() const { return m_SourcePaths; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default source path where new source file will be created
    ///
    /// \param  a_strPath   The default source path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getDefaultSourcePath() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the default source path where new source file will be created
    ///
    /// \param  a_strPath   The default source path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool setDefaultSourcePath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a source path (source path is where you can manipulate your source files, not
    /// to be confound with the module path's source folder where only valid '.ili' sources are
    /// stored for backups and restorations of the module).
    ///
    /// \param  a_strPath   The source path to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addSourcePath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a source path
    ///
    /// \param  a_strPath   The source path to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool removeSourcePath(StringView a_strPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the root package folder.
    ///
    /// \return null if it fails, else the root package folder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* rootPackageFolder() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a source from unique name 'package.Source'.
    ///
    /// \param  a_strUniqueName The source name.
    ///
    /// \return null if no source found, else the source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getSource(StringView a_strUniqueName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a package folder from unique name 'package0.package1...'.
    ///
    /// \param  a_strUniqueName The package name.
    ///
    /// \return null if no package found, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getPackageFolder(StringView a_strUniqueName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an existing or create a new package folder from unique name
    /// 'package0.package1...'.
    ///
    /// \param  a_strUniqueName The package name.
    ///
    /// \return null if no package found, else the package.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PackageFolder* getOrCreatePackageFolder(StringView a_strUniqueName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find the plugin path containing the plugin with given name.
    ///
    /// \param  a_strModuleName The plugin name.
    ///
    /// \return If plugin has not been found in any plugin path, returns an empty path String, else
    ///         the found plugin path.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    String findPluginPath(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find the plugins in the given path.
    ///
    /// \param  a_strPath The path to look at.
    /// \param  a_PluginNames The names of the found plugins.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findPlugins(StringView a_strPath, Strings& a_PluginNames) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the plugin paths (index 0).
    ///
    /// \return the plugin paths.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Strings const& getPluginPaths() const { return m_PluginPaths; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for the type matching the given hash.
    ///
    /// \param  a_Hash  The type's hash.
    ///
    /// \return null if no type found matching the given hash, else the type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* findType(hash64 a_Hash) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for the symbol matching the given unique name (strict match, no space
    ///         skipping rule).
    ///
    /// \param  a_strUniqueName The symbol's unique name.
    /// \param  a_pScope        (optional) [in,out] If non-null, the scope where to start the search
    /// from.
    ///
    /// \return null if no symbol found matching the given unique name, else the symbol.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Symbol* findSymbol(StringView a_strUniqueName, LanguageElement* a_pScope = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for the type matching the given unique name (strict match, no space
    /// skipping
    ///         rule).
    ///
    /// \param  a_strUniqueName The type's unique name.
    /// \param  a_pScope        (optional) [in,out] If non-null, the scope where to start the search
    /// from.
    ///
    /// \return null if no type found matching the given unique name, else the type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* findType(StringView a_strUniqueName, LanguageElement* a_pScope = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Find all the classes derived from the given optional base class.
    ///
    /// \param [in,out] a_Classes       [in,out] The classes found are stored in this container.
    /// \param  a_pBaseClass            (optional) The base class to filter the result. If null,
    /// every class in the application will be
    ///                                 collected.
    /// \param  a_bDefaultInstanciable  (optional) If true, every collected class must be default
    /// instanciable (not abstract + default constructible).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findClasses(Classes& a_Classes, lang::Class* a_pBaseClass = nullptr,
                     bool a_bDefaultInstanciable = false) const;

    Undefineds* getUndefineds(StringView a_TypeName) const;

    Undefineds* undefineds(StringView a_TypeName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'nullptr' constant representation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constant* getNullptr() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'auto' type representation.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getAuto() const;

    void getUniqueName(StringBuffer& a_Buf) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Returns a memory context you can use to bind memory with the Application and its
    /// life duration.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemoryContext& getMemoryContext() { return m_MemoryContext; }

    CustomAllocator const* getAllocator() const override { return &CustomAllocator::CurrentOrDefault(); }

public:
    phantom::Signal<void(StringView)> pluginPathAdded;
    phantom::Signal<void(StringView)> pluginPathRemoved;
    phantom::Signal<void(StringView)> binaryPathAdded;
    phantom::Signal<void(StringView)> binaryPathRemoved;
    phantom::Signal<void(Module*)>    moduleAdded;
    phantom::Signal<void(Module*)>    moduleDiscarded;
    phantom::Signal<void(Module*)>    moduleAboutToBeRemoved;
    phantom::Signal<void(Module*)>    moduleUnloaded;
    phantom::Signal<void(Plugin*)>    pluginAdded;
    phantom::Signal<void(Plugin*)>    pluginAboutToBeRemoved;
    phantom::Signal<void(Plugin*)>    pluginLoaded;
    phantom::Signal<void(Plugin*)>    pluginLoadingFailed;
    phantom::Signal<void(Plugin*)>    pluginAboutToBeUnloaded;
    phantom::Signal<void(Source*)>    sourceAdded;
    phantom::Signal<void(Source*)>    sourceAboutToBeRemoved;
    phantom::Signal<void(Package*)>   packageAdded;
    phantom::Signal<void(Package*)>   packageAboutToBeRemoved;
    phantom::Signal<void(Source*)>    sourceDeleted;

private:
    void _registerBuiltInTypes();
    void _addBuiltInType(Type* a_pType);
    void _removeBuiltInType(Type* a_pType);
    void _registerSourceFile(SourceFile* a_pSourceFile) { m_SourceFiles.push_back(a_pSourceFile); }
    void _unregisterSourceFile(SourceFile* a_pSourceFile)
    {
        m_SourceFiles.erase(std::find(m_SourceFiles.begin(), m_SourceFiles.end(), a_pSourceFile));
    }
    bool    _findSymbols(StringView a_strUniqueName, Symbols& a_OutSymbols) const;
    Symbol* _findSymbol(const Strings& words, const Types* a_pFunctionSignature, Modifiers a_RefQualifiers,
                        LanguageElement* a_pScope = nullptr) const;
    void    _createNativeModule(ModuleRegistrationInfo* info);
    void    _uninstallNativeModule(Module* a_pModule);

    void _loadMain(size_t a_MainHandle, StringView a_strModuleName, StringView a_strFileName,
                   StringView a_strSourceFile, uint a_uiFlags);
    void _unloadMain();

    void _addModule(Module* a_pModule);
    void _removeModule(Module* a_pModule);
    void _moduleAdded(Module* a_pModule);
    void _moduleDiscarded(Module* a_pModule);
    void _moduleAboutToBeRemoved(Module* a_pModule);

    void _sourceAdded(Source* a_pSource) { PHANTOM_EMIT sourceAdded(a_pSource); }
    void _sourceAboutToBeRemoved(Source* a_pSource) { PHANTOM_EMIT sourceAboutToBeRemoved(a_pSource); }

    void _prefetchPlugins();
    void _prefetchPlugins(StringView a_strPath);
    void _pluginLoaded(Plugin* p) { PHANTOM_EMIT pluginLoaded(p); }
    void _pluginLoadingFailed(Plugin* p) { PHANTOM_EMIT pluginLoadingFailed(p); }
    void _pluginAboutToBeUnloaded(Plugin* p) { PHANTOM_EMIT pluginAboutToBeUnloaded(p); }
    void _NewH(LanguageElement* a_pOwner, LanguageElement* a_pElem, Class* a_pClass, void* a_pMD);
    void _NewDeferredH(LanguageElement* a_pOwner, LanguageElement* a_pElem, Class* a_pClass, void* a_pMD,
                       StringView a_QN);

protected:
    hash64 computeHash() const override { return 0; }

private: // to ensure they won't be accessible
    using LanguageElement::New;
    using LanguageElement::NewDeferred;
    using LanguageElement::NewMeta;
    using LanguageElement::new_;
    using LanguageElement::delete_;

private:
    typedef SmallMap<String, Undefineds*> UndefinedsMap;
    String                                m_DefaultPluginPath;
    String                                m_DefaultBinaryPath;
    String                                m_DefaultExportPath;
    Strings                               m_PluginPaths;
    Strings                               m_BinaryPaths;
    Strings                               m_SourcePaths;
    SourceFiles                           m_SourceFiles;
    Plugins                               m_Plugins;
    Modules                               m_NativeModules;
    Modules                               m_Modules;
    Modules                               m_StartupModules;
    Module*                               m_pMainModule;
    Modules                               m_CurrentlyLoadingModules;
    size_t                                m_OperationCounter;
    LanguageElements                      m_ReplacementOldElements;
    Types                                 m_BuiltInTypes;
    PackageFolder*                        m_pRootPackageFolder;
    mutable Constant*                     m_pNullptr;
    MemoryContext                         m_MemoryContext;
    UndefinedsMap                         m_Undefineds;
    Source*                               m_pDefaultSource = nullptr;
};

} // namespace lang
} // namespace phantom
