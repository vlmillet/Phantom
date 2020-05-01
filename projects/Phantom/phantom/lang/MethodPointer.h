// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/MemberPointer.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a member function pointer type.
/// \code{.cpp}
///     ex: int (MyClass*::member_function_pointer_type)(float, int)
/// \endcode
class PHANTOM_EXPORT_PHANTOM MethodPointer : public MemberPointer
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(MethodPointer);

    friend class Type;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a member function pointer type from an owner class type, a signature and
    ///         optional modifiers.
    ///
    /// \param  a_pOwnerClassType   The owner class type.
    /// \param  a_pSignature        The member function signature.
    /// \param  a_Modifiers         (optional) The modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    MethodPointer(ClassType* a_pObjectType, FunctionType* a_pFunctionType, Modifiers a_Modifiers = 0,
                  uint a_uiFlags = 0);

protected: // Native constructor
    /// \internal
    MethodPointer(ClassType* a_pObjectType, FunctionType* a_pFunctionType, size_t a_Size, size_t a_Alignment,
                  Modifiers a_Modifiers, uint a_uiFlags);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if the signature matches the given parameter types and qualifiers (const,
    /// &amp;,
    ///         &amp;&amp; => PHANTOM_R_CONST, PHANTOM_R_LVALUEREF, PHANTOM_R_RVALUEREF).
    ///
    /// \param  a_ParameterTypes    The parameter types.
    /// \param  a_Modifiers        The qualifiers.
    ///
    /// \return true if it matches, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool matches(TypesView parameters, Modifiers a_Modifiers = 0, uint a_uiFlags = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the underlying closure associated with the given member function pointer.
    ///
    /// \param a_pPointer  If non-null, the generic member function pointer.
    ///
    /// \return null if closure couldn't be found, else the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Closure getClosure(void* a_pPointer) const;

    MethodPointer* asMethodPointer() const override { return const_cast<MethodPointer*>(this); }

    void valueFromString(StringView a_str, void* dest) const override;

    void valueToString(StringBuffer& a_Buf, const void* src) const override;

    void valueToLiteral(StringBuffer& a_Buf, const void* src) const override;

    bool isCopyable() const override { return true; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function signature.
    ///
    /// \return The member function signature.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FunctionType* getFunctionType() const { return m_pFunctionType; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the member function return type.
    ///
    /// \return The member function return type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getReturnType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the parameter's type at given index position.
    ///
    /// \param  i   Zero-based index of the parameter.
    ///
    /// \return The parameter type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getParameterType(size_t i) const;

    size_t getParameterTypeCount() const;

    TypesView getParameterTypes() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the implicit object parameter type.
    ///
    /// \return null if it fails, else the implicit object parameter type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getImplicitObjectParameterType() const;

    bool acceptsCallerExpressionType(Type* a_pType) const;

    bool acceptsCallerExpressionQualifiers(Modifiers a_CallerQualifiers) const;

    inline void call(void* a_pPointer, void** a_pArgs) const
    {
        void* pThis = *a_pArgs++; /// 'this' pointer is stored as '*this', a reference, so getting address of
                                  /// argument is getting address of *this (&*this) which is in fact 'this'
        call(a_pPointer, pThis, a_pArgs);
    }
    inline void call(void* a_pPointer, void** a_pArgs, void* a_pReturnAddress) const
    {
        void* pThis = *a_pArgs++; /// 'this' pointer is stored as '*this', a reference, so getting address of
                                  /// argument is getting address of *this (&*this) which is in fact 'this'
        call(a_pPointer, pThis, a_pArgs, a_pReturnAddress);
    }

    /// Non native member function pointers contains the meta member function they point to

    virtual void call(void* a_pPointer, void* a_pThis, void** a_pArgs) const;

    virtual void call(void* a_pPointer, void* a_pThis, void** a_pArgs, void* a_pReturnAddress) const;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;

protected:
    FunctionType* m_pFunctionType;
};

} // namespace lang
} // namespace phantom
