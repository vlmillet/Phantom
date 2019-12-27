// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Aggregate.h"

#include <phantom/reflection/Symbol.h>
#include <phantom/thread/RecursiveSpinMutex.h>
#include <phantom/utils/ScopeExit.h>
#include <phantom/utils/Signal.h>
/* **************** Declarations ***************** */
/* *********************************************** */

#define PHANTOM_DECL_TYPE PHANTOM_REGISTER_FOR_VISIT(phantom::reflection::LanguageElementVisitor)

namespace phantom
{
namespace reflection
{
template<class ScopeExit>
struct ScopedConstruction
{
    ScopedConstruction(ScopeExit&& a_ScopeExit, void* a_pMemory)
        : memory(a_pMemory), m_ScopeExit(std::move(a_ScopeExit))
    {
    }

    operator void*() const
    {
        return memory;
    }

private:
    ScopeExit m_ScopeExit;
    void*     memory;
};

template<class ScopeExit>
ScopedConstruction<ScopeExit> makeScopedConstruction(ScopeExit&& a_ScopeExit, void* a_pMemory)
{
    return ScopedConstruction<ScopeExit>(std::forward<ScopeExit>(a_ScopeExit), a_pMemory);
}

class PHANTOM_EXPORT_PHANTOM Type : public Symbol
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Type);

    friend class phantom::reflection::Scope;
    friend class phantom::reflection::Alias;
    friend class Pointer;
    friend class Reference;
    friend class LValueReference;
    friend class RValueReference;
    friend class ConstType;
    friend class VolatileType;
    friend class ConstVolatileType;
    friend class Array;

public:
    typedef Delegate<bool(Type*)> TypeFilter;
    static bool                   DataPointerFilter(Type* a_pType)
    {
        return a_pType->removeReference()->removeQualifiers()->asPointer() != nullptr;
    }
    static bool NoFilter(Type*)
    {
        return true;
    }

    /// \brief  Represents a relation between two types.
    enum class TypeRelation
    {
        None = 0,
        Equal,
        Child,
        Parent,
        Compatible,
        GenericContentChild,
        GenericContentParent,
    };

    /// \brief  Helper that computes automatically alignment via pushing types to it.
    class PHANTOM_EXPORT_PHANTOM AlignmentComputer
    {
    public:
        AlignmentComputer(size_t a_MaxAlignment = 0) : m_MaxAlignment(a_MaxAlignment)
        {
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Align a group of data elements constituting a memory structure
        ///
        /// \param a_DataElements The data elements (Data member, anonymous section...).
        /// \param [out] a_OutSize The computed size
        /// \param [out] a_OutAlignment The computed alignment
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void alignStruct(DataElements const& a_DataElements, size_t& a_OutSize, size_t& a_OutAlignment);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Align a group of data elements constituting a memory union
        ///
        /// \param a_DataElements The data elements (Data member, anonymous section...).
        /// \param [out] a_OutSize The computed size
        /// \param [out] a_OutAlignment The computed alignment
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        void alignUnion(DataElements const& a_DataElements, size_t& a_OutSize, size_t& a_OutAlignment);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Pushes a type into the alignment computer and get the computed offset where the
        /// data must placed into memory
        ///
        /// \param a_pType The data type.
        ///
        /// \return The computed offset where the data must placed into memory.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        size_t push(Type* a_pType);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Pushes an arbitrary data described by given size and alignment, and get the
        /// computed offset where the data must placed into memory
        ///
        /// \param a_uiSize         The data size.
        /// \param a_uiAlignment    The data alignment.
        ///
        /// \return The computed offset where the data must placed into memory.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        size_t push(size_t a_uiSize, size_t a_uiAlignment);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Computes the next viable offset according to maximum alignment.
        ///
        /// \return The next viable offset according to maximum alignment.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        size_t align();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Gets the maximum alignment seen among pushed types.
        ///
        /// \return The maximum alignment seen among pushed types.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        PHANTOM_FORCEINLINE size_t maxAlignment() const
        {
            return m_MaxAlignment;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief  Sets the maximum alignement.
        ///
        /// \param  a_MaxAlignment   The maximum alignment.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        PHANTOM_FORCEINLINE void setMaxAlignement(size_t a_MaxAlignment)
        {
            m_MaxAlignment = a_MaxAlignment;
        }

        void reset()
        {
            m_Result = 0;
            m_MaxAlignment = 0;
        }

    private:
        size_t m_Result = 0;
        size_t m_MaxAlignment = 0;
    };

    static int ConversionRank(TypeKind id);

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a type with given type id and optional modifiers.
    ///
    /// \param  a_eTypeKind   The type id.
    /// \param  a_Modifiers (optional) The type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type(TypeKind a_eTypeKind, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a type with given type id, name and optional modifiers.
    ///
    /// \param  a_eTypeKind   The type id.
    /// \param  a_strName   The type name.
    /// \param  a_Modifiers (optional) The type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a native type with given type id, name, size, alignment and optional
    /// modifiers. .
    ///
    /// \param  a_eTypeKind       The type id.
    /// \param  a_strName       The type name.
    /// \param  a_uiSize        The type size.
    /// \param  a_uiAlignment   The type alignment.
    /// \param  a_Modifiers     (optional) the type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
         uint a_uiFlags);

public:
    ~Type() override;

    void terminate();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type identifier.
    ///
    /// \return The type identifier.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE TypeKind getTypeKind() const
    {
        return m_eTypeKind;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type size.
    ///
    /// \return The type size.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getSize() const;

    void setSize(size_t a_uiSize);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Is 'void' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline bool isVoid() const
    {
        return m_eTypeKind == TypeKind::Void;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets this type as it was a flattened aggregate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getFlattenedAggregateFields(AggregateFields& _aggregateFields) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this type is equivalent to the given one. This means that both types have
    /// the
    ///         same size, alignment and field layout for classes.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isEquivalent(Type* a_pOther) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type alignment.
    ///
    /// \return The type alignment.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getAlignment() const;

    void setAlignment(size_t a_uiAlignment)
    {
        PHANTOM_ASSERT(m_uiAlignment == 0);
        m_uiAlignment = a_uiAlignment;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is singleton.
    ///
    /// \return true if singleton, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isSingleton() const
    {
        return ((m_Modifiers & PHANTOM_R_SINGLETON) == PHANTOM_R_SINGLETON);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is default constructible.
    ///
    /// \return true if default constructible, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isDefaultConstructible() const
    {
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type can be instantiated via default construction (non-abstract +
    /// default constructible).
    ///
    /// \return true if default instanciable, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isDefaultInstanciable() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is a kind of the given type (i.e. inheritance descendant).
    ///
    /// \param [in,out] a_pType The type to test.
    ///
    /// \return true if kind of the given type, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isA(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is a kind of the given type (i.e. inheritance descendant).
    ///
    /// \param [in,out] a_pType The type to test.
    ///
    /// \return true if kind of the given type, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    bool isA() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is the C++11 'auto' placeholder.
    ///
    /// \return true if this type is the C++11 'auto' placeholder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isAuto() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is a derivation of the C++11 'auto' placeholder.
    ///
    /// \return true if this type is a derivation of the C++11 'auto' placeholder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasAuto() const;

    Type* toType() const override
    {
        return const_cast<Type*>(this);
    }
    Type* asType() const override
    {
        return const_cast<Type*>(this);
    }
    virtual LanguageElement* asTemplateElement() const
    {
        return const_cast<Type*>(this);
    }
    virtual LanguageElement* asLanguageElement() const
    {
        return const_cast<Type*>(this);
    }

    virtual bool isPolymorphic() const
    {
        return false;
    }

    virtual bool hasStrongDependencyOnType(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the underlying type if any (ex: enum underlying int type, referenced type of a
    /// reference type, etc...).
    ///
    /// \return null if no underlying type, else the underlying type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* getUnderlyingType() const
    {
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds 'const' to this type.
    ///
    /// \return this type if already 'const', else the corresponding const type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addConst() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds 'volatile' to this type.
    ///
    /// \return this type if already 'volatile', else the corresponding volatile type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addVolatile() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds 'const &' to this type.
    ///
    /// \return this type if already 'const &', else the corresponding 'const &' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* addConstLValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds 'const &&' to this type.
    ///
    /// \return this type if already 'const &&', else the corresponding 'const &&' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* addConstRValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds 'const volatile' to this type.
    ///
    /// \return this type if already 'const volatile', else the corresponding 'const volatile' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* addConstVolatile() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds '[N]' extent to this type.
    ///
    /// \param  N   The array extent N.
    ///
    /// \return this type if already '[N]' extent, else the corresponding '[N]' extent type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addArray(size_t a_uiCount) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds '&' to this type.
    ///
    /// \return this type if already '&', else the corresponding '&' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addLValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds '&&' to this type.
    ///
    /// \return this type if already '&&', else the corresponding '&&' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addRValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds '*' to this type.
    ///
    /// \return this type if already '*', else the corresponding '*' type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addPointer() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a custom extension to this type.
    ///
    /// \param  id  The custom extension id.
    ///
    /// \return this type if already holding the custom extension, else the corresponding extended
    /// type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* addCustom(int id) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const' from this type.
    ///
    /// \return this type if not 'const', else the corresponding type without 'const'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeConst() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'volatile' from this type.
    ///
    /// \return this type if not 'volatile', else the corresponding type without 'volatile'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeVolatile() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const volatile' from this type.
    ///
    /// \return this type if not 'const volatile', else the corresponding type without 'const
    /// volatile'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeConstVolatile() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const' or/and 'volatile' from this type.
    ///
    /// \return this type if not 'const' or/and 'volatile', else the corresponding type without
    /// 'const' or/and 'volatile'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Type* removeQualifiers() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const &' or 'const &&' from this type.
    ///
    /// \return this type if not 'const &' or 'const &&', else the corresponding type without 'const
    /// &' or 'const &&'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* removeConstReference() const
    {
        return removeReference()->removeConst();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const &' from this type.
    ///
    /// \return this type if not 'const &', else the corresponding type without 'const &'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* removeConstLValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '*' or '[X]' extent from this type.
    ///
    /// \return this type if not '*' or '[X]' extent, else the corresponding type without '*' or
    /// '[X]' extent.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removePointerOrArray() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '*' or '&' or '&&' from this type.
    ///
    /// \return this type if not '*' or '&' or '&&', else the corresponding type without '*' or '&'
    /// or '&&'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeAddress() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '[X]' extent from this type.
    ///
    /// \return this type if not '[X]' extent, else the corresponding type without '[X]' extent.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeArray() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '&' or '&&' from this type.
    ///
    /// \return this type if not '&' or '&&', else the corresponding type without '&' or '&&'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeReference() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes everything which extend the original type (qualifiers, pointers, references,
    /// arrays...).
    ///
    /// \return this type if not '&' or '&&', else the corresponding type without '&' or '&&'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeEverything() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '&' from this type.
    ///
    /// \return this type if not '&', else the corresponding type without '&'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeLValueReference() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '&&' from this type.
    ///
    /// \return this type if not '&&', else the corresponding type without '&&'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeRValueReference() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes '*' from this type.
    ///
    /// \return this type if not '*', else the corresponding type without '*'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removePointer() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const' from this type and every underlying type recursively.
    ///
    /// \return this type if not 'const' and none underlying type is 'const', else the corresponding
    /// type without any 'const'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeAllConst() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'volatile' from this type and every underlying type recursively.
    ///
    /// \return this type if not 'volatile' and none underlying type is 'volatile', else the
    /// corresponding type without any 'volatile'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeAllVolatile() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const volatile' from this type and every underlying type recursively.
    ///
    /// \return this type if not 'const volatile' and none underlying type is 'const volatile', else
    /// the corresponding type without any 'const volatile'.

    virtual Type* removeAllConstVolatile() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes 'const' or 'volatile' from this type and every underlying type recursively.
    ///
    /// \return this type if not 'const' or 'volatile' and none underlying type is 'const' or
    /// 'volatile', else the corresponding type without any 'const' or 'volatile'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeAllQualifiers() const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a custom extension from this type.
    ///
    /// \param  id  The custom extension id.
    ///
    /// \return this type if not holding the custom extension, else the corresponding type without
    /// the custom extension.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* removeCustom(int) const
    {
        return const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Replicates the 'const' '&' and &&' pattern of given model type to this type.
    ///         ex: (int const&).replicate(float) => float const&
    ///
    /// \param a_pInput    The type replicated to this one (float in our example).
    ///
    /// \return The replicated type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* replicate(Type* a_pInput) const
    {
        return a_pInput;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs basic fundamental conversions (no user defined conversion applied, use
    /// phantom.lang to do so).
    ///
    /// \param a_pSrcType   The cast st=rc type.
    /// \param a_pSrc          The source pointer (must be of the current type).
    ///
    /// \return null if it fails, else the adjusted pointer issued from the cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs template pointer cast.
    ///
    /// \param a_pSrc          The source pointer (must be of the current type).
    ///
    /// \return null if it fails, else the adjusted pointer issued from the cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T* cast(void* a_pPointer) const;
    template<class T>
    T const* cast(void const* a_pPointer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs generic pointer cast.
    ///
    /// \param a_pTargetType   The cast target type.
    /// \param a_pSrc          The source pointer (must be of the current type).
    ///
    /// \return null if it fails, else the adjusted pointer issued from the cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* cast(Type* a_pTargetType, void* a_pSrc) const
    {
        return a_pTargetType == this ? a_pSrc : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs generic pointer cast.
    ///
    /// \param a_pTargetType   The cast target type.
    /// \param a_pSrc          The source pointer (must be of the current type).
    ///
    /// \return null if it fails, else the adjusted pointer issued from the cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void const* cast(Type* a_pTargetType, void const* a_pSrc) const
    {
        return a_pTargetType == this ? a_pSrc : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs pointer upcast.
    ///
    /// \param a_pTargetType   The cast target type.
    /// \param a_pSrc          The source pointer (must be of the current type).
    ///
    /// \return null if it fails, else the adjusted pointer issued from the cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* upcast(Type* a_pTargetType, void* a_pSrc) const
    {
        return a_pTargetType == this ? a_pSrc : nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs pointer downcast.
    ///
    /// \param a_pTargetType   The cast target type.
    /// \param a_pSrc          The source pointer (must be of the current type).
    ///
    /// \return null if it fails, else the adjusted pointer issued from the cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* downcast(Type* a_pTargetType, void* a_pSrc) const
    {
        return a_pTargetType == this ? a_pSrc : nullptr;
    }

    /// Allocation

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Allocates memory required to hold this type.
    ///
    /// \return null if it fails, else the allocated memory.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* allocate() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Deallocates given memory containg an instance of this type.
    ///
    /// \param a_pAddress The memory to deallocate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void deallocate(void* a_pInstance) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Allocates memory required to hold N instance of this type.
    ///
    /// \param  N   The instance count.
    ///
    /// \return null if it fails, else the allocated memory chunk.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* allocate(size_t a_uiCount) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Deallocates given memory containg N instance of this type.
    ///
    /// \param a_pChunk The memory chunk to deallocate.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void deallocate(void* a_pChunk, size_t N) const;

#if PHANTOM_CUSTOM_ENABLE_ALLOCATION_INFOS
    /// \internal
    HAUNT_OFF virtual void* allocate(PHANTOM_MEMORY_STAT_INSERT_PARAMS) const;
    /// \internal
    HAUNT_OFF virtual void* allocate(size_t a_uiCount PHANTOM_MEMORY_STAT_APPEND_PARAMS) const;
#endif

    // Construction

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an instance of this type at given memory address.
    ///
    /// \param a_pMemory   The memory address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void construct(void* a_pMemory) const
    {
        memset(a_pMemory, 0, m_uiSize);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an instance of this type at given memory address and destroy at end of
    /// caller's scope.
    ///
    /// \param a_pMemory   The memory address to construct.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    auto scopedConstruct(void* a_pMemory) const
    {
        construct(a_pMemory);
        return makeScopedConstruction(makeScopeExit([=]() { destroy(a_pMemory); }), a_pMemory);
    }
    auto localConstruct(void* a_pMemory) const
    {
        return scopedConstruct(a_pMemory);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Destroys the given a_pInstance.
    ///
    /// \param a_pInstance The instance to destroy.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void destroy(void*) const
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Hashed an instance of this Type.
    ///
    /// \param  a_pInstance   The instance to hash.
    ///
    /// \return The computed hash.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual uint64_t hash(const void* a_pInstance) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Allocates and constructs an instance of this type with default allocators
    ///
    /// \return null if it fails, else the new instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* newInstance() const;

#if PHANTOM_CUSTOM_ENABLE_ALLOCATION_INFOS
    HAUNT_OFF virtual void* newInstance(PHANTOM_MEMORY_STAT_INSERT_PARAMS) const;
#endif
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  destroys and deallocate an instance of this type with default allocators
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void deleteInstance(void* a_pInstance) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  In place PHANTOM_NEW(instance) of this type : construct [+ install + initialize if
    ///         class].
    ///
    /// \return null if it fails, else the PHANTOM_NEW(instance).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* placementNewInstance(void* a_pMemory) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  In place delete instance of this type
    ///
    /// \return null if it fails, else the PHANTOM_NEW(instance).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void placementDeleteInstance(void* a_pMemory) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets type relation with given other type.
    ///
    /// \param  The other type.
    ///
    /// \return The relation with the other type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual TypeRelation getRelationWith(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if the two given value of this type are equal.
    ///
    /// \param  a_pValueAddress0 The address where to find the first value .
    /// \param  a_pValueAddress1 The address where to find the second value.
    ///
    /// \return true if value are equal, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool equal(void const* a_pSrc0, void const* a_pSrc1) const;

    virtual void valueFromString(StringView a_str, void* dest) const;
    virtual void valueToString(StringBuffer& a_Buf, const void* src) const;
    virtual void valueToLiteral(StringBuffer& a_Buf, const void* src) const;

    using Symbol::fetchElements;
    virtual void fetchElements(LanguageElements& out, Class* a_pClass = nullptr) const;

    // copy and move semantics

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Copy construction semantic.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void copyConstruct(void* a_pDest, void const* a_pSrc) const
    {
        memcpy(a_pDest, a_pSrc, m_uiSize);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Move construction semantic.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void moveConstruct(void* a_pDest, void* a_pSrc) const
    {
        memcpy(a_pDest, a_pSrc, m_uiSize);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Copy assignment semantic.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void copyAssign(void* a_pDest, void const* a_pSrc) const
    {
        memcpy(a_pDest, a_pSrc, m_uiSize);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Move assignment semantic.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void moveAssign(void* a_pDest, void* a_pSrc) const
    {
        memcpy(a_pDest, a_pSrc, m_uiSize);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Copy values (for class : alias to copy assignment semantic).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void copy(void* a_pDest, void const* a_pSrc)
    {
        copyAssign(a_pDest, a_pSrc);
    }

    virtual bool isCopyable() const;
    virtual bool isCopyConstructible() const;
    virtual bool isCopyAssignable() const;
    virtual bool isTriviallyCopyAssignable() const;
    virtual bool isTriviallyCopyConstructible() const;

    virtual bool isMoveable() const;
    virtual bool isMoveConstructible() const;
    virtual bool isMoveAssignable() const;
    virtual bool isTriviallyMoveAssignable() const;
    virtual bool isTriviallyMoveConstructible() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type has copy disabled.
    ///
    /// \return true if copy disabled, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool hasCopyDisabled() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type has move disabled.
    ///
    /// \return true if move disabled, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool hasMoveDisabled() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  If any, provides the unsigned counterpart of this type. For example if this type is
    /// 'int', it returns 'unsigned int'.
    ///
    /// \return The counterpart unsigned type if exists, null if no sign exist for this type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* unsignedType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  If any, provides the signed counterpart of this type. For example if this type is
    /// 'unsigned int', it returns 'int'.
    ///
    /// \return The counterpart signed type if exists, null if no sign exist for this type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* signedType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  If any, provides the long counterpart of this type. For example if this type is
    /// 'int', it returns 'long'.
    ///
    /// \return The counterpart long type if exists, null if no long type exist for this type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* longType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is a signed integer
    ///
    /// \return true if signed integer, false otherwise.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isSignedInteger() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is an unsigned integer
    ///
    /// \return true if unsigned integer, false otherwise.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isUnsignedInteger() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Promotes this type according to the C++ standard, if possible.
    ///
    /// \return this type if no promotion available, else the promoted type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* promote() const; /// standard 4.x

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is promoted arithmetic type according to the C++ standard
    /// (13.6.2).
    ///
    /// \return true if promoted arithmetic type, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline bool isPromotedArithmeticType() const
    {
        return asFloatingPointType() OR isPromotedIntegralType();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this type is promoted integral type according to the C++ standard (13.6.2).
    ///
    /// \return true if promoted integral type, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline bool isPromotedIntegralType() const
    {
        return asIntegralType() AND promote() == const_cast<Type*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the data pointer level of this type (0 if not a pointer, 1 if '*', 2 if '**',
    /// etc...).
    ///
    /// \return The data pointer level.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual uint getDataPointerLevel() const
    {
        return 0;
    }

    Scope* getScope() const;

    /// \brief  Removes this type from its scope.
    void removeFromScope();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the owner type of this type, if exists.
    ///
    /// \return null if no owner or owner is not a type, else the owner type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getOwnerType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the extended type (reference types, pointer types, array types...) count of
    /// this type .
    ///
    /// \return The extended type count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getExtendedTypeCount() const
    {
        return m_pExtendedTypes == NULL ? 0 : m_pExtendedTypes->size();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the extended type (reference types, pointer types, array types...) at given
    /// index position .
    ///
    /// \return The extended type count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getExtendedType(size_t index) const
    {
        PHANTOM_ASSERT(index < getExtendedTypeCount());
        return (*m_pExtendedTypes)[index];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the common base ancestor of this type and the given one if they are class (i.e.
    /// the first common base class, recursively through both classes hierarchy).
    ///
    /// \param  The type to compare to.
    ///
    /// \return null if no common base ancestor or types are not classes, else the common base
    /// ancestor class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Type* getCommonBaseAncestor(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the data pointer type of this type.
    ///
    /// \return null if the data pointer type has not been created, else the data pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Pointer* getPointer() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the l-value reference type of this type.
    ///
    /// \return null if the l-value reference type has not been created, else the l-value reference
    /// type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LValueReference* getLValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the r value reference type of this type.
    ///
    /// \return null if the r-value reference type has not been created, else the r-value reference
    /// type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    RValueReference* getRValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the array type of this type with given extent.
    ///
    /// \param  a_uiCount   The array extent (if == 0, gets the abstract array type).
    ///
    /// \return null if the array type with given extent has not been created, else the array type
    /// with given extent.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Array* getArray(size_t a_uiCount) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the const type of this type.
    ///
    /// \return null if the const type has not been created, else the const type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstType* getConstType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the volatile type of this type.
    ///
    /// \return null if the volatile type has not been created, else the volatile type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VolatileType* getVolatileType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the const volatile type of this type.
    ///
    /// \return null if the const volatile type has not been created, else the const volatile type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstVolatileType* getConstVolatileType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the data pointer type of N levels of this type.
    ///
    /// \return null if data pointer type cannot be created for this type, else the data pointer
    /// type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* makePointer(size_t a_uiPointerLevel) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the data pointer type of this type.
    ///
    /// \return null if data pointer type cannot be created for this type, else the data pointer
    /// type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Pointer* makePointer() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the l-value reference type of this type.
    ///
    /// \return null if l-value reference type cannot be created for this type, else the l-value
    /// reference type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LValueReference* makeLValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the r-value reference type of this type.
    ///
    /// \return null if r-value reference type cannot be created for this type, else the r-value
    /// reference type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    RValueReference* makeRValueReference() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the array type of this type with given extent.
    ///
    /// \param  a_uiCount   The array extent (if == 0, gets or creates the abstract array type).
    ///
    /// \return null if array types cannot be created for this type, else the array type with given
    /// extent.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Array* makeArray(size_t a_uiCount) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the const type of this type.
    ///
    /// \return null if const type cannot be created for this type, else the const type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstType* makeConst() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the volatile type of this type.
    ///
    /// \return null if volatile type cannot be created for this type, else the volatile type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VolatileType* makeVolatile() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets or create the const volatile type of this type.
    ///
    /// \return null if const volatile type cannot be created for this type, else the const volatile
    /// type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ConstVolatileType* makeConstVolatile() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the template associated with this type, if any.
    ///
    /// \return null if not a template specialization, else the template.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Template* getTemplate() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pointer adjustment offset required to convert a pointer of this to the
    /// pointer of the given type.
    ///
    /// \param a_pType The type we want to have our pointer adjusted to its own.
    ///
    /// \return The pointer adjustment offset if conversion if possible, else '-1'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual ptrdiff_t getPointerAdjustmentOffset(Type* a_pType) const;

    /// \internal
    bool partialAccepts(LanguageElement* a_pLanguageElement, size_t& a_Score,
                        PlaceholderMap& a_Deductions) const override;
    /// \internal
    virtual bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const;

protected:
    void                       onElementRemoved(LanguageElement* a_pElement) override;
    virtual Pointer*           createPointer() const;
    virtual LValueReference*   createLValueReference() const;
    virtual RValueReference*   createRValueReference() const;
    virtual Array*             createArray(size_t a_uiCount) const;
    virtual ConstType*         createConstType() const;
    virtual VolatileType*      createVolatileType() const;
    virtual ConstVolatileType* createConstVolatileType() const;
    void                       removeExtendedType(Type* a_pType);

    void onReferencedElementRemoved(LanguageElement* a_pElement) override;
    void onAncestorChanged(LanguageElement* a_pOwner) override;
    void onAncestorAboutToBeChanged(LanguageElement* a_pOwner) override;

#if PHANTOM_CUSTOM_ENABLE_INSTANTIATION_SIGNALS
public:
    /**
     *  @name Signals
     */
    //@{
    phantom::Signal<void(void*)> kindCreated;
    phantom::Signal<void(void*)> kindDestroyed;
    //@}
#endif

protected:
    TypeKind                   m_eTypeKind;
    mutable Types*             m_pExtendedTypes{};
    size_t                     m_uiSize{};
    size_t                     m_uiAlignment{};
    mutable RecursiveSpinMutex m_ExtendedTypesMutex;
};

#define PHANTOM_LOCAL(pType) pType->localConstruct(PHANTOM_ALLOCA(pType->getSize()))

} // namespace reflection
} // namespace phantom
