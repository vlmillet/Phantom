// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "ClassType.h"
#include "GlobalRegistrer.h"

#include <phantom/lang/ClassT.h>

namespace phantom
{
namespace lang
{
template<class T>
struct RemoveBaseAccessWrapper
{
    using type = T;
};
template<class T>
struct RemoveBaseAccessWrapper<_PHNTM_RegistrerKeyWords::protected_<T>> : RemoveBaseAccessWrapper<T>
{
};
template<class T>
struct RemoveBaseAccessWrapper<_PHNTM_RegistrerKeyWords::public_<T>> : RemoveBaseAccessWrapper<T>
{
};
template<class T>
struct RemoveBaseAccessWrapper<_PHNTM_RegistrerKeyWords::private_<T>> : RemoveBaseAccessWrapper<T>
{
};
template<class T>
using RemoveBaseAccessWrapperT = typename RemoveBaseAccessWrapper<T>::type;

template<class T>
struct BaseClassAccessH
{
    static const Access value = Access::Public;
};
template<class T>
struct BaseClassAccessH<_PHNTM_RegistrerKeyWords::public_<T>> : BaseClassAccessH<T>
{
};
template<class T>
struct BaseClassAccessH<_PHNTM_RegistrerKeyWords::protected_<T>>
{
    static const Access value = Access::Protected;
};
template<class T>
struct BaseClassAccessH<_PHNTM_RegistrerKeyWords::private_<T>>
{
    static const Access value = Access::Private;
};

template<class T>
struct RemoveForward<_PHNTM_RegistrerKeyWords::public_<T>>
{
    using type = _PHNTM_RegistrerKeyWords::public_<RemoveForwardT<T>>;
};
template<class T>
struct RemoveForward<_PHNTM_RegistrerKeyWords::protected_<T>>
{
    using type = _PHNTM_RegistrerKeyWords::protected_<RemoveForwardT<T>>;
};
template<class T>
struct RemoveForward<_PHNTM_RegistrerKeyWords::private_<T>>
{
    using type = _PHNTM_RegistrerKeyWords::private_<RemoveForwardT<T>>;
};

template<class Derived, class... Bases>
struct InheritanceH
{
    PHANTOM_STATIC_ASSERT((std::is_same<Derived, RemoveForwardT<Derived>>::value));
    PHANTOM_STATIC_ASSERT((IsTypeDefined<Derived>::value), "derived class is not defined");
    static void Add(lang::Class* a_pDerivedClass) { _add(a_pDerivedClass, _get<Bases>()...); }

private:
    template<class... LastPairs>
    inline static void _add(lang::Class* a_pDerivedClass, BaseClass const& a_BC, LastPairs... a_LastPairs)
    {
        a_pDerivedClass->addBaseClass(a_BC.baseClass, a_BC.offset, a_BC.access);
        _add(a_pDerivedClass, a_LastPairs...);
    }
    inline static void _add(lang::Class*) {} // end recursion
    template<typename BaseWrapped>
    inline static BaseClass _get()
    {
        using Base = RemoveBaseAccessWrapperT<BaseWrapped>;
        using BaseNoForward = RemoveForwardT<RemoveBaseAccessWrapperT<BaseWrapped>>;
        // we don't use static_cast here to avoid errors in case of private/protected inheritance,
        // instead we use C style cast + base static checks
        PHANTOM_STATIC_ASSERT((std::is_base_of<BaseNoForward, Derived>::value),
                              "class is not a base class (" PHANTOM_DIAGNOSTIC_FUNCTION ")");
        PHANTOM_STATIC_ASSERT((IsTypeDefined<BaseNoForward>::value), "base class is not defined");
        lang::Class* pBaseClass = PHANTOM_CLASSOF(Base);
        PHANTOM_ASSERT(pBaseClass, "base class lang was not found, check your lang definitions/lang files");
        BaseClass bc;
        bc.access = BaseClassAccessH<BaseWrapped>::value;
        bc.baseClass = pBaseClass;
        bc.offset = reinterpret_cast<size_t>((BaseNoForward*)(reinterpret_cast<Derived*>((void*)1))) - 1;
        return bc;
    }
};
template<class... Ts>
struct TypesH
{
    static lang::Types Get() { return lang::Types{phantom::lang::TypeOf<Ts>::object()...}; }
};

template<class T, class Top, class MostDerived>
struct ClassBuilderT : ClassTypeBuilderT<T, Top, MostDerived>
{
    _PHNTM_REG_FRIENDS;

public:
    using SelfType = ClassBuilderT<T, Top, MostDerived>;
    using BaseType = ClassTypeBuilderT<T, Top, MostDerived>;
    using TopType = Top;

    ClassBuilderT(Top* a_pTop, lang::Access a_StartAccess, TemplateSpecArgumentRegistrer a_Arguments)
        : BaseType(a_StartAccess, a_pTop, a_Arguments)
    {
        m_pClass = this->_PHNTM_getMeta();
    }

    // inheritance

    template<class... Bases>
    MostDerived& inherits()
    {
        this->m_Inheritance = [](lang::Class* a_pClass) { InheritanceH<T, Bases...>::Add(a_pClass); };
        return static_cast<MostDerived&>(*this);
    }

    // signal

    /// @brief signal declaration : ex : signal("mySignal", &_::mySignal)
    template<class Sign>
    MostDerived& signal(StringView a_Name, phantom::Signal<Sign>(T::*a_SignalPtr))
    {
        using SignalType = decltype(a_SignalPtr);
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<phantom::lang::SignalT<T, Sign>>::value),
                                 "missing #include <phantom/signal>");
        this->_addSymbol(
        m_pClass, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_SignalPtr)}, [](MemberBuilder const& a_Member) {
            a_Member.class_()->addSignal(a_Member.apply(NewMeta<phantom::lang::SignalT<T, Sign>>(
            a_Member.name, lang::SignatureH<Sign>::Create(), PHANTOM_REG_MEMBER_GETBACK_ARG(0, SignalType))));
        });
        return static_cast<MostDerived&>(*this);
    }

    template<class UserDataT, class... Args>
    MostDerived& userData(Args... a_Args)
    {
        m_pClass->setUserData(UserData(PHANTOM_NEW(UserDataT)(std::forward<Args>(a_Args)...), [](void* ptr) {
            PHANTOM_DELETE(UserDataT) reinterpret_cast<UserDataT*>(ptr);
        }));
        return static_cast<MostDerived&>(*this);
    }

    using BaseType::operator();

    MostDerived& operator()() { return *this; }

private:
    lang::Class*          m_pClass;
    lang::SymbolExtenders m_SymbolExtenders;
};

template<class Top, class MostDerived>
struct ClassBuilderT<AnonymousStructProxy, Top, MostDerived>
    : MemberAnonymousSectionBuilderT<lang::MemberAnonymousStruct, Top>
{
    ClassBuilderT(Top* a_pTop, lang::Access a_StartAccess, TemplateSpecArgumentRegistrer)
        : MemberAnonymousSectionBuilderT<lang::MemberAnonymousStruct, Top>(a_pTop)
    {
    }
};

template<class T, class Top>
struct DefaultClassBuilderT : ClassBuilderT<T, Top, DefaultClassBuilderT<T, Top>>
{
    using BaseType = ClassBuilderT<T, Top, DefaultClassBuilderT<T, Top>>;
    DefaultClassBuilderT(Top* a_pTop, lang::Access a_StartAccess, TemplateSpecArgumentRegistrer a_TArgs)
        : BaseType(a_pTop, a_StartAccess, a_TArgs)
    {
    }
};

} // namespace lang
} // namespace phantom

#define PHANTOM_INHERITS(class_, ...) (this_()).inherits<class_, __VA_ARGS__>()

#define PHANTOM_CLASS(TypeName, ...) _PHANTOM_CLASS(TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_T(TemplateTypes, TemplateParams, TypeName, ...)                                                  \
    _PHANTOM_CLASS_T(TemplateTypes, TemplateParams, TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_S(TemplateArgs, TypeName, ...) _PHANTOM_CLASS_S(TemplateArgs, TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, ...)                                   \
    _PHANTOM_CLASS_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_V(VariadicPair, TypeName, ...) _PHANTOM_CLASS_V(VariadicPair, TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, ...)                                   \
    _PHANTOM_CLASS_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_VS(VariadicPair, TemplateArgs, TypeName, ...)                                                    \
    _PHANTOM_CLASS_VS(VariadicPair, TemplateArgs, TypeName, Private, (__VA_ARGS__))
#define PHANTOM_CLASS_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, ...)                    \
    _PHANTOM_CLASS_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, Private, (__VA_ARGS__))

#define PHANTOM_STRUCT(TypeName, ...) _PHANTOM_CLASS(TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_T(TemplateTypes, TemplateParams, TypeName, ...)                                                 \
    _PHANTOM_CLASS_T(TemplateTypes, TemplateParams, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_S(TemplateArgs, TypeName, ...) _PHANTOM_CLASS_S(TemplateArgs, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, ...)                                  \
    _PHANTOM_CLASS_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_V(VariadicPair, TypeName, ...) _PHANTOM_CLASS_V(VariadicPair, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, ...)                                  \
    _PHANTOM_CLASS_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_VS(VariadicPair, TemplateArgs, TypeName, ...)                                                   \
    _PHANTOM_CLASS_VS(VariadicPair, TemplateArgs, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_STRUCT_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, ...)                   \
    _PHANTOM_CLASS_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, Public, (__VA_ARGS__))

#define PHANTOM_T_MCLASS(...) PHANTOM_T_MTYPE(class, ##__VA_ARGS__)
#define PHANTOM_T_MSTRUCT(...) PHANTOM_T_MTYPE(struct, ##__VA_ARGS__)

#define PHANTOM_MCLASS(...) PHANTOM_MTYPE(class, ##__VA_ARGS__)
#define PHANTOM_MSTRUCT(...) PHANTOM_MTYPE(struct, ##__VA_ARGS__)
