// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Subroutine.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
class ExecutionContext;

template<class R, class... Args>
struct MethodInvokeH
{
    static R invoke(Method const* a_pMethod, void* a_pThis, Args&&... a_Args);
};

template<class... Args>
struct MethodInvokeH<void, Args...>
{
    static void invoke(Method const* a_pMethod, void* a_pThis, Args&&... a_Args);
};

/// \brief  Represents a member function.
class PHANTOM_EXPORT_PHANTOM Method : public Subroutine
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Method);

    friend class Class;
    friend class ClassType;
    friend class Structure;
    friend class Property;
    friend class VirtualMethodTable;

public:
    Method(ABI a_eABI = ABI::MethodCall, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a member function.
    ///
    /// \param  a_strName               The name.
    /// \param  a_pSignature            The signature.
    /// \param  a_Modifiers             (optional) the modifiers.
    /// \param  a_uiFlags               (optional) the flags.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Method(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    ~Method() override;

    lang::ClassType* getOwnerClassType() const;
    lang::Class*     getOwnerClass() const;

    void invoke(void* a_pObject, void** a_pArgs) const override;
    void invoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress) const override;
    void placementInvoke(void* a_pObject, void** a_pArgs, void* a_pReturnAddress) const override;
    void placementCall(ExecutionContext& a_Context, void** a_pArgs) const override
    {
        Subroutine::placementCall(a_Context, a_pArgs);
    }
    void call(void** a_pArgs) const override
    {
        Subroutine::call(a_pArgs);
    }
    void call(void** a_pArgs, void* a_pReturnAddress) const override
    {
        Subroutine::call(a_pArgs, a_pReturnAddress);
    }

    template<class T, class... Args>
    Variant invokeV(T* a_pThis, Args&&... a_Args) const
    {
        void* addresses[] = {&a_Args..., 0};
        void* pAdjustedThis =
        PHANTOM_TYPEOF(PHANTOM_TYPENAME std::remove_cv<T>::type)->cast(getOwnerClassType(), a_pThis);
        PHANTOM_ASSERT(pAdjustedThis);
        if (getReturnType()->getTypeKind() == TypeKind::Void)
        {
            invoke(pAdjustedThis, addresses);
            return Variant();
        }
        else
        {
            Variant result;
            result.setType(getReturnType());
            invoke(pAdjustedThis, addresses, result.data());
            return result;
        }
    }

    template<class... Args>
    Variant invokeV(void* a_pThis, Args&&... a_Args) const
    {
        void* addresses[] = {&a_Args..., 0};
        if (getReturnType()->getTypeKind() == TypeKind::Void)
        {
            invoke(a_pThis, addresses);
            return Variant();
        }
        else
        {
            Variant result;
            result.setType(getReturnType());
            invoke(a_pThis, addresses, result.data());
            return result;
        }
    }

    template<class R, class T, class... Args>
    R invoke(T* a_pThis, Args&&... a_Args) const
    {
        void* pAdjustedThis =
        PHANTOM_TYPEOF(PHANTOM_TYPENAME std::remove_cv<T>::type)->cast(getOwnerClassType(), a_pThis);
        PHANTOM_ASSERT(pAdjustedThis);
        return MethodInvokeH<R, Args...>::invoke(this, pAdjustedThis, std::forward<Args>(a_Args)...);
    }

    template<class R, class... Args>
    R invoke(void* a_pThis, Args&&... a_Args) const
    {
        return MethodInvokeH<R, Args...>::invoke(this, a_pThis, std::forward<Args>(a_Args)...);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if the current member function can override the given one.
    ///
    /// \param [in,out] a_pMethod   The member function to override.
    ///
    /// \return true if we can override, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool canOverride(Method* a_pMethod) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if the current member function can override the described one.
    ///
    /// \param  a_strName               The name of the member function to override.
    /// \param [in,out] a_pSignature    The signature of the member function to override.
    /// \param  a_Modifiers             The modifiers of the member function to override.
    ///
    /// \return true if we can override, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool canOverride(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if the current member function can be overridden by the given one.
    ///
    /// \param  a_pMethod             The candidate overriding member function.
    ///
    /// \return true if we can override, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isOverridableBy(Method* a_pMethod) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Determine if the current member function can be overridden by the described one.
    ///
    /// \param  a_strName               The name of the member function to override.
    /// \param [in,out] a_pSignature    The signature of the overriding member function.
    /// \param  a_Modifiers             The modifiers of the overriding member function.
    ///
    /// \return true if we can override, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isOverridableBy(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0) const;

    Method* asSlot() const override
    {
        return (((m_Modifiers & PHANTOM_R_SLOT_METHOD) == PHANTOM_R_SLOT_METHOD)) ? const_cast<Method*>(this) : nullptr;
    }
    virtual LanguageElement* asLanguageElement() const
    {
        return const_cast<Method*>(this);
    }
    Subroutine* asSubroutine() const override
    {
        return const_cast<Method*>(this);
    }
    Method* asMethod() const override
    {
        return const_cast<Method*>(this);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the virtual table index in which this member function is referenced.
    ///
    /// \return The virtual table index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    size_t getVirtualTableIndex(size_t a_uiVtableIndex) const;

    /// \brief  Sets this member function 'virtual'.
    void setVirtual();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a closure for vtable and specified 'this' offset (if exists).
    ///
    /// \param  a_uiOffset  The 'this' offset.
    ///
    /// \return null if closure doesn't exist, else the closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* getVTableClosure(size_t a_uiOffset) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets a closure for vtable and specified 'this' offset.
    ///
    /// \param  a_uiOffset  The 'this' offset.
    /// \param  a_pClosure  The closure.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setVTableClosure(size_t a_uiOffset, void* a_pClosure);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  If this member function is an overriden version of a member function, gets the
    /// original (first declared in inheritance tree) versions of this member function.
    ///
    /// \param [in,out] a_Out   The output list of original member functions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getOriginalOverriddenMethods(Methods& a_Out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the property associated with this member function, if any.
    ///
    /// \return null if no property is associated with this member function, else the property.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Property* getProperty() const
    {
        return m_pProperty;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this member function accepts the given caller expression type (check
    /// implicit conversion and const, & and && qualifier compatibilities).
    ///
    /// \param [in,out] The caller expression type to test.
    ///
    /// \return true if it accepts the given caller expression type, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsCallerExpressionType(Type* a_pType) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this member function accepts the given caller expression qualifiers (check
    /// const, & and && qualifier compatibilities).
    ///
    /// \param  a_CallerQualifiers  The caller expression qualifiers to test.
    ///
    /// \return true if it accepts the given caller expression qualifiers, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool acceptsCallerExpressionQualifiers(Modifiers a_CallerQualifiers) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the implicit object parameter type.
    ///
    /// \return null if it fails, else the implicit object parameter type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getImplicitObjectParameterType() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the 'this' local variable.
    ///
    /// \return null if it 'this' not yet setup, else the 'this' local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    LocalVariable* getThis() const;

protected:
    Method(LanguageElement* a_pScope, StringView a_strName, StringView a_strSignature, Modifiers a_Modifiers = 0,
           uint a_uiFlags = 0);
    Method(LanguageElement* a_pScope, StringView a_strName, StringView a_strSignature, ABI a_eABI,
           Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    Method(StringView a_strName, Signature* a_pSignature, ABI a_eABI, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  If this member function is an overriden version of a member function, gets the
    /// original (first declared in inheritance tree) versions of this member function.
    ///
    /// \param [in,out] a_Out   The output list of original member functions.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual size_t computeNativeVirtualIndex() const
    {
        PHANTOM_ASSERT(false, "must be reimplemented in native implementations");
        return ~size_t(0);
    }

    void onAncestorChanged(LanguageElement* a_pAncestor) override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Reimplements this function to provide your own version of the 'this' local variable.
    ///
    /// \return The newly created 'this' local variable.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual LocalVariable* createThis() const;

protected:
    LocalVariable*           m_pThis;
    size_t                   m_uiVirtualTableIndex;
    SmallMap<size_t, void*>* m_pVTableClosures;
    Property*                m_pProperty;

    /// -----------------------
};

template<class R, class... Args>
R MethodInvokeH<R, Args...>::invoke(Method const* a_pMethod, void* a_pThis, Args&&... a_Args)
{
    PHANTOM_ASSERT(a_pMethod->getReturnType()->removeQualifiers() == PHANTOM_TYPEOF(R));
    void* addresses[] = {((void*)(&a_Args))..., 0};
    R     result;
    a_pMethod->invoke(a_pThis, addresses, &result);
    return result;
}

template<class... Args>
void MethodInvokeH<void, Args...>::invoke(Method const* a_pMethod, void* a_pThis, Args&&... a_Args)
{
    PHANTOM_ASSERT(a_pMethod->getReturnType()->getTypeKind() == TypeKind::Void);
    void* addresses[] = {((void*)(&a_Args))..., 0};
    a_pMethod->invoke(a_pThis, addresses);
}

} // namespace lang
} // namespace phantom
