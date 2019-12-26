// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include "Aggregate.h"

#include <phantom/Delegate.h>
#include <phantom/reflection/Scope.h>
#include <phantom/reflection/Type.h>
#include <phantom/thread/RecursiveSpinMutex.h>
#include <phantom/traits/IsTypeDefined.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
class ModuleRegistrationInfo;

template<class Sign>
struct FunctionTypeH;

template<class R, class... Params>
struct FunctionTypeH<R(Params...)>
{
    static Type* ReadReturnTypeAndParameterTypes(Types& a_Params)
    {
        a_Params = {phantom::TypeOf<Params>(phantom::TypeOfTag<Params>())...};
        return phantom::TypeOf<R>(phantom::TypeOfTag<R>());
    }
};

class PHANTOM_EXPORT_PHANTOM ClassType : public Type, public Scope, public Aggregate
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ClassType);

public:
    struct ExtraData
    {
        virtual void PHANTOM_CUSTOM_VIRTUAL_DELETE();
        ExtraData() : m_uiDataTypeCount(0)
        {
        }
        Type::AlignmentComputer m_AlignmentComputer;
        size_t                  m_uiDataTypeCount;
        uint                    m_BuildState = 0;
    };

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor for custom derived class types.
    ///
    /// \param  a_eTypeKind               Meta type id.
    /// \param  a_pExtraData            Extra Data for runtime compilation.
    /// \param  a_Modifiers             (optional) the type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType(TypeKind a_eTypeKind, ExtraData* a_pExtraData, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor for custom derived class types.
    ///
    /// \param  a_eTypeKind               Meta type id.
    /// \param  a_pExtraData            Extra Data for runtime compilation.
    /// \param  a_strName               The type name.
    /// \param  a_Modifiers             (optional) the type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType(TypeKind a_eTypeKind, ExtraData* a_pExtraData, StringView a_strName, Modifiers a_Modifiers = 0,
              uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor for native derived class types. (shouldn't be called directly).
    ///
    /// \param  a_eTypeKind       Meta type id.
    /// \param  a_strName       The type name.
    /// \param  a_uiSize        The type size.
    /// \param  a_uiAlignment   The type alignment.
    /// \param  a_Modifiers     (optional) the type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
              uint a_uiFlags);

public:
    PHANTOM_DTOR ~ClassType() override;

    bool isCopyable() const override;

    bool isCopyAssignable() const override;

    bool isCopyConstructible() const override;

    bool isMoveable() const override;

    bool isMoveAssignable() const override;

    bool isMoveConstructible() const override;

    bool isDefaultInstanciable() const override;

    bool isDefaultConstructible() const override;

    ClassType* asClassType() const override
    {
        return const_cast<ClassType*>(this);
    }
    Class* asClass() const override
    {
        return nullptr;
    }

    Scope* asScope() const override
    {
        return const_cast<ClassType*>(this);
    }

    virtual bool isListInitializable() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets default access specifier.
    ///
    /// \param  a_Specifier    The access specifier.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDefaultAccess(Access a_eAccess);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets default access.
    ///
    /// \return The default access.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Access getDefaultAccess() const
    {
        return m_DefaultAccess;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a constructor.
    ///
    /// \param  a_pConstructor  The constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addConstructor(Constructor* a_pConstructor);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a PHANTOM_NEW(constructor) instantiated from given parameters String. example :
    ///
    ///         \code{.cpp}
    ///         addConstructor("int, size_t");
    ///         \endcode.
    ///
    /// \param  a_strParametersString   The parameters String.
    ///
    /// \return null if it fails, else the instantiated constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Constructor* addConstructor(StringView a_strParametersString);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a PHANTOM_NEW(constructor) instantiated from given parameters and modifiers.
    ///
    /// \param  a_Parameters    Options for controlling the operation.
    /// \param  a_Modifiers     (optional) the modifiers.
    ///
    /// \return null if it fails, else the instantiated constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* addConstructor(const Parameters& a_Parameters, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a PHANTOM_NEW(default) constructor instantiated with given modifiers.
    ///
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if it fails, else  the instantiated constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* addConstructor(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a constructor with given single parameter type and modifiers.
    ///
    /// \param [in,out] a_pSingleParameterType  The ype of the single parameter.
    /// \param  a_Modifiers                     The modifiers.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* addConstructor(Type* a_pSingleParameterType, Modifiers a_Modifiers, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a PHANTOM_NEW(destructor) instantiated with given modifiers.
    ///
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if it fails, else  the instantiated destructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* addDestructor(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a value member.
    ///
    /// \param [in,out] a_pValueMember  The value member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addValueMember(ValueMember* a_pValueMember);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a static Data member.
    ///
    /// \param [in,out] a_pField   The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addStaticField(StaticField* a_pField);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a static Data member.
    ///
    /// \param [in,out] a_pField   The static Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addField(StaticField* a_pField)
    {
        addStaticField(a_pField);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a Data member.
    ///
    /// \param [in,out] a_pField   The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addField(Field* a_pField);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a Data member from a value type, a name and optionals modifiers and flags.
    ///
    /// \param a_pValueType     The value type.
    /// \param a_strName        The Data member name.
    /// \param a_Modifiers      (optional) The modifiers.
    /// \param a_uiFlags        (optional) The flags.
    ///
    /// \return the PHANTOM_NEW(Data) member or null if the Data member cannot be created or added
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* addField(Type* a_pValueType, StringView a_strName, uint a_uiFilterFlag, Modifiers a_Modifiers = 0,
                    uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a property.
    ///
    /// \param [in,out] a_pProperty The property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addProperty(Property* a_pProperty);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a new property instantiated from name and database mask.
    ///
    /// \param  a_strName               The name.
    /// \param  a_uiFilterMask   (optional) the database mask.
    ///
    /// \return null if it fails, else the instantiated property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Property* addProperty(StringView a_strName, uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a new functor property based on two functors.
    ///
    /// \param  a_strName               The name.
    /// \param  a_uiFilterMask   (optional) the database mask.
    ///
    /// \return null if it fails, else the instantiated property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class ClassT, class ValueT, class GetFunctor, class SetFunctor, class DontTouchThis = FunctorProperty>
    FunctorProperty* addFunctorProperty(StringView a_strName, GetFunctor&& a_Get, SetFunctor&& a_Set,
                                        uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY);

    template<class ClassT, class ValueT, class GetFunctor, class SetFunctor, class DontTouchThis = FunctorProperty>
    FunctorProperty* addFunctorProperty(StringView a_strName, GetFunctor&& a_Get,
                                        uint a_uiFilterMask = PHANTOM_R_FILTER_PROPERTY);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds an instance member function.
    ///
    /// \param [in,out] a_pMethod   The member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addMethod(Method* a_pMethod);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a static member function.
    ///
    /// \param [in,out] a_pMethod   The member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addMethod(StaticMethod* a_pMethod)
    {
        addStaticMethod(a_pMethod);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a static member function.
    ///
    /// \param [in,out] a_pStaticMethod The static member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addStaticMethod(StaticMethod* a_pStaticMethod);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a subroutine.
    ///
    /// \param [in,out] a_pSubroutine   The subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addSubroutine(Subroutine* a_pSubroutine) override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a constructor.
    ///
    /// \param  a_pConstructor  The constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeConstructor(Constructor* a_pConstructor);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a value member.
    ///
    /// \param [in,out] a_pValueMember  The value member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeValueMember(ValueMember* a_pValueMember);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a static Data member.
    ///
    /// \param [in,out] a_pField   The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeStaticField(StaticField* a_pField);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a Data member.
    ///
    /// \param [in,out] a_pField   The Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeField(Field* a_pField);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a property.
    ///
    /// \param [in,out] a_pProperty The property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeProperty(Property* a_pProperty);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a member function.
    ///
    /// \param [in,out] a_pMethod    The member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeMethod(Method* a_pMethod);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a static member function.
    ///
    /// \param [in,out] a_pMethod    The member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeStaticMethod(StaticMethod* a_pStaticMethod);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default constructor if matching the given modifiers.
    ///
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if no default constructor defined or not matching the modifiers, else the
    /// default constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getDefaultConstructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the copy constructor if matching the given modifiers.
    ///
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if no copy constructor defined or not matching the modifiers, else the copy
    /// constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getCopyConstructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the move constructor if matching the given modifiers.
    ///
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if no move constructor defined or not matching the modifiers, else the move
    /// constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getMoveConstructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a constructor matching given parameter types.
    ///
    /// \param  a_Types     The types.
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if no constructor found, else the constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getConstructor(TypesView a_Types) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor matching the given parameters String.
    ///
    ///         \code{.cpp}
    ///                    getConstructor("const MyClassType&"); // copy constructor
    ///                    getConstructor(""); // default constructor
    ///         \endcode
    ///
    ///
    /// \param  a_strParameterString  The parameters String.
    ///
    /// \return null if no constructor found, else the constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getConstructor(StringView a_strParameterString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constructor at the given index.
    ///
    /// \param  a_uiIndex  The constructor index.
    ///
    /// \return the constructor at the given index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getConstructor(size_t a_uiIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a constructor with the given single parameter type.
    ///
    /// \param  a_pType  The single parameter type.
    ///
    /// \return If any, returns the constructor matching the given single parameter type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constructor* getConstructor(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a conversion function to the given type.
    ///
    /// \param  a_pType  The type of the conversion function.
    ///
    /// \return If any, returns the conversion function matching the given type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getConversionFunction(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the copy assignment operator.
    ///
    /// \return null if no copy constructor defined or not matching the modifiers, else the copy
    /// constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getCopyAssignmentOperator() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the move assignment operator.
    ///
    /// \return null if no move constructor defined or not matching the modifiers, else the move
    /// constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMoveAssignmentOperator() const;

    void getFullConversionTypes(Types& out, bool a_bImplicits = true) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the destructor.
    ///
    /// \return null if it fails, else the destructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Destructor* getDestructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value member (Data member, property or custom) matching the given name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if no value member found, else the value member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ValueMember* getValueMember(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value members list.
    ///
    /// \return the value members list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const ValueMembers& getValueMembers() const
    {
        return *m_ValueMembers;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member at given index.
    ///
    /// \param  a_uiIndex   The Data member index.
    ///
    /// \return The Data member at given index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getField(size_t a_uiIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member matching the given name.
    ///
    /// \param  a_strName   The Data member name.
    ///
    /// \return null if no Data member found, else the Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getField(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member list.
    ///
    /// \return the Data member list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Fields& getFields() const
    {
        return *m_Fields;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member list (including the one in anonymous sections).
    ///
    /// \return the Data member list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getAllFields(Fields& a_Out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data elements list (a Data element is either a Data member, either an
    /// anonymous struct or union).
    ///
    /// \return the Data elements list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const DataElements& getDataElements() const
    {
        *m_Fields; /* to force member registration */
        return m_DataElements;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the index of the given field.
    ///
    /// \return The index of the given field.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getFieldIndex(Field* a_pField) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member matching the given offset from base address.
    ///
    /// \param  a_uiOffset  The offset from base address.
    ///
    /// \return null if no Data member found, else the Data member by offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Field* getFieldByOffset(size_t a_uiOffset) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member matching the given offset from base address.
    ///
    /// \param  a_uiOffset  The offset from base address.
    ///
    /// \return null if no Data member found, else the Data member by offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Field* getFieldAtOffset(size_t a_uiOffset) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member starting at the given Data member address relative to the given
    /// base address .
    ///
    /// \param  a_pBaseAddress          The base address.
    /// \param  a_pFieldAddress    The Data member address.
    ///
    /// \return null if no Data member found, else the Data member at address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getFieldByAddress(const void* a_pBaseAddress, const void* a_pAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the Data member including the given Data member address relative to the given
    /// base address.
    ///
    /// \param  a_pBaseAddress          The base address.
    /// \param  a_pFieldAddress    The Data member address.
    ///
    /// \return null if no Data member found, else the Data member at address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Field* getFieldAtAddress(const void* a_pBaseAddress, const void* a_pAddress) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the static Data member matching the given name.
    ///
    /// \param  a_strName   The static Data member name.
    ///
    /// \return null if no static Data member found, else the static Data member.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StaticField* getStaticField(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the property at given index.
    ///
    /// \param  a_uiIndex   The property index.
    ///
    /// \return The property at given index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property* getProperty(size_t a_uiIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the property matching the given name.
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if no property found, else the property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property* getProperty(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the field list.
    ///
    /// \return the field list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Properties& getProperties() const
    {
        return *m_Properties;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine matching the given signature name.
    ///
    ///         \code{.cpp}
    ///                    getSubroutine("static int myStaticMethod(float f, int i)"); // retrieves
    ///                    a C++ static member function getSubroutine("MyClassType(const
    ///                    MyClassType&)"); // retrieves a C++ copy constructor
    ///         \endcode
    ///
    /// \param  a_strSignatureName  The signature name.
    ///
    /// \return null if no subroutine found, else the subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine matching the given name, parameter types and optional
    /// qualifiers.
    ///
    /// \param  a_strName           The name.
    /// \param  a_ParameterTypes    The parameter types.
    /// \param  a_Modifiers        (optional) the qualifiers.
    ///
    /// \return null if it fails, else the subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getSubroutine(StringView a_strName, TypesView a_Types, Modifiers a_Modifiers = 0) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the method matching the given signature name.
    ///
    /// \param  a_strDecoratedName  The signature name.
    ///
    /// \return null if no method found, else the member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class Sign>
    Method* getMethod(StringView a_strDecoratedName, Modifiers a_Qualifiers = Modifiers()) const
    {
        Types paramTypes;
        Type* pReturnType = FunctionTypeH<Sign>::ReadReturnTypeAndParameterTypes(paramTypes);
        return getMethod(pReturnType, a_strDecoratedName, paramTypes, a_Qualifiers);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the method matching the given signature name.
    ///
    /// \param  a_strDecoratedName  The signature name.
    ///
    /// \return null if no method found, else the member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethod(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the method matching the given name, parameter types and optional
    ///         qualifiers..
    ///
    /// \param  a_strName           The name.
    /// \param  a_ParameterTypes    The parameter types.
    /// \param  a_Qualifiers        (optional) the qualifiers.
    ///
    /// \return null if it fails, else the member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethod(StringView a_strName, TypesView a_Types, Modifiers a_Qualifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the method matching the given name, parameter types and optional
    ///         qualifiers..
    ///
    /// \param  a_pReturnType       The return type.
    /// \param  a_strName           The name.
    /// \param  a_ParameterTypes    The parameter types.
    /// \param  a_Qualifiers        (optional) the qualifiers.
    ///
    /// \return null if it fails, else the member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getMethod(Type* a_pReturnType, StringView a_strName, TypesView a_ParameterTypes,
                      Modifiers a_Qualifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the method matching the given signature name.
    ///
    /// \param  a_strDecoratedName  The signature name.
    ///
    /// \return null if no method found, else the member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Method* getMethodByPtr(void* a_ppMethodPtr) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the method list.
    ///
    /// \return the method list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Methods& getMethods() const
    {
        return *m_Methods;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the static method matching the given signature name.
    ///
    /// \param  a_strSignatureName  The signature name.
    ///
    /// \return null if no static method found, else the static member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StaticMethod* getStaticMethod(StringView a_strIdentifierString) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the static method matching the given name and parameter types.
    ///
    /// \param  a_strName           The name.
    /// \param  a_ParameterTypes    The parameter types.
    ///
    /// \return null if no static method found, else the static member function.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StaticMethod* getStaticMethod(StringView a_strName, TypesView a_Types) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets unique method with given name. If multiple method are found with same name,
    /// null is returned
    ///
    /// \param  a_strName   The name.
    ///
    /// \return null if none or more than one method found matching the given name, else the unique
    /// method with name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method* getUniqueMethodWithName(StringView a_strName) const;

    void findPublicValueMembersPointingValueType(Type* a_pType, ValueMembers& out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the first constructor.
    ///
    /// \return An iterator pointing to the first constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    const Constructors& getConstructors() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this class type is a template specialization and matches the given template
    ///         arguments.
    ///
    /// \param  a_TemplateArguments The template arguments.
    ///
    /// \return true if matches the template arguments, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matchesTemplateArguments(const LanguageElements& a_Elements) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a member anonymous section.
    ///
    /// \param a_pMemberAnonymousSection   If non-null, the member anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a member anonymous section to 'a_Modifiers'.
    ///
    /// \param  a_strCode   The C++ code to parse and generate anonymous structs and unions.
    /// \param  a_Modifiers (optional) the modifiers.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual MemberAnonymousSection* addMemberAnonymousSection(StringView a_strCode, Modifiers a_Modifiers = 0,
                                                              uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes the given member anonymous section.
    ///
    /// \param [in,out] a_pMemberAnonymousSection   If non-null, the member anonymous section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void removeMemberAnonymousSection(MemberAnonymousSection* a_pMemberAnonymousSection);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member anonymous sections of this class type.
    ///
    /// \return The member anonymous sections.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MemberAnonymousSections const& getMemberAnonymousSections() const
    {
        return *m_MemberAnonymousSections;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Accepts subroutine.
    ///
    /// \param  a_pReturnType                   The subroutine return type.
    /// \param  a_strName                       The subroutine name.
    /// \param  a_Types                         The subroutine parameter types.
    /// \param  a_Modifiers                     The subroutine modifiers.
    /// \param  a_uiFlags                       The flags.
    /// \param  a_pOutConflictingSubroutines    (optional) If non-null and subroutine not accepted,
    ///                                         will contain the conflicting subroutines which avoid
    ///                                         the subroutine to be accepted. subroutines.
    ///
    /// \return true if it succeeds, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsSubroutine(Type* a_pReturnType, StringView a_strName, TypesView a_Types, Modifiers a_Modifiers,
                           uint a_uiFlags, Subroutines* a_pOutConflictingSubroutines = nullptr) const override;

    bool acceptsSubroutine(Subroutine* a_pSubroutine, Subroutines* a_pOutConflictingSubroutines = nullptr) const
    {
        return Scope::acceptsSubroutine(a_pSubroutine, a_pOutConflictingSubroutines);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this class type can have an implicit default constructor.
    ///
    /// \return true if this class type can have trivial default constructor, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canHaveImplicitDefaultConstructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this class type can have an implicit copy constructor.
    ///
    /// \return true if this class type can have a trivial copy constructor, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canHaveImplicitCopyConstructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this class type can have an implicit copy operator.
    ///
    /// \return true if this class type can have a trivial copy operator, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canHaveImplicitCopyAssignmentOperator() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this class type can have an implicit move constructor.
    ///
    /// \return true if this class type can have a trivial move constructor, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canHaveImplicitMoveConstructor() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if this class type can have an implicit move operator.
    ///
    /// \return true if this class type can have a trivial move operator, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool canHaveImplicitMoveAssignmentOperator() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial default constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addImplicitDefaultConstructor();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial copy constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addImplicitCopyConstructor();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial copy operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addImplicitCopyAssignmentOperator();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial move constructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addImplicitMoveConstructor();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial move operator.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addImplicitMoveAssignmentOperator();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a trivial destructor.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void addImplicitDestructor();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// reimplemented
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* allocate() const override;
    void* allocate(size_t n) const override;
    void  construct(void* a_pInstance) const override;
    void  destroy(void* a_pInstance) const override;

    void*         newInstance() const override;
    void*         placementNewInstance(void*) const override;
    virtual void* placementNewInstance(void*, Constructor* a_pConstructor, void** a_pArgs = NULL) const;
    void          deleteInstance(void* a_pInstance) const override;
    void          placementDeleteInstance(void* a_pInstance) const override;

    void copyConstruct(void* a_pDest, void const* a_pSrc) const override;
    void moveConstruct(void* a_pDest, void* a_pSrc) const override;
    void copyAssign(void* a_pDest, void const* a_pSrc) const override;
    void moveAssign(void* a_pDest, void* a_pSrc) const override;

    using Type::fetchElements;
    void fetchElements(LanguageElements& out, Class* a_pClass = nullptr) const override;

    ExtraData* getExtraData() const
    {
        return m_pExtraData;
    }

    Template* getTemplate() const;

    bool                    isTemplateInstance() const override;
    TemplateSpecialization* getTemplateSpecialization() const
    {
        return Type::getTemplateSpecialization();
    }
    TemplateSpecialization* getTemplateSpecialization(StringView              a_strTemplateName,
                                                      const LanguageElements& a_Arguments) const
    {
        return Scope::getTemplateSpecialization(a_strTemplateName, a_Arguments);
    }

    void addFriend(Symbol* a_pFriend);
    bool hasFriend(Symbol* a_pFriend) const override;

    void checkCompleteness() const override;

    bool isSame(Symbol* a_pSymbol) const override
    {
        return Type::isSame(a_pSymbol);
    }
    bool isSame(LanguageElement* a_pElement) const override
    {
        return Type::isSame(a_pElement);
    }

    bool hasStrongDependencyOnType(Type* a_pType) const override;

    virtual void finalizeNative()
    {
        PHANTOM_ASSERT_NO_IMPL();
    }

    void getFields(AggregateFields& _fields) const override;
    void getFlattenedAggregateFields(AggregateFields& _aggregateFields) const override;

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;

    bool canBeDestroyed() const override;
    void onElementAdded(LanguageElement* a_pElement) override;
    void onElementRemoved(LanguageElement* a_pElement) override;

    void addScopeElement(Symbol* a_pElement) override
    {
        addElement(a_pElement);
    }
    void removeScopeElement(Symbol* a_pElement) override
    {
        removeElement(a_pElement);
    }
    void onElementsAccess() override;

protected:
    friend class ModuleRegistrationInfo;
    void _onNativeElementsAccess();     ///< @off
    void _onNativeElementsAccessImpl(); ///< @off

private:
    typedef Members<ValueMembers>            ValueMembersList;
    typedef Members<Properties>              PropertiesList;
    typedef Members<Fields>                  FieldsList;
    typedef Members<Methods>                 MethodsList;
    typedef Members<MemberAnonymousSections> MemberAnonymousSectionsList;
    typedef Members<Symbols>                 FriendsList;
    typedef Members<Constructors>            ConstructorsList;

    ValueMembersList            m_ValueMembers{this};
    PropertiesList              m_Properties{this};
    FieldsList                  m_Fields{this};
    MethodsList                 m_Methods{this};
    FriendsList                 m_Friends{this};
    ConstructorsList            m_Constructors{this};
    DataElements                m_DataElements;
    MemberAnonymousSectionsList m_MemberAnonymousSections{this};
    Access                      m_DefaultAccess = Access::Public;
    ExtraData*                  m_pExtraData = nullptr;

private:
    friend class Class;
    phantom::TypeInstallationDelegate m_OnDemandMembersFunc;
    RecursiveSpinMutex                m_OnDemandMutex;
};

template<class ClassT, class ValueT, class GetFunctor, class SetFunctor, class DontTouchThis>
FunctorProperty* ClassType::addFunctorProperty(StringView a_strName, GetFunctor&& a_Get, SetFunctor&& a_Set,
                                               uint a_uiFilterMask)
{
    PHANTOM_STATIC_ASSERT(phantom::IsTypeDefined<DontTouchThis>::value,
                          "missing #include <phantom/reflection/FunctorProperty.h");
    PHANTOM_ASSERT(static_cast<ClassType*>(PHANTOM_TYPEOF(ClassT)) == this);
    auto pFProp = ::phantom::detail::NewH<DontTouchThis>() *
    new (PHANTOM_ALLOCATE(DontTouchThis)) DontTouchThis(
                  PHANTOM_TYPEOF(ValueT), a_strName,
                  [=](void const* a_pInstance, void* a_pValue) {
                      a_Get(reinterpret_cast<ClassT const*>(a_pInstance), reinterpret_cast<ValueT*>(a_pValue));
                  },
                  [=](void* a_pInstance, void const* a_pValue) {
                      a_Set(reinterpret_cast<ClassT*>(a_pInstance), reinterpret_cast<ValueT const*>(a_pValue));
                  },
                  a_uiFilterMask);
    pFProp->setFlag(getFlags() & PHANTOM_R_FLAG_NATIVE);
    addValueMember(pFProp);
    return pFProp;
}

template<class ClassT, class ValueT, class GetFunctor, class SetFunctor, class DontTouchThis>
FunctorProperty* ClassType::addFunctorProperty(StringView a_strName, GetFunctor&& a_Get, uint a_uiFilterMask)
{
    PHANTOM_STATIC_ASSERT(phantom::IsTypeDefined<DontTouchThis>::value,
                          "missing #include <phantom/reflection/FunctorProperty.h");
    PHANTOM_ASSERT(static_cast<ClassType*>(PHANTOM_TYPEOF(ClassT)) == this);
    auto pFProp = ::phantom::detail::NewH<DontTouchThis>() *
    new (PHANTOM_ALLOCATE(DontTouchThis)) DontTouchThis(
                  PHANTOM_TYPEOF(ValueT), a_strName,
                  [=](void const* a_pInstance, void* a_pValue) {
                      a_Get(reinterpret_cast<ClassT const*>(a_pInstance), reinterpret_cast<ValueT*>(a_pValue));
                  },
                  a_uiFilterMask);
    pFProp->setFlag(getFlags() & PHANTOM_R_FLAG_NATIVE);
    addValueMember(pFProp);
    return pFProp;
}

} // namespace reflection
} // namespace phantom
