// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
#include <phantom/utils/ArrayView.h>
#include <phantom/utils/StringViews.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief Represents a function signature with return type, parameters and qualifiers.
class PHANTOM_EXPORT_PHANTOM Signature : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Signature);

    friend class Subroutine;
    friend class Source;

public:
    enum EState
    {
        eState_Virgin = 0,
        eState_UnderConstruction,
        eState_Final,
    };

    static Signature* Create(LanguageElement* a_pOwner, Type* a_pRet, TypesView a_ParamTs, Modifiers a_Modifiers = 0,
                             uint a_uiFlags = 0);

    static Signature* Create(LanguageElement* a_pOwner, Type* a_pRet, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an empty signature with void return type, no parameters and optional
    /// a_Modifiers/qualifiers.
    ///
    /// \param  a_Modifiers   (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Signature(Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an signature with a given return type, no parameters and optional
    /// a_Modifiers/qualifiers.
    ///
    /// \param  a_pType The return type.
    /// \param  a_Modifiers       (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Signature(Type* a_pReturnType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an signature with a given return type, a list of parameters and
    ///         optional a_Modifiers/qualifiers.
    ///
    /// \param  a_pType The return type.
    /// \param  a_Parameters    The list of parameters
    /// \param  a_Modifiers       (optional) the a_Modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Signature(Type* a_pType, const Parameters& a_Parameters, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    void initialize();

    Signature* asSignature() const override { return (Signature*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this signature is candidate for Return Value Optimization.
    ///
    /// \return true if rvo candidate, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isRVOCandidate() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this signature has variadic arguments.
    ///
    /// \return true if has variadic arguments, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isVariadic() const { return m_bVariadic; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parenthesized list of parameter types as a String using unique names.
    ///
    /// \return The parenthesized list of parameter types as a String using unique names.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getUniqueParameterTypeNameList(StringBuffer& a_Buf) const;

    bool isSame(Symbol* a_pOther) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this signature is considered equal to another.
    ///
    /// \param [in,out] a_pOther    If non-null, the signature to compare to this object.
    ///
    /// \return true if the objects are considered equal, false if they are not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
    bool isSame(Signature* a_pOther) const;
#pragma clang diagnostic pop

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this signature matches the given parameters' type list and optional
    /// a_Modifiers.
    ///
    /// \param  a_ParameterTypes    The parameters' type list (a nullptr in the list is considered
    /// as "any type"). \param  a_Modifiers         (optional) The modifiers. \param  a_uiFlags
    /// (optional) The flags.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matches(TypesView a_ParameterTypes, Modifiers a_Modifiers = Modifiers()) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if this signature matches the given other signature.
    ///
    /// \param  a_pOther    The other signature.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matches(Signature* a_pOther) const { return matches(a_pOther->getParameterTypes(), a_pOther->getModifiers()); }

    Signature* clone(LanguageElement* a_pOwner) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter count.
    ///
    /// \return The parameter count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getParameterCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter at given index position.
    ///
    /// \param  i   Index of the parameter.
    ///
    /// \return The parameter at given index position.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parameter* getParameter(size_t i) const { return m_Parameters[i]; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter matching the given name.
    ///
    /// \param  a_strName   The parameter name.
    ///
    /// \return null is no parameter found, else the parameter matching the given name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parameter* getParameter(StringView a_strName) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type of the parameter at given index position.
    ///
    /// \param  a_uiParamIndex  Index of the parameter.
    ///
    /// \return The parameter's type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getParameterType(size_t a_uiParamIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameters' type list.
    ///
    /// \return The parameters' type list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Types getParameterTypes() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameters' list.
    ///
    /// \return The parameters' list.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Parameters const& getParameters() const { return m_Parameters; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the name of the parameter at given index position.
    ///
    /// \param  a_uiParamIndex  Index of the parameter.
    ///
    /// \return The parameter's name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringView getParameterName(size_t a_uiParamIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default value expression of the parameter at given index position.
    ///
    /// \param  a_uiParamIndex  Index of the parameter.
    ///
    /// \return The parameter's default value expression.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Expression* getParameterDefaultValueExpression(size_t a_uiParamIndex) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the return type of this signature.
    ///
    /// \return The return type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getReturnType() const;

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getRelativeDecoratedName;
    using LanguageElement::getUniqueName;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void getUniqueName(StringBuffer& a_Buf) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the minimum needed argument count (= all - default).
    ///
    /// \return The minimum needed argument count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    size_t getRequiredArgumentCount() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the subroutine which owns this signature.
    ///
    /// \return The owner subroutine.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Subroutine* getOwnerSubroutine() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds default arguments to this signature.
    ///
    /// \param  a_DefaultArgExpressions    The default arguments as an array view of string views
    ///                                    (be careful if you use this, no string view will be
    ///                                    stored as string, they but be allocated on your side).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setNativeDefaultArgumentStrings(ArrayView<StringView> a_DefaultArgs);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  collects the native default arguments of this signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    StringViews getNativeDefaultArgumentStrings() const;

    FunctionType* getFunctionType() const;

    void setVariadic()
    {
        PHANTOM_ASSERT(!m_bVariadic);
        m_bVariadic = true;
    }

private:
    bool separateParameters(StringView a_strText, Strings& a_OutParameters);
    bool parseParameterTypeList(StringView a_strText, LanguageElement* a_pContextScope);

protected:
    Type*      m_pReturnType;
    Parameters m_Parameters;
    bool       m_bVariadic = false;
};

} // namespace lang
} // namespace phantom
