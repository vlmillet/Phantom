// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Scope.h"
#include "Symbol.h"

#include <phantom/traits/TypeIdentity.h>
#include <phantom/utils/ForwardHeap.h>
#include <phantom/utils/Signal.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
class Phantom;
}

namespace phantom
{
namespace lang
{
class SourceStream;

template<class T>
struct ObjectDtor
{
    static void apply(void* a_pBase) { Delete<T>(reinterpret_cast)<T*>(a_pBase); }
};

/// \brief  Represents a source code (not a source file, see phantom::lang::SourceStream for
/// this).
class PHANTOM_EXPORT_PHANTOM Source : public Symbol, public Scope
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Source);

    friend class LanguageElement;
    friend class Application;
    friend class Module;
    friend class Package;
    friend class Plugin;
    friend class Parser;
    friend class Translator;
    friend class phantom::detail::DynamicCppInitializerH;

public:
    struct Import
    {
        Symbol* symbol;
        bool    isPublic;
        bool    isStatic;
        Alias*  alias;
    };
    typedef SmallVector<Import> Imports;

public:
    enum
    {
        InvalidVersionNumber = -1
    };

public:
    Source(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    /// \brief  Destructor.
    ~Source() override;

    void initialize();
    void terminate() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package of this source.
    ///
    /// \return The package of this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Module* getModule() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the package of this source.
    ///
    /// \return The package of this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Package* getPackage() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a valid version of this source matching the given id.
    ///
    /// \param  a_uiVersionId   The version id.
    ///
    /// \return null if it fails, else a valid version.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getNativeArchive() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the source file associated with this source.
    ///
    /// \return null if no source file associated with this source, else the source file.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SourceStream* getSourceStream() const { return m_pSourceStream; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get or create a function pointer type.
    ///
    /// \param [in,out] a_pReturnType   The function return type.
    /// \param  a_eABI                  The function ABI.
    /// \param  a_ParameterTypes        The function parameter types.
    /// \param  a_Qualifiers            (optional) The function qualifiers.
    ///
    /// \return The function type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType* functionType(Type* a_pReturnType, TypesView a_ParameterTypes, Modifiers a_Modifiers = 0,
                               uint a_Flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get or create a function pointer type.
    ///
    /// \param [in,out] a_pReturnType   The function return type.
    /// \param  a_eABI                  The function ABI.
    /// \param  a_ParameterTypes        The function parameter types.
    /// \param  a_Qualifiers            (optional) The function qualifiers.
    ///
    /// \return The function pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionPointer* functionPointerType(Type* a_pReturnType, ABI a_eABI, TypesView a_ParameterTypes,
                                         Modifiers a_Qualifiers = 0, uint a_Flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get or create a function pointer type.
    ///
    /// \param  a_pFunctionType         The function type.
    /// \param  a_eABI                  The function ABI.
    ///
    /// \return The function pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionPointer* functionPointerType(FunctionType* a_pFunctionType, ABI a_eABI, Modifiers a_Qualifiers = 0,
                                         uint a_Flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get or create an Initializer list type, used for conversion/overload purpose (use
    /// std::initializer_list type to use the effective one).
    ///
    /// \param  a_pFunctionType         The function type.
    /// \param  a_eABI                  The function ABI.
    ///
    /// \return The function pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    InitializerListType* initializerListType(TypesView a_Types);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Requests a member function pointer type associated with this class type.
    ///
    /// \param [in,out] a_pReturnType   The function pointer return type.
    /// \param  a_ParameterTypes        The parameters' types.
    /// \param  a_RefQualifiers         The qualifiers
    /// (PHANTOM_R_CONST|PHANTOM_R_LVALUEREF|PHANTOM_R_RVALUEREF)
    ///
    /// \return null if it fails, else the member function pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MethodPointer* methodPointerType(ClassType* a_pObjectType, Type* a_pReturnType, TypesView a_ParameterTypes,
                                     Modifiers a_RefQualifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Requests a data member pointer type associated with this class type.
    ///
    /// \param [in,out] a_pValueType    The data member value type.
    ///
    /// \return null if it fails, else the data member pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldPointer* fieldPointerType(ClassType* a_pObjectType, Type* a_pValueType, Modifiers a_Modifiers = 0,
                                   uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the stream from which we will collect the source code.
    ///
    /// \param a_pStream    The stream (an abstract class you can reimplement to provide your own
    /// std::istream specialization)
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setSourceStream(SourceStream* a_pStream);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the imports list.
    ///
    /// \return An iterator pointing to the end of imports in this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Imports const& getImports() const { return m_Imports; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all the imported source (recursively).
    ///
    /// \param a_Importeds The container which receives the imported list
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getImported(Symbols& a_Imports) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a list of the sources importing this one.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Sources const& getImportings() const { return m_Importings; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an imported symbol to this source.
    ///
    /// \param [in,out] a_pSource   The imported symbol.
    /// \param  a_bStatic           (optional) is the import 'static' (which means you need to use
    /// the full qualified name of a symbol to access it). \param  a_bPublic           (optional) is
    /// the import 'public' (which means import is 'open' to importing sources and will be
    /// accessible to them).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addImport(Symbol* a_pSource, bool a_bStatic = false, bool a_bPublic = false);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an imported source with given name to this source.
    ///
    /// \param  a_strName   The imported source name.
    /// \param  a_bStatic   (optional) is the import 'static' (which means you need to use the full
    /// qualified name of a symbol to access it). \param  a_bPublic   (optional) is the import
    /// 'public' (which means import is 'open' to importing sources and will be accessible to them).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addImport(StringView a_strName, bool a_bStatic = false, bool a_bPublic = false);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this source has the given described imported source with given modifiers
    /// and
    ///         flags.
    ///
    /// \param [in,out] a_pSource   the imported source.
    /// \param  a_Modifiers         (optional) the import modifiers.
    /// \param  a_uiFlags           (optional) the import flags.
    ///
    /// \return true if import, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasImport(Symbol* a_pSource) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this source has the given imported source (imported directly or imported by
    /// an import, etc...)
    ///
    /// \param [in,out] a_pSource   the candidate imported source.
    ///
    /// \return true if imported from the given source, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasImported(Symbol* a_pSource) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if we can import the given source with given modifiers and flags.
    ///
    /// \param [in,out] a_pSource   The source to import.
    /// \param  a_Modifiers         (optional) the modifiers.
    /// \param  a_uiFlags           (optional) the flags.
    ///
    /// \return true if we can import the source without name collisions, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool canImport(Symbol* a_pSymbol, Access a_eAccess = Access::Public, Modifiers a_Modifiers = 0, uint a_uiFlags = 0,
                   SmallMap<Symbol*, Symbols>* a_pCollidingSymbols = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the dependencies list.
    ///
    /// \return An iterator pointing to the end of dependencies in this source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Sources& getDependencies() const { return m_Dependencies; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this source has a given dependency on the given source (real definition
    /// dependency, not declaration only => equivalent of including a file in the .h in C++)
    ///
    /// \param [in,out] a_pSource   the candidate dependency source.
    ///
    /// \return true if dependant from the given source, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDependency(Source* a_pSource) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this source has a given dependency on the given source recursively across
    /// all dependencies (real definition dependency, not declaration only => equivalent of
    /// including a file in the .h in C++)
    ///
    /// \param [in,out] a_pSource   the candidate dependency source.
    ///
    /// \return true if dependant from the given source, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDependencyCascade(Source* a_pSource) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add a dependency on the given source (real definition dependency, not declaration
    /// only => equivalent of including a file in the .h in C++)
    ///
    /// \param [in,out] a_pSource   the dependency source to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addDependency(Source* a_pSource);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Add a dependency on the given source (real definition dependency, not declaration
    /// only => equivalent of including a file in the .h in C++)
    ///
    /// \param [in,out] a_SourceName   the name of the dependency source to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool addDependency(StringView a_strName);

    Sources const& getDependings() const { return m_Dependings; }

    Source* asSource() const override { return (Source*)this; }

    Scope* asScope() const override { return (Scope*)this; }

    bool canBeUnloaded() const override;

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override { return getQualifiedName(a_Buf); }
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

    using LanguageElement::getQualifiedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getUniqueName;

    Source* getCodeLocationSource() const override;

    // this must be used inside containers holded by any element (so ensure they will share the same memory context)
    CustomAllocator const* getAllocator() const override { return &m_CustomAlloc; }

protected:
    void   onScopeSymbolAdded(Symbol* a_pSymbol) override;
    hash64 computeHash() const override;

private:
    bool _hasImported(Symbol* a_pSymbol, SmallSet<const Source*>& treated) const;
    void _addImporting(Source*);
    void _removeImporting(Source*);
    void _addDepending(Source*);
    void _removeDepending(Source*);
    bool _hasDependencyCascade(Source* a_pSource, SmallSet<Source*>& treated) const;
    void _NewH(LanguageElement* a_pOwner, LanguageElement* a_pElem, Class* a_pClass, void* a_pMD);
    void _NewDeferredH(LanguageElement* a_pOwner, LanguageElement* a_pElem, Class* a_pClass, void* a_pMD,
                       StringView a_QN);

    template<typename T>
    static void _AssertSpecialSymbols()
    {
        // special symbols
        PHANTOM_STATIC_ASSERT(!(std::is_same<Module, T>::value), "use Application::newModule() instead");
        PHANTOM_STATIC_ASSERT(!(std::is_same<Package, T>::value), "use Module::newPackage() instead");
        PHANTOM_STATIC_ASSERT(!(std::is_same<Source, T>::value), "use Package::newSource() instead");
        PHANTOM_STATIC_ASSERT(!(std::is_same<PackageFolder, T>::value),
                              "use PackageFolder::newPackageFolder() instead");
    }

    template<typename T, class... Args>
    T* _NewMeta(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        _NewH(a_Owner.this_, ptr, T::MetaClass(), ptr);
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _New(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        _NewH(a_Owner.this_, ptr, PHANTOM_CLASSOF(T), ptr);
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _NewDeferred(LanguageElement::Owner a_Owner, Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        _NewDeferredH(a_Owner.this_, ptr, PHANTOM_CLASSOF(T), ptr,
                      lang::TypeInfosOf<T>::object().qualifiedDecoratedName());
        ptr->initialize();
        return ptr;
    }

    template<class T, class... Args>
    T* _new(Args&&... a_Args)
    {
        return new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
    }

    template<class T>
    void _delete(TypeIndentityT<T*> a_p)
    {
        a_p->~T();
        m_pAlloc->deallocate(a_p);
    }

private:
    void* _alloc(size_t size, size_t align) { return m_pAlloc->allocate(size, align); }

    void* _relloc(void* ptr, size_t size, size_t align)
    {
        m_pAlloc->deallocate(ptr);
        return m_pAlloc->allocate(size, align);
    }

    void _dealloc(void* ptr) { m_pAlloc->deallocate(ptr); }

private:
public:
    phantom::Signal<void(SourceStream*)> sourceStreamChanged;
    phantom::Signal<void()>              buildSucceeded;

protected:
    SourceStream* m_pSourceStream = nullptr;
    Source*       m_pNativeArchive = nullptr;

private:
    FunctionPointers     m_FunctionPointers;
    InitializerListTypes m_InitializerListTypes;
    FunctionTypes        m_FunctionTypes;
    MethodPointers       m_MethodPointers;
    FieldPointers        m_FieldPointers;
    Imports              m_Imports;
    Sources              m_Importings;
    Sources              m_Dependencies;
    Sources              m_Dependings;
    // this is a forward allocator which never deallocates until being destroyed (optimized chunks because source are
    // generally all-or-nothing)
    ForwardHeapSequence  m_RTAllocator{65536};
    ForwardHeapSequence* m_pAlloc;
    CustomAllocator      m_CustomAlloc;
};

} // namespace lang
} // namespace phantom
