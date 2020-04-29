// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/detail/StringConverter.h>
#include <phantom/lang/ClassType.h>
#include <phantom/lang/Constructor.h>
#include <phantom/lang/ConstructorT.h>
#include <phantom/lang/DestructorT.h>
#include <phantom/lang/MetaNewDelete.h>
#include <phantom/lang/MethodT.h>
#include <phantom/lang/TypeT.h>

#define _PHNTM_do_not_declare_trivials_text                                                                            \
    "do not declare lang for default or trivial constructors/operators or destructors, it "                            \
    "is automatic because required by the lang system"

namespace phantom
{
namespace lang
{
namespace detail
{
template<typename t_Ty, bool t_is_not_pod_class_default_constructible_and_not_abstract>
struct DefaultCtorProviderH
{
    static void apply(ClassType* a_pType, StringView a_strName)
    {
        if (a_pType->getDefaultConstructor() == nullptr)
        {
            auto pCtor = a_pType->NewMeta<ConstructorT<t_Ty, void()>>(
            a_strName, Signature::Create(PHANTOM_TYPEOF(void), PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE), PHANTOM_R_NONE,
            PHANTOM_R_FLAG_IMPLICIT);
            pCtor->setAccess(Access::Public);
            a_pType->addConstructor(pCtor);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getDefaultConstructor()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
};

template<typename t_Ty>
struct DefaultCtorProviderH<t_Ty, false>
{
    static void apply(Type*, StringView) {}
};

template<typename t_Ty, bool t_is_notstructure_copy_constructible>
struct DefaultCopyCtorProviderH
{
    static void apply(ClassType* a_pType, StringView a_strName)
    {
        if (a_pType->getCopyConstructor() == nullptr)
        {
            auto pCtor = NewMeta<ConstructorT<t_Ty, void(const t_Ty&)>>(
            a_strName,
            Signature::Create(PHANTOM_TYPEOF(void), a_pType->addConst()->addLValueReference(), PHANTOM_R_NONE,
                              PHANTOM_R_FLAG_NATIVE),
            PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
            a_pType->addConstructor(pCtor);
            pCtor->setAccess(Access::Public);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getCopyConstructor()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
};

template<typename t_Ty>
struct DefaultCopyCtorProviderH<t_Ty, false>
{
    static void apply(Type*, StringView) {}
};

template<typename t_Ty, bool t_is_notstructure_copy_constructible>
struct DefaultCopyAssignOpProviderH
{
#if PHANTOM_CUSTOM_USE_PRECISE_SIGNATURE
    static void apply(ClassType* a_pType)
    {
        if (a_pType->getCopyAssignmentOperator() == nullptr)
        {
            auto pFunc = NewMeta<MethodT<t_Ty, t_Ty& (t_Ty::*)(const t_Ty&)>>(
            "operator=",
            Signature::Create(a_pType->addLValueReference(), a_pType->addConst()->addLValueReference(), PHANTOM_R_NONE,
                              PHANTOM_R_FLAG_NATIVE),
            &t_Ty::operator=);
            pFunc->setAccess(Access::Public);
            a_pType->addMethod(pFunc);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getCopyAssignmentOperator()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
#else
    static void apply(ClassType* a_pType)
    {
        t_Ty& (t_Ty::*mf)(const t_Ty&) = &t_Ty::operator=;
        if (a_pType->getCopyAssignmentOperator() == nullptr)
        {
            auto pFunc = NewMeta<MethodT<int& (DummyClass::*)(int&)>>(
            "operator=",
            Signature::Create(a_pType->addLValueReference(), a_pType->addConst()->addLValueReference(), PHANTOM_R_NONE,
                              PHANTOM_R_FLAG_NATIVE),
            (int& (DummyClass::*)(int&))mf, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
            pFunc->setAccess(Access::Public);
            a_pType->addMethod(pFunc);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getCopyAssignmentOperator()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
#endif
};

template<typename t_Ty>
struct DefaultCopyAssignOpProviderH<t_Ty, false>
{
    static void apply(Type*) {}
};

template<typename t_Ty, bool t_is_notstructure_move_constructible>
struct DefaultMoveCtorProviderH
{
    static void apply(ClassType* a_pType, StringView a_strName)
    {
        if (a_pType->getMoveConstructor() == nullptr)
        {
            auto pCtor = NewMeta<ConstructorT<t_Ty, void(t_Ty &&)>>(
            a_strName,
            Signature::Create(PHANTOM_TYPEOF(void), a_pType->addRValueReference(), Modifiers(0), PHANTOM_R_FLAG_NATIVE),
            PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
            pCtor->setAccess(Access::Public);
            a_pType->addConstructor(pCtor);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getMoveConstructor()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
};

template<typename t_Ty>
struct DefaultMoveCtorProviderH<t_Ty, false>
{
    static void apply(Type*, StringView) {}
};

template<typename t_Ty, bool t_is_notstructure_and_has_move_assignment_op>
struct DefaultMoveAssignOpProviderH;

template<typename t_Ty>
struct DefaultMoveAssignOpProviderH<t_Ty, true>
{
#if PHANTOM_CUSTOM_USE_PRECISE_SIGNATURE
    static void apply(ClassType* a_pType)
    {
        PHANTOM_ASSERT(a_pType->getMoveAssignmentOperator() == nullptr, _PHNTM_do_not_declare_trivials_text);
        if (a_pType->getMoveAssignmentOperator() == nullptr)
        {
            auto pFunc = NewMeta<MethodT<t_Ty, t_Ty& (t_Ty::*)(t_Ty &&)>>(
            "operator=",
            Signature::Create(a_pType->addLValueReference(), a_pType->addRValueReference(), PHANTOM_R_NONE,
                              PHANTOM_R_FLAG_NATIVE),
            &t_Ty::operator=);
            pFunc->setAccess(Access::Public);
            a_pType->addMethod(pFunc);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getMoveAssignmentOperator()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
#else
    static void apply(ClassType* a_pType)
    {
        t_Ty& (t_Ty::*mf)(t_Ty &&) = &t_Ty::operator=;
        if (a_pType->getMoveAssignmentOperator() == nullptr)
        {
            auto pFunc = NewMeta<MethodT<int& (DummyClass::*)(int&)>>(
            "operator=",
            Signature::Create(a_pType->addLValueReference(), a_pType->addRValueReference(), Modifiers(0),
                              PHANTOM_R_FLAG_NATIVE),
            (int& (DummyClass::*)(int&))mf, PHANTOM_R_NONE, PHANTOM_R_FLAG_IMPLICIT);
            pFunc->setAccess(Access::Public);
            a_pType->addMethod(pFunc);
        }
        else
        {
            PHANTOM_ASSERT(!a_pType->getMoveAssignmentOperator()->testFlags(PHANTOM_R_FLAG_IMPLICIT));
        }
    }
#endif
};

template<typename t_Ty>
struct DefaultMoveAssignOpProviderH<t_Ty, false>
{
    static void apply(Type*) {}
};

template<typename t_Ty, bool t_is_class>
struct DtorProviderH
{
    static void apply(ClassType* a_pType, StringView a_strName)
    {
        PHANTOM_ASSERT(a_pType->getDestructor() == nullptr, _PHNTM_do_not_declare_trivials_text);
        auto pDtor = NewMeta<DestructorT<t_Ty>>(a_strName);
        pDtor->setAccess(Access::Public);
        a_pType->addMethod(pDtor);
    }
};

template<typename t_Ty>
struct DtorProviderH<t_Ty, false>
{
    static void apply(Type* a_pType, StringView a_strName) {}
};
} // namespace detail

template<typename t_Ty>
struct DefaultCtorProvider
    : public detail::DefaultCtorProviderH<t_Ty,
                                          std::is_class<t_Ty>::value && !std::is_abstract<t_Ty>::value &&
                                          ::phantom::IsPublicOrProtectedDefaultConstructible<t_Ty>::value>
{
};

template<typename t_Ty>
struct DefaultCopyCtorProvider
    : public detail::DefaultCopyCtorProviderH<t_Ty,
                                              !phantom::IsStructure<t_Ty>::value && std::is_class<t_Ty>::value &&
                                              IsCopyConstructibleAndNotDisabled<t_Ty>::value>
{
};

template<typename t_Ty>
struct DefaultCopyAssignOpProvider
    : public detail::DefaultCopyAssignOpProviderH<
      t_Ty, !phantom::IsStructure<t_Ty>::value && std::is_class<t_Ty>::value && HasCopyAssignmentOperator<t_Ty>::value>
{
};

template<typename t_Ty>
struct DefaultMoveCtorProvider
    : public detail::DefaultMoveCtorProviderH<t_Ty,
                                              !phantom::IsStructure<t_Ty>::value && std::is_class<t_Ty>::value &&
                                              IsMoveConstructibleAndNotDisabled<t_Ty>::value>
{
};

template<typename t_Ty>
struct DefaultMoveAssignOpProvider
    : public detail::DefaultMoveAssignOpProviderH<
      t_Ty, !phantom::IsStructure<t_Ty>::value && std::is_class<t_Ty>::value && HasMoveAssignmentOperator<t_Ty>::value>
{
};

template<typename t_Ty>
struct DtorProvider : public detail::DtorProviderH<t_Ty, std::is_class<t_Ty>::value>
{
};

template<typename t_Ty, typename t_Base>
class ClassTypeT : public TypeT<t_Ty, t_Base>
{
public:
    typedef ClassTypeT<t_Ty, t_Base> SelfType;
    typedef TypeT<t_Ty, t_Base>      BaseType;

    template<typename, bool>
    friend struct VTableRegistrerH;

protected:
    ClassTypeT(StringView a_TypeName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : BaseType(a_TypeName, a_Modifiers, a_uiFlags)
    {
    }

public:
    bool canHaveImplicitDefaultConstructor() const override { return SelfType::isDefaultConstructible(); }

    void addImplicitDefaultConstructor() override { DefaultCtorProvider<t_Ty>::apply(this, BaseType::m_strName); }

    bool canHaveImplicitCopyConstructor() const override { return SelfType::isTriviallyCopyConstructible(); }

    bool canHaveImplicitMoveConstructor() const override { return SelfType::isTriviallyMoveConstructible(); }

    void addImplicitCopyConstructor() override { DefaultCopyCtorProvider<t_Ty>::apply(this, BaseType::m_strName); }

    void addImplicitMoveConstructor() override { DefaultMoveCtorProvider<t_Ty>::apply(this, BaseType::m_strName); }

    bool canHaveImplicitCopyAssignmentOperator() const override { return SelfType::isTriviallyCopyAssignable(); }

    bool canHaveImplicitMoveAssignmentOperator() const override { return SelfType::isTriviallyMoveAssignable(); }

    void addImplicitCopyAssignmentOperator() override { DefaultCopyAssignOpProvider<t_Ty>::apply(this); }

    void addImplicitMoveAssignmentOperator() override { DefaultMoveAssignOpProvider<t_Ty>::apply(this); }

    void addImplicitDestructor() override { DtorProvider<t_Ty>::apply(this, "~" + BaseType::m_strName); }

    bool isPolymorphic() const override { return std::is_polymorphic<t_Ty>::value; }
    bool isDefaultConstructible() const override
    {
        return ::phantom::IsPublicOrProtectedDefaultConstructible<t_Ty>::value &&
        ((this->m_Modifiers & PHANTOM_R_FLAG_NO_DEFAULT_CTOR) == 0);
    }
    virtual bool isDefaultInstanciable() const override
    {
        return ::phantom::IsPublicOrProtectedDefaultConstructible<t_Ty>::value &&
        ((this->m_Modifiers & PHANTOM_R_FLAG_NO_DEFAULT_CTOR) == 0) && !std::is_abstract<t_Ty>::value;
    }
    bool hasCopyDisabled() const override { return HasCopyDisabled<t_Ty>::value; }
    bool hasMoveDisabled() const override { return HasMoveDisabled<t_Ty>::value; }

    virtual void finalizeNative() override
    {
        /// Ensure default constructor, copy constructor and destructor are available (if allowed)
        DefaultCtorProvider<t_Ty>::apply(this, BaseType::m_strName);
        DefaultCopyCtorProvider<t_Ty>::apply(this, BaseType::m_strName);
        DefaultCopyAssignOpProvider<t_Ty>::apply(this);
        DefaultMoveCtorProvider<t_Ty>::apply(this, BaseType::m_strName);
        DefaultMoveAssignOpProvider<t_Ty>::apply(this);
        DtorProvider<t_Ty>::apply(this, "~" + BaseType::m_strName);
    }

    void onElementsAccess() override { this->_onNativeElementsAccess(); }
};

template<typename t_Ty, typename t_Base>
struct TypeOf<ClassTypeT<t_Ty, t_Base>>
{
    static Type* object() { return Class::metaClass; }
};

template<typename t_Ty, typename t_Base>
struct MetaTypeOf<ClassTypeT<t_Ty, t_Base>>
{
    typedef MetaTypeOf<Class>::type type;
};

} // namespace lang

} // namespace phantom
