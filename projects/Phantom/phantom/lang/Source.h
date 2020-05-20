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

    template<class T, class... Args>
    T* New(Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        _NewH(ptr, PHANTOM_CLASSOF(T), ptr);
        ptr->initialize();
        return ptr;
    }

    void* PlacementInit(Class* a_pClass, void* a_pInstance);

    template<class T, class... Args>
    T* NewDeferred(Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        _NewDeferredH(ptr, PHANTOM_CLASSOF(T), ptr, lang::TypeInfosOf<T>::object().qualifiedDecoratedName());
        ptr->initialize();
        return ptr;
    }

    template<typename T, class... Args>
    T* NewMeta(Args&&... a_Args)
    {
        _AssertSpecialSymbols<T>();
        PHANTOM_STATIC_ASSERT((std::is_base_of<LanguageElement, T>::value));
        T* ptr = new (m_pAlloc->allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
        if (auto meta = T::MetaClass())
            _NewH(ptr, meta, ptr);
        else
            _NewDeferredH(ptr, meta, ptr,
                          lang::TypeInfosOf<typename T::MetaClassType>::object().qualifiedDecoratedName());
        ptr->initialize();
        return ptr;
    }

    void Delete(LanguageElement* a_pElem);

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
    /// \brief  Gets the source stream associated with this source.
    ///
    /// \return null if no source stream associated with this source, else the source stream.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SourceStream* getSourceStream() const { return m_pSourceStream; }

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

    void               addDependency(Source* a_pSource) { m_Dependencies.insert(a_pSource); }
    ArrayView<Source*> getDependencies() const
    {
        return ArrayView<Source*>(m_Dependencies.begin(), m_Dependencies.end());
    }

protected:
    void   onScopeSymbolAdded(Symbol* a_pSymbol) override;
    void   onScopeSymbolRemoving(Symbol* a_pSymbol) override;
    hash64 computeHash() const override;

private:
    bool _hasImported(Symbol* a_pSymbol, SmallSet<const Source*>& treated) const;
    void _NewH(LanguageElement* a_pElem, Class* a_pClass, void* a_pMD);
    void _NewDeferredH(LanguageElement* a_pElem, Class* a_pClass, void* a_pMD, StringView a_QN);

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

public:
    phantom::Signal<void(SourceStream*)> sourceStreamChanged;
    phantom::Signal<void()>              buildSucceeded;

private:
    SourceStream*        m_pSourceStream = nullptr;
    LanguageElements     m_CreatedElements; ///< list of every created elements at this source level
    FunctionPointers     m_FunctionPointers;
    InitializerListTypes m_InitializerListTypes;
    FunctionTypes        m_FunctionTypes;
    MethodPointers       m_MethodPointers;
    FieldPointers        m_FieldPointers;
    Imports              m_Imports;
    Sources              m_Importings;
    // this is a forward allocator which never deallocates until being destroyed (optimized chunks because source are
    // generally all-or-nothing)
    ForwardHeapSequence  m_RTAllocator{65536};
    ForwardHeapSequence* m_pAlloc;
    CustomAllocator      m_CustomAlloc;
    SmallSet<Source*>    m_Dependencies;
};

} // namespace lang
} // namespace phantom
