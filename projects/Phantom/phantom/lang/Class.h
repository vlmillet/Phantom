// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "../utils/Optional.h"

#include <phantom/detail/ClassOfFwd.h>
#include <phantom/lang/ClassType.h>
#include <phantom/utils/SmallMap.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace lang
{
class InstanceCache;

/// \brief  Base class data.
struct BaseClass
{
    BaseClass() {}
    BaseClass(Class* s, size_t o, Access a = Access::Public) : access(a), baseClass(s), offset(o) {}
    /// \brief  The base class access (Access::Public, Access::Protected or Access::Private).
    Access access = Access::Public;
    /// \brief  The base class.
    Class* baseClass = nullptr;
    /// \brief  The address offset from the derived address (for cast purpose).
    size_t offset = 0;

    operator Class*() const { return baseClass; }
};
typedef SmallVector<BaseClass, 1> BaseClasses;

struct PHANTOM_EXPORT_PHANTOM ClassBuilder
{
public:
    static ClassBuilder struct_(StringView a_Name, size_t a_MinAlign = 0)
    {
        return ClassBuilder(nullptr, nullptr, a_Name, Access::Public, a_MinAlign);
    }
    static ClassBuilder class_(StringView a_Name, size_t a_MinAlign = 0)
    {
        return ClassBuilder(nullptr, nullptr, a_Name, Access::Private, a_MinAlign);
    }
    static ClassBuilder struct_(Scope* a_pOwnerScope, StringView a_Name, size_t a_MinAlign = 0)
    {
        return ClassBuilder(a_pOwnerScope, nullptr, a_Name, Access::Public, a_MinAlign);
    }
    static ClassBuilder class_(Scope* a_pOwnerScope, StringView a_Name, size_t a_MinAlign = 0)
    {
        return ClassBuilder(a_pOwnerScope, nullptr, a_Name, Access::Private, a_MinAlign);
    }
    static ClassBuilder struct_(Scope* a_pOwnerScope, Scope* a_pNamingScope, StringView a_Name, size_t a_MinAlign = 0)
    {
        return ClassBuilder(a_pOwnerScope, a_pNamingScope, a_Name, Access::Public, a_MinAlign);
    }
    static ClassBuilder class_(Scope* a_pOwnerScope, Scope* a_pNamingScope, StringView a_Name, size_t a_MinAlign = 0)
    {
        return ClassBuilder(a_pOwnerScope, a_pNamingScope, a_Name, Access::Private, a_MinAlign);
    }

public:
    ClassBuilder(Scope* a_pOwnerScope, Scope* a_pNamingScope, StringView a_Name, Access a_Access,
                 size_t a_MinAlign = 0);

    Scope*        scope() const;
    ClassBuilder& inherits(Class* _class);
    ClassBuilder& field(Type* a_pType, StringView a_Name, size_t a_Align = 0, uint a_FilterMask = ~0u);
    ClassBuilder& access(Access a_Access);
    Class*        finalize();

    operator Class*() { return finalize(); }

private:
    Class* m_pClass = nullptr;
    Access m_Access;
    size_t m_MinAlign = 0;
};

/// \brief  Represents an object oriented class.
class PHANTOM_EXPORT_PHANTOM Class : public ClassType
{
    PHANTOM_DECL_TYPE;

public:
    static Class*           MetaClass();
    HAUNT_OFF static Class* metaClass;

public:
    friend class phantom::Phantom;
    friend class phantom::lang::Namespace;

    template<typename t_Ty, int t_default_installer_id>
    friend struct phantom::detail::DefaultInstallerH;

    template<typename t_Ty, int t_default_initializer_id>
    friend struct phantom::detail::DefaultInitializerH;

public:
    struct ExtraData : ClassType::ExtraData
    {
        ExtraData() : m_uiClassPtrOffset(0), m_uiStateMachineDataPtrOffset(0), m_bHasVTablePtr(true) {}
        size_t m_uiFieldMemoryOffset{};
        size_t m_uiClassPtrOffset;
        size_t m_uiStateMachineDataPtrOffset;
        bool   m_bHasVTablePtr;
    };

public:
    /// \brief  Creates an empty class.
    Class();
    Class(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : Class(TypeKind::Class, a_strName, a_Modifiers, a_uiFlags)
    {
    }

protected: // Reserved to native class derivation
    Class(TypeKind a_eTypeKind, StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    Class(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
          uint a_uiFlags);
    Class(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags)
        : Class(TypeKind::Class, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
    {
    }

public:
    ~Class() override;

    void initialize();
    void terminate();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the common base class of this class and the given one.
    ///
    /// \param  The class to compare to.
    ///
    /// \return null if no common base ancestor or types are not classes, else the common base
    /// ancestor class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getCommonBaseClass(Class* a_pClass) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Keep only non overloaded pure virtual member functions.
    ///
    /// \param [in,out]  a_Result vector passed is filtered to keep only the pure virtual member
    /// functions that haven't been overloaded, recursively through all the base classes.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void filtersNonOverloadedPureVirtualMethodsCascade(Methods& a_Result) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Filters the vector passed as argument and keeps only inside the abstract member
    /// functions that haven't been overloaded, recursively in all the base classes.
    ///
    /// \param [out]  a_pOutList If non-null, vector filtered.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findPureVirtualMethods(Methods& a_Result) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a subroutine from String, recursively through all the base classes.
    ///
    /// \param   a_strIdentifierString The identifier String (ex: "member_function(int)" ).
    ///
    /// \return NULL if it fails, else the member_function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutineCascade(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets subroutine cascade.
    ///
    /// \param  a_strName           The subroutine name.
    /// \param  a_ParametersTypes   The parameters' types.
    /// \param  a_Modifiers         (optional) The modifiers (or qualifiers. ex: PHANTOM_R_CONST).
    ///
    /// \return null if it fails, else the subroutine cascade.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutineCascade(StringView a_strName, TypesView a_FunctionSignature,
                                     Modifiers a_Modifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets all valueMembers recursively through all the base classes.
    ///
    /// \param [in,out]  a_ValueMembers A vector to store the value members.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void getValueMembersCascade(ValueMembers& out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets overriden versions of a given method in this class.
    ///
    /// \param [in] a_pOverridenMethod  The overriden member_function (must be non-NULL).
    /// \param [out]    a_Overridens            A vector to store the overriden member functions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findOverriddenMethods(Method* a_pOverridingCandidate, Methods& a_Result);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets overriden versions of a given method in this class.
    ///
    /// \param [in]     a_strName       The overriden member_function name.
    /// \param [in]     a_pSignature    The overriden member_function signature .
    /// \param [out]    a_Overridens    A vector to store the overriden member functions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findOverriddenMethods(StringView a_strName, Signature* a_pSignature, Methods& a_Result);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sort base and root member functions.
    ///
    /// \param [in,out] a_pBaseVMT                  If non-null, the base virtual method table.
    /// \param [in,out] a_OutBaseMethods    If non-null, the out base member functions.
    /// \param [in,out] a_OutRootMethods    If non-null, the out root member functions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void sortBaseAndRootMethods(VirtualMethodTable* a_pBaseVMT, Methods* a_OutBaseMethods,
                                Methods* a_OutRootMethods) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a non-static method by its signature String, recursively through all the base
    /// classes (stop at first occurence found by deep search).
    ///
    /// \param   a_strIdentifierString The identifier String (ex: \"member_function(int)\").
    ///
    /// \return null if it fails, else the instance member_function cascade.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethodCascade(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a non-static method by its name and parameters types, recursively through all
    /// the base classes.
    ///
    /// \param  a_strName           The method name.
    /// \param  a_ParameterTypes    Parameters types.
    /// \param  a_Qualifiers        (optional) method's qualifiers (ex: Modifiers::Const).
    ///
    /// \return null if it fails to find, else the method cascade.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethodCascade(StringView a_strName, TypesView a_ParameterTypes, Modifiers a_Modifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a non-static method by its name and parameters types, recursively through all
    /// the base classes.
    ///
    /// \param  a_strName               The method name.
    /// \param  a_pSingleParameterType  single parameter type.
    /// \param  a_Qualifiers            (optional) method's qualifiers (ex: Modifiers::Const).
    ///
    /// \return null if it fails to find, else the method cascade.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethodCascade(StringView a_strName, Type* a_pSingleParameterType, Modifiers a_Qualifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a value member by its name, recursively through all the base classes (stop at
    /// first occurence found by deep search).
    ///
    /// \param   a_strValueMemberName The valueMember name.
    ///
    /// \return null if it fails, else the value member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ValueMember* getValueMemberCascade(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for the first public value members pointing the given value type,
    /// recursively through all the base classes.
    ///
    /// \param [in,out] a_pType If non-null, the type pointed by the candidates value members (means
    /// value member types are pointer types). \param [in,out] out     [in,out] If non-null, the
    /// out.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findPublicValueMembersPointingValueTypeCascade(Type* a_pType, ValueMembers& out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a non-static Data member by its name, recursively through all the base classes
    /// (stop at first occurence found by deep search).
    ///
    /// \param   a_strFieldName The Data member name.
    ///
    /// \return null if it fails, else the Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getFieldCascade(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a non-static Data member by its offset-to-base value, recursively through all
    /// the base classes.
    ///
    /// \param   a_strFieldName The Data member name.
    ///
    /// \return null if it fails, else the Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getFieldByOffset(size_t a_uiOffset) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member including the given offset from base address.
    ///
    /// \param  a_pBaseAddress          The base address.
    /// \param  a_pFieldAddress    The Data member address.
    ///
    /// \return null if no Data member found, else the Data member at address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getFieldAtOffset(size_t a_uiOffset) const override;

    using ClassType::getFields;

    void getFields(AggregateFields& a_OutFields) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a property by its name, recursively through all the base classes.
    ///
    /// \param   a_strPropertyName The instance dataMember name.
    ///
    /// \return null if it fails, else the instance dataMember.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property* getPropertyCascade(StringView a_strPropertyName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a static method by its name and parameters types, recursively through all the
    /// base classes (stop at first occurence found by deep search).
    ///
    /// \param   a_strName                 The name.
    /// \param [in,out]  a_pParameterTypes If non-null, list of the parameters' types.
    /// \param   a_uiParameterCount        Number of parameters.
    /// \param   a_Modifiers             (optional) member_function's modifiers (ex:
    /// PHANTOM_R_CONST).
    ///
    /// \return null if it fails, else the instance member_function cascade.²
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StaticMethod* getStaticMethodCascade(StringView a_strName, TypesView a_FunctionSignature) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a static method by its signature name, recursively through all the base classes
    /// (stop at first occurence found by deep search).
    ///
    /// \param   a_strSignatureName The signature name.
    ///
    /// \return null if it fails, else the static member_function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StaticMethod* getStaticMethodCascade(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets a static Data member by its name, recursively through all the base classes (stop
    /// at first occurence found by deep search).
    ///
    /// \param   a_strName The static Data member name.
    ///
    /// \return null if it fails, else the static Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StaticField* getStaticFieldCascade(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Gets the size of the virtual method table at a_uindex.
    ///
    /// \param a_uiIndex Zero-based index.
    ///
    /// \return The size of the virtual method table at a_uindex.

    size_t getVirtualMethodTableSize(size_t a_uiIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class accepts the subroutine described with the given parameters.
    ///
    /// \param  a_pReturnType                       The candidate subroutine return type.
    /// \param  a_strName                           The candidate subroutine name.
    /// \param  a_ParametersTypes                   The candidate subroutine parameters types.
    /// \param  a_Modifiers                         The candidate subroutine modifiers.
    /// \param  a_uiFlags                           The candidate subroutine flags.
    /// \param  a_pOutConflictingMethods    (optional) [in,out] If non-null, will contain
    ///                                             after call the conflicting subroutines that
    ///                                             forbid the candidate subroutine to be accepted.
    ///
    /// \return true if it succeeds, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types, Modifiers a_Modifiers,
                           uint a_uiFlags, Subroutines* a_pOutConflictingMethods = NULL) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class could accept in its derived class the subroutine described with
    ///         the given parameters (use C++ covariant convention).
    ///
    /// \param  a_pReturnType                               The candidate method return type.
    /// \param  a_strName                                   The candidate method name.
    /// \param  a_Types                                     The candidate method parameters
    ///                                                     types.
    /// \param  a_Modifiers                                 The candidate method modifiers.
    /// \param  a_uiFlags                                   The flags.
    /// \param [out]    a_pOutConflictingMethods    (optional) [in,out] If non-null, will
    ///                                                     contain after call the
    ///                                                        conflicting subroutines that forbid
    ///                                                        the candidate subroutine to be
    ///                                                        accepted.
    ///
    /// \return true if it succeeds, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool acceptsDerivedClassSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types,
                                               Modifiers a_Modifiers, uint a_uiFlags,
                                               Subroutines* a_pOutConflictingMethods) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the base class at given position a_uiIndex.
    ///
    /// \param  index   Zero-based index of the base class.
    ///
    /// \return The base class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Class* getBaseClass(size_t a_uiIndex) const { return m_BaseClasses[a_uiIndex].baseClass; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a cast from current class to the base class at given index.
    ///
    /// \param [in] a_pInstance An instance of this Class.
    /// \param  index               Zero-based index of the base class.
    ///
    /// \return The adjusted address of a_pInstance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void* baseClassCast(void* a_pInstance, size_t index) const
    {
        return (((byte*)a_pInstance) + m_BaseClasses[index].offset);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Performs a cast from current class to the base class at given index.
    ///
    /// \param [in] a_pInstance A const instance of this Class.
    /// \param  index               Zero-based index of the base class.
    ///
    /// \return The adjusted address of a_pInstance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline const void* baseClassCast(const void* a_pInstance, size_t index) const
    {
        return (((const byte*)a_pInstance) + m_BaseClasses[index].offset);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getInheritanceLevel(size_t i) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Computes how many classes separate this class from given base class.
    ///
    /// \param [in] a_pBaseClass    If non-null, the base class.
    ///
    /// \return The inheritance level from base.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getInheritanceLevelFromBase(Class* a_pClass) const;

    bool hasPublicMember(Symbol* a_pLanguageElement) const override;
    bool hasProtectedMember(Symbol* a_pSymbol) const override;
    bool hasPrivateMember(Symbol* a_pSymbol) const override;

    bool isListInitializable() const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the virtual method count in a given virtual method table.
    ///
    /// \param  a_uiVTableOffset    The offset where to find the virtual method table.
    ///
    /// \return The virtual method count, or -1 if no table found at given 'm_uiVTableOffset'
    /// offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual size_t getVirtualMethodCount(size_t a_uiOffset) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the virtual method table list.
    ///
    /// \return the virtual method table list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    VirtualMethodTables const& getVirtualMethodTables() const { return m_VirtualMethodTables; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a base class.
    ///
    /// \param [in,out] a_pClass    If non-null, the class.
    /// \param  a_Access            (optional) the access.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addBaseClass(Class* a_pClass, Access a_Access = Access::Public);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    /// \brief  Adds a base class to given offset (for native purpose, do not invoke explicitely).
    ///
    /// \param [in,out] a_pClass    If non-null, the class.
    /// \param  a_uiOffset          The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addBaseClass(Class* a_pBaseClass, size_t a_uiOffset, Access a_Access = Access::Public);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets base classes table.
    ///
    /// \return The base classes table.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const BaseClasses& getBaseClasses() const { return m_BaseClasses; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the access of the given base class.
    ///
    /// \param a_pBaseClass    The base class.
    ///
    /// \return The base class access.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Access getBaseClassAccess(Class* a_pClass) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the access of a given base class, recursively.
    ///
    /// \param a_pBaseClass    The base class.
    ///
    /// \return The base class access.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Access getBaseClassAccessCascade(Class* a_pClass) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class has a given base class.
    ///
    /// \param  a_pBaseClass            The base class candidate.
    /// \param  a_pInheritanceAccess    (optional) If non-null, will contain the inheritance access
    ///                                 of the base class if found.
    ///
    /// \return true if the base class candidate is effectively a base class, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasBaseClass(Class* a_pClass, Access* a_pInheritanceAccess = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class has a given base class, recursively.
    ///
    /// \param  a_pBaseClass    The base class candidate.
    ///
    /// \return true if the base class candidate is effectively a base class, recursively, false if
    /// not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasBaseClassCascade(Class* a_pClass) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the derived class at given position index.
    ///
    /// \param  a_uiIndex   The index of the derived class.
    ///
    /// \return null if it fails, else the derived class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Class* getDerivedClass(size_t a_uiIndex) const { return m_DerivedClasses[a_uiIndex]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the derived classes.
    ///
    /// \param [out]    a_DerivedClasses    The input vector to store the result derived classes.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Classes const& getDerivedClasses() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all the derived classes, recursively.
    ///
    /// \param [out]    a_DerivedClasses    The input vector to store the result derived classes.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getDerivedClassesCascade(Classes& out) const;

#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all the derived classes, recursively.
    ///
    /// \param [out]    a_DerivedClasses    The input vector to store the result derived classes.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Classes const& getDerivedClassesCascade() const;
#endif
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class has a given derived class.
    ///
    /// \param  a_pDerivedClass     The derived class candidate.
    /// \param  a_pDerivedAccess    (optional) If non-null, will contain the inheritance access of
    ///                             this class inside the derived class.
    ///
    /// \return true if the derived class candidate is effectively a base class, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDerivedClass(Class* a_pClass, Access* a_pDerivedAccess = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class has a given derived class, recursively.
    ///
    /// \param  a_pDerivedClass     The derived class candidate.
    /// \param  a_pDerivedAccess    (optional) If non-null, will contain the inheritance access of
    ///                             this class inside the derived class.
    ///
    /// \return true if the derived class candidate is effectively a base class, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasDerivedClassCascade(Class* a_pClass, Access* a_pDerivedAccess = nullptr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class has a member recursively through all base classes.
    ///
    /// \param [in,out] a_pElement  The element.
    ///
    /// \return true if has member in this or any base, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasMemberCascade(LanguageElement* a_pElement) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset of the base class at given position index.
    ///
    /// \param  a_uiBaseClassIndex  Zero-based index of the base class.
    ///
    /// \return The base class offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PHANTOM_FORCEINLINE ptrdiff_t getBaseClassOffset(size_t a_uiBaseClassIndex) const
    {
        return m_BaseClasses[a_uiBaseClassIndex].offset;
    }

    void setBaseClassOffset(Class* a_pClass, ptrdiff_t a_Offset);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset of the given base class.
    ///
    /// \param  a_pBaseClass    The base class.
    ///
    /// \return The base class offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline ptrdiff_t getBaseClassOffset(Class* a_pBaseClass) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the offset of the given base class, recursively (addition of every offset in
    /// the
    ///         derivation path).
    ///
    /// \param  a_pBaseClass    The base class.
    ///
    /// \return The base class offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline ptrdiff_t getBaseClassOffsetCascade(Class* a_pBaseType) const;
    ptrdiff_t        getPointerAdjustmentOffset(Type* a_pType) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the adjustement offset needed to cast from a pointer of this class to a pointer
    /// of the given class.
    ///
    /// \param [in,out] a_pClass    The destination class.
    ///
    /// \return The pointer cast adjustment offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrdiff_t getPointerAdjustmentOffset(Class* a_pClass) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class is a root class (no base class).
    ///
    /// \return true if root class, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isRootClass() const { return m_BaseClasses.empty(); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class is a kind of the given class (i.e. a derived class).
    ///
    /// \param [in,out] a_pClass The candidate base class.
    ///
    /// \return true if a descendant recursively of a_pClass, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isA(Class* a_pType) const;
    bool isA(Type* a_pType) const override { return a_pType->asClass() ? isA(static_cast<Class*>(a_pType)) : false; }

    template<class T>
    bool isA() const
    {
        return std::is_class<T>::value && isA(static_cast<Type*>(PHANTOM_TYPEOF(T)));
    }

    TypeRelation getRelationWith(Type* a_pType) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Overrides the default value of a base class value members.
    ///
    /// \return null if it fails, else the state machine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setOverriddenDefaultExpression(ValueMember* a_pValueMember, Expression* a_pExpression);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the overriden value of a base class value member.
    ///
    /// \return null if no default expression defined, else the default value expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getOverriddenDefaultExpression(ValueMember* a_pValueMember) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the overriden value of a base class field.
    ///
    /// \return null if no default expression defined, else the default value expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getOverriddenDefaultExpressionCascade(ValueMember* a_pValueMember) const;

    bool isPolymorphic() const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Visit function that allows you to browse the inheritance hierarchy of an instance.
    ///         Than can be used for example to create constructor like invocations
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class Impl>
    void visitInheritance(void* a_pInstance, Impl&& a_Visitor)
    {
        for (BaseClass const& bc : m_BaseClasses)
        {
            bc.baseClass->visitInheritance((char*)a_pInstance + bc.offset, a_Visitor);
        }
        a_Visitor(const_cast<Class*>(this), a_pInstance);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Visit function that allows you to browse the inheritance hierarchy of an instance
    /// (reverse way).
    ///         Than can be used for example to create destructor like invocations
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class Impl>
    void visitInheritanceReversed(void* a_pInstance, Impl&& a_Visitor)
    {
        a_Visitor(const_cast<Class*>(this), a_pInstance);
        auto it = std::make_reverse_iterator(m_BaseClasses.end());
        auto end = std::make_reverse_iterator(m_BaseClasses.begin());
        for (; it != end; ++it)
        {
            it->baseClass->visitInheritanceReversed((char*)a_pInstance + it->offset, a_Visitor);
        }
    }

    using Type::allocate;
    using Type::deallocate;

    void* allocate() const override;
    void  deallocate(void* a_pInstance) const override;
    void* allocate(size_t a_uiCount) const override;
    void  deallocate(void* a_pInstance, size_t a_uiCount) const override;
    void  construct(void* a_pInstance) const override;
    void  destroy(void* a_pObject) const override;

    void*  getSingleton() const { return m_pSingleton; }
    size_t getInstanceCount() const { return m_InstanceCount; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Upcasts.
    ///
    /// \param [in,out] a_pBaseClass    The base class.
    /// \param [in,out] a_pInstance  The instance.
    ///
    /// \return null if it fails, else the adjusted pointer.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void*       upcast(Class* a_pBaseClass, void* a_pInstance) const;
    void const* upcast(Class* a_pBaseClass, void const* a_pInstance) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Downcasts.
    ///
    /// \param [in,out] a_pDerivedClass    The derived class.
    /// \param [in,out] a_pInstance  The instance.
    ///
    /// \return null if it fails, else the adjusted pointer.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void*       downcast(Class* a_pDerivedClass, void* a_pInstance) const;
    void const* downcast(Class* a_pDerivedClass, void const* a_pInstance) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Casts.
    ///
    /// \param [in,out] a_pClass        The destination class.
    /// \param [in,out] a_pBaseAddress  The instance.
    ///
    /// \return null if it fails, else the adjusted pointer.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void*       cast(Class* a_pBaseOrDerivedClass, void* a_pInstance) const;
    void const* cast(Class* a_pBaseOrDerivedClass, void const* a_pInstance) const;

    template<class T>
    T* cast(void* a_pBaseAddress) const;
    template<class T>
    T const* cast(void const* a_pBaseAddress) const;

    void*       upcast(Type* a_pTarget, void* a_pBaseAddress) const override;
    void*       downcast(Type* a_pTarget, void* a_pBaseAddress) const override;
    void*       cast(Type* a_pTargetType, void* a_pBase) const final;
    void const* cast(Type* a_pTargetType, void const* a_pInstance) const final;

    void* newInstance() const override;
    void  deleteInstance(void* a_pInstance) const override;
    void* placementNewInstance(void* a_pMemory) const override;
    void* placementNewInstance(void* a_pMemory, Constructor* a_pConstructor, void** a_pArgs) const override;
    void  placementDeleteInstance(void* a_pInstance) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the state machine associated to this class.
    ///
    /// \param a_pStateMachine If non-null, the state machine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a meta value by its name, recursively through all the base classes.
    ///
    /// \param  a_strName   The key name.
    ///
    /// \return null String if no meta found, else the meta data value String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Variant& getMetaDataIncludingBases(StringView a_Name) const;
    const Variant& getMetaDataIncludingBases(StringWithHash a_Hash) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets all meta value by name, recursively through all the base classes.
    ///
    /// \param  a_strName   The key name.
    ///
    /// \return null String if no meta found, else the meta data value String.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getMetaDatasIncludingBases(StringView a_Name, Variants& a_MetaDatas) const;
    void getMetaDatasIncludingBases(StringWithHash a_Hash, Variants& a_MetaDatas) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this clss has a meta data with given name through all base classes.
    ///
    /// \param  a_strName   The meta data name.
    ///
    /// \return True is a meta data has been found, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasMetaDataIncludingBases(StringWithHash a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this clss has a meta data with given name through all base classes.
    ///
    /// \param  a_strName   The meta data name.
    ///
    /// \return True is a meta data has been found, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasMetaDataIncludingBases(StringView a_strName) const;

    bool isCopyable() const override;

    bool isCopyConstructible() const override;

    bool isMoveConstructible() const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  operator=
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void copyConstruct(void* a_pDest, void const* a_pSrc) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  operator==
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool equal(void const* a_pInstance0, void const* a_pInstance1) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class provides a new vtable relative to its polymorphic (or not)
    /// inheritance.
    ///
    /// \return true if a new vtable method table is provided, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool hasNewVTable() const;

    bool hasStrongDependencyOnType(Type* a_pType) const override;

    ExtraData* getExtraData() const { return static_cast<ExtraData*>(ClassType::getExtraData()); }

    /// \cond INTERNAL

    void addNewVirtualMethodTable();

    /// \endcond

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial default constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addImplicitDefaultConstructor() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial copy constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addImplicitCopyConstructor() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial copy operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addImplicitCopyAssignmentOperator() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial move constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addImplicitMoveConstructor() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial move operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addImplicitMoveAssignmentOperator() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial destructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addImplicitDestructor() override;

    bool canHaveImplicitDefaultConstructor() const override;
    bool canHaveImplicitCopyConstructor() const override;
    bool canHaveImplicitCopyAssignmentOperator() const override;
    bool canHaveImplicitMoveConstructor() const override;
    bool canHaveImplicitMoveAssignmentOperator() const override;

    static VirtualMethodTable*  CreateVirtualMethodTable(Class* a_pOwner, void** a_ppAddr, size_t a_MethodCount);
    static VirtualMethodTable*  DeriveVirtualMethodTable(Class* a_pOwner, VirtualMethodTable* a_pBase,
                                                         size_t a_MethodCount);
    static VirtualMethodTable*  DeriveVirtualMethodTable(Class* a_pOwner, VirtualMethodTable* a_pBase);
    static Class*               VTablePrimaryClass(VirtualMethodTable* a_pVTable);
    virtual VirtualMethodTable* createVirtualMethodTable();
    virtual VirtualMethodTable* deriveVirtualMethodTable(VirtualMethodTable* a_pVirtualMethodTable);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of extensions.
    ///
    /// \param  a_MetaDatas [in,out] The meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    SymbolExtension* getExtensionCascade(Class* a_pSymbolExtensionClass, size_t a_Num = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Get the list of extensions.
    ///
    /// \param  a_MetaDatas [in,out] The meta data list as a key/value map.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class SymbolExtensionClass>
    SymbolExtensionClass* getExtensionCascade(size_t a_Num = 0) const
    {
        return static_cast<SymbolExtensionClass*>(getExtensionCascade(PHANTOM_CLASSOF(SymbolExtensionClass), a_Num));
    }

    virtual void registerInstance(void* a_pInstance);
    virtual void unregisterInstance(void* a_pInstance);

    InstanceCache* getOrCreateInstanceCache();

protected:
    bool         canBeDestroyed() const override;
    Strings      getBaseClasseNames() const;
    virtual void setBaseClasseNames(Strings table);

private:
    Type* getCommonBaseAncestor(Type* a_pType) const override;
    void  _addBaseClass(Class* a_pBaseClass, size_t a_uiOffset, Access a_Access = Access::Public);
    void  addDerivedClass(Class* a_pType);
#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
    void _addDerivedClassRecursive(Class* a_pType);
    void _removeDerivedClassRecursive(Class* a_pType);
#endif
    void removeDerivedClass(Class* a_pType);
    bool getBaseClassAccess(Class* a_pClass, Access* a_pInheritanceAccess) const;
    bool getBaseClassAccessCascade(Class* a_pClass, Access* a_pInheritanceAccess) const;

protected:
    void _onNativeElementsAccess();
    void _onNativeElementsAccessImpl();

private:
    void _registerKind(void* a_pInstance);
    void _unregisterKind(void* a_pInstance);

private:
    BaseClasses m_BaseClasses;
    Classes     m_DerivedClasses;
#if PHANTOM_CUSTOM_ENABLE_DERIVED_CLASS_CACHE
    Classes m_RecurseDerivedClasses;
#endif
    InstanceCache*                       m_pInstanceCache = nullptr;
    size_t                               m_InstanceCount = 0;
    SmallMap<ValueMember*, Expression*>* m_pOverriddenDefaultExpressions = nullptr;
    mutable VirtualMethodTables          m_VirtualMethodTables;
    void*                                m_pSingleton = nullptr;
    mutable Optional<Subroutine*>        m_OpEquals;
};

/// inlines

inline ptrdiff_t Class::getBaseClassOffset(Class* a_pBaseClass) const
{
    for (auto bc : m_BaseClasses)
    {
        if (bc.baseClass == a_pBaseClass)
            return bc.offset;
    }
    return -1;
}

inline ptrdiff_t Class::getBaseClassOffsetCascade(Class* a_pBaseType) const
{
    if (this == a_pBaseType)
        return 0;
    for (auto bc : m_BaseClasses)
    {
        ptrdiff_t sub_result = bc.baseClass->getBaseClassOffsetCascade(a_pBaseType);
        if (sub_result != -1)
            return sub_result + bc.offset;
    }
    return -1;
}

} // namespace lang
} // namespace phantom
