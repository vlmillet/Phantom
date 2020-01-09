// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Type.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief Represents a function function type with return type, parameters and qualifiers.
class PHANTOM_EXPORT_PHANTOM FunctionType : public Type
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    friend class Subroutine;
    friend class Source;

    PHANTOM_DECLARE_META_CLASS(FunctionType);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Construction helper.
    ///
    /// \param  a_Modifiers   (optional) The modifiers.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static FunctionType* Create(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Construction helper.
    ///
    /// \param  a_pReturnType   The return type.
    /// \param  a_Modifiers       (optional) The modifiers.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static FunctionType* Create(Type* a_pReturnType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Construction helper.
    ///
    /// \param  a_pReturnType           The return type.
    /// \param  a_pSingleParameterType  The single parameter type.
    /// \param  a_Modifiers               (optional) The modifiers.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static FunctionType* Create(Type* a_pReturnType, Type* a_pSingleParameterType, Modifiers a_Modifiers = 0,
                                uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Construction helper.
    ///
    /// \param  a_pReturnType           The return type.
    /// \param  a_ParameterTypes        The parameter types.
    /// \param  a_Modifiers             (optional) The modifiers.
    ///
    /// \return null if it fails, else.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    static FunctionType* Create(Type* a_pReturnType, TypesView a_ParameterTypes, Modifiers a_Modifiers = 0,
                                uint a_uiFlags = 0);

    enum EState
    {
        eState_Virgin = 0,
        eState_UnderConstruction,
        eState_Final,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an empty function type with void return type, no parameters and optional
    /// a_Modifiers/qualifiers.
    ///
    /// \param  a_Modifiers   (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an function type with a given return type, no parameters and optional
    /// a_Modifiers/qualifiers.
    ///
    /// \param  a_pReturnType The return type.
    /// \param  a_Modifiers       (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType(Type* a_pReturnType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an function type from a function type name (ex : "void(int, float)") and
    /// optional
    ///         a_Modifiers/qualifiers.
    ///
    /// \param  a_strCode               The function type name.
    /// \param  a_pContextScope         (optional) The parsing context scope (for type names
    /// resolution). \param  a_Modifiers               (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType(StringView a_strCode, LanguageElement* a_pContextScope = nullptr, Modifiers a_Modifiers = 0,
                 uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an function type with a given return type, a single parameter type and
    ///         optional a_Modifiers/qualifiers.
    ///
    /// \param  a_pReturnType           The return type.
    /// \param  a_pSingleParameterType  The single parameter type.
    /// \param  a_Modifiers               (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType(Type* a_pReturnType, Type* a_pSingleParameterType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an function type with a given return type, parameters types and
    ///         optional a_Modifiers/qualifiers.
    ///
    /// \param [in,out] a_pType The return type.
    /// \param  a_Types         The parameters' types.
    /// \param  a_Modifiers       (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType(Type* a_pType, TypesView a_Types, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    PHANTOM_DTOR ~FunctionType() override;

    FunctionType* asFunctionType() const override
    {
        return (FunctionType*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this function type is candidate for Return Value Optimization.
    ///
    /// \return true if rvo candidate, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isRVOCandidate() const;

    bool isSame(Symbol* a_pOther) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this function type is considered equal to another.
    ///
    /// \param [in,out] a_pOther    If non-null, the function type to compare to this object.
    ///
    /// \return true if the objects are considered equal, false if they are not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isSame(FunctionType* a_pOther) const;

    using Type::isSame;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this function type matches the given parameters' type list and optional
    /// a_Modifiers.
    ///
    /// \param  parameterTypes  The parameters' type list.
    /// \param  a_Modifiers     (optional) The modifiers.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matches(TypesView parameterTypes, Modifiers a_Modifiers = 0, uint a_uiFlags = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this function type matches the given other function type.
    ///
    /// \param  a_pOther    The other function type.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matches(FunctionType* a_pOther) const
    {
        return matches(a_pOther->getParameterTypes(), a_pOther->getModifiers());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Parses a function type String (ex: void(int, float)) to extract the return type and
    /// the
    ///         parameters.
    ///
    /// \param  a_strFunctionType  The function type String.
    /// \param  a_pContextScope  (optional) The context scope from where to solve function type
    /// types.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void parse(StringView a_strFunctionType, LanguageElement* a_pContextScope = nullptr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter type count.
    ///
    /// \return The parameter count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getParameterTypeCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter at given index position.
    ///
    /// \param  i   Index of the parameter.
    ///
    /// \return The parameter at given index position.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getParameterType(size_t i) const
    {
        return m_ParameterTypes[i];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a PHANTOM_NEW(parameter) type.
    ///
    /// \param [in,out] a_pParameterType    The parameter's type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addParameterType(Type* a_pType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameters' type list.
    ///
    /// \return The parameters' type list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    TypesView getParameterTypes() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the return type of this function type.
    ///
    /// \param [in,out] a_pType The return type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setReturnType(Type* a_pType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the return type of this function type.
    ///
    /// \return The return type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getReturnType() const;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getUniqueName;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this function type has ellipsis parameter.
    ///
    /// \return true if it has ellipsis, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool hasEllipsis() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the first parameter of this function type.
    ///
    /// \return An iterator pointing to the first parameter of this function type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Types::const_iterator beginParameterTypes() const
    {
        return m_ParameterTypes.begin();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets an iterator pointing to the end of parameters of this function type.
    ///
    /// \return An iterator pointing to the end of parameters of this function type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Types::const_iterator endParameterTypes() const
    {
        return m_ParameterTypes.end();
    }

    Type* addPointer() const override;

private:
    bool separateParameters(StringView a_strText, Strings& a_OutParameters);
    bool parseParameterTypeList(StringView a_strText, Types& a_OutParameterTypes, LanguageElement* a_pContextScope);

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;
    void onElementRemoved(LanguageElement* a_pElement) override;

protected:
    Type* m_pReturnType;
    Types m_ParameterTypes;
};

} // namespace lang
} // namespace phantom
