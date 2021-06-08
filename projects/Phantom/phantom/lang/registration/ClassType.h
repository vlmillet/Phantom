// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "MemberAnonymousSection.h"
#include "Scope.h"
#include "Template.h"
#include "Type.h"

#include <locale>
#include <phantom/detail/ClassOfFwd.h>
#include <phantom/detail/StaticGlobals.h>
#include <phantom/lang/ClassTypeT.h>
#include <phantom/lang/ConstantT.h>
#include <phantom/lang/MetaNewDelete.h>
#include <phantom/traits/FunctionTypeToFunctionPointerType.h>
#include <phantom/traits/FunctionTypeToMethodPointerType.h>
#include <phantom/traits/RemoveConstReference.h>
#include <phantom/traits/SelectType.h>
#include <phantom/utils/Placement.h>
#include <phantom/utils/StaticIf.h>
#include <phantom/utils/StringUtil.h>

namespace phantom
{
namespace lang
{
namespace detail
{
template<typename T, class Sign>
struct MethodPointerSimplifier;
PHANTOM_EXPORT_PHANTOM void newTemplateSpecialization(Template* a_pTemplate, const LanguageElements& arguments,
                                                      Symbol* a_pBody, uint a_uiFlags);
} // namespace detail
} // namespace lang
namespace lang
{
template<bool Cond>
struct if_H
{
    template<class MostDerived, class Lazy>
    static MostDerived& Do(MostDerived* this_, Lazy const& a_Lazy)
    {
        return a_Lazy(this_);
    }
};
template<>
struct if_H<false>
{
    template<class MostDerived, class Lazy>
    static MostDerived& Do(MostDerived* this_, Lazy const& a_Lazy)
    {
        return *this_;
    }
};

template<class T, class Sign, class MethodPtrT, int Modifiers = 0>
struct MethodH;

template<class T, class Proxy>
struct ProxyTypeH
{
    using type = Proxy;
};
template<class T>
struct ProxyTypeH<T, void>
{
    using type = T;
};

template<class T>
struct BuilderProxyType : ProxyTypeH<PHANTOM_TYPENAME T::ReflectedType, PHANTOM_TYPENAME T::BuilderProxyType>
{
};

template<class T>
using BuilderProxyTypeT = PHANTOM_TYPENAME BuilderProxyType<std::remove_reference_t<T>>::type;

template<class T, bool Possible>
struct InheritsIfPossible : public T
{
};

template<class T>
struct InheritsIfPossible<T, false>
{
};

template<class T>
using InheritsIfPossibleT = InheritsIfPossible<T, std::is_class<T>::value && !std::is_final<T>::value>;

template<class T, class Top, class MostDerived>
struct ClassTypeBuilderT : TypeBuilderT<T, Top, MostDerived>, ScopeBuilderT<MostDerived>
{
    _PHNTM_REG_FRIENDS;

    using BaseType = TypeBuilderT<T, Top, MostDerived>;
    using SelfType = ClassTypeBuilderT<T, Top, MostDerived>;
    using TopType = Top;
    using ReflectedType = T;
    using BuilderProxyType = PHANTOM_TYPENAME Top::_PHNTM_Proxy;

    using ScopeBuilderT<MostDerived>::struct_;
    using ScopeBuilderT<MostDerived>::union_;

    ClassTypeBuilderT(BuilderReleaser _releaser, lang::Access a_StartAccess, Top* a_pTop,
                      TemplateSpecArgumentRegistrer a_TplArguments)
        : BaseType(_releaser, a_pTop, a_TplArguments)
    {
        m_pClassType = this->_PHNTM_getMeta();
        m_pClassType->setDefaultAccess(a_StartAccess);
    }

    template<class ConstantT>
    MostDerived& constant(String a_Name, ConstantT a_Val)
    {
        _PHNTM_REG_STATIC_ASSERT(IsTypeDefined<lang::ConstantT<ConstantT>>::value,
                                 "missing #include <phantom/constant>");
        this->_addSymbol(
        m_pClassType, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_Val)}, [](MemberBuilder const& a_Member) {
            auto pConst = a_Member.classType()->NewMeta<lang::ConstantT<ConstantT>>(
            a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, ConstantT), PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE);
            a_Member.classType()->addConstant(pConst);
        });
        return static_cast<MostDerived&>(*this);
    }

    template<int Modifiers, class StaticFieldTForward>
    MostDerived& staticField(String a_Name, RemoveForwardT<StaticFieldTForward>* a_pVar)
    {
        using NoFwdT = RemoveForwardT<StaticFieldTForward>;
        _PHNTM_REG_STATIC_ASSERT(IsTypeDefined<lang::VariableT<NoFwdT>>::value,
                                 "missing #include <phantom/static_field>");
        this->_addSymbol(m_pClassType, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_pVar)},
                         [](MemberBuilder const& a_Member) {
                             auto pVar = a_Member.classType()->NewMeta<lang::VariableT<NoFwdT>>(
                             PHANTOM_TYPEOF(StaticFieldTForward), a_Member.name,
                             PHANTOM_REG_MEMBER_GETBACK_ARG(0, NoFwdT*), lang::Modifiers(Modifiers));
                             a_Member.classType()->addVariable(pVar);
                         });
        return static_cast<MostDerived&>(*this);
    }

    template<int Modifiers, class StaticFieldT>
    MostDerived& staticField(String a_Name, StaticFieldT* a_pVar)
    {
        using NoFwdT = RemoveForwardT<StaticFieldT>;
        _PHNTM_REG_STATIC_ASSERT(IsTypeDefined<lang::VariableT<StaticFieldT>>::value,
                                 "missing #include <phantom/static_field>");
        this->_addSymbol(m_pClassType, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_pVar)},
                         [](MemberBuilder const& a_Member) {
                             auto pVar = a_Member.classType()->NewMeta<lang::VariableT<NoFwdT>>(
                             PHANTOM_TYPEOF(StaticFieldT), a_Member.name,
                             PHANTOM_REG_MEMBER_GETBACK_ARG(0, StaticFieldT*), lang::Modifiers(Modifiers));
                             a_Member.classType()->addVariable(pVar);
                         });
        return static_cast<MostDerived&>(*this);
    }

    template<class StaticFieldTForward>
    MostDerived& staticField(String a_Name, RemoveForwardT<StaticFieldTForward>* a_pVar)
    {
        return staticField<0, StaticFieldTForward>(a_Name, a_pVar);
    }

    template<class StaticFieldT>
    MostDerived& staticField(String a_Name, StaticFieldT* a_pVar)
    {
        return staticField<0, StaticFieldT>(a_Name, a_pVar);
    }

    // access

    MostDerived& access_(lang::Access a_Access)
    {
        this->_addAccess(m_pClassType, a_Access);
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& public_() { return access_(lang::Access::Public); }
    MostDerived& protected_() { return access_(lang::Access::Protected); }
    MostDerived& private_() { return access_(lang::Access::Private); }

    // constructor

    template<class F, class... Args>
    struct FirstConstructorArg
    {
        using type = F;
    };

    template<class Sign, int Modifiers = 0>
    MostDerived& constructor()
    {
        using CtorNoFwd = phantom::lang::ConstructorT<T, RemoveForwardT<Sign>>;
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<CtorNoFwd>::value), "missing #include <phantom/constructor>");
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<phantom::lang::SignatureH<Sign>>::value),
                                 "missing #include <phantom/constructor>");
        this->_addCtor(m_pClassType, [](MemberBuilder const& a_Member) {
            auto pConstructor = a_Member.apply(a_Member.classType()->NewMeta<CtorNoFwd>(
            a_Member.classType()->getName(), phantom::lang::SignatureH<Sign>::Create(a_Member.classType()->getSource()),
            lang::Modifiers(Modifiers)));
            a_Member.classType()->addConstructor(pConstructor);
        });
        return static_cast<MostDerived&>(*this);
    }

    // member typedef/alias

    template<class MemberTypedefT, class DontTouchThis = lang::Alias>
    MostDerived& typedef_(StringView a_Name)
    {
        using NoFwdT = RemoveForwardT<MemberTypedefT>;
        _PHNTM_REG_STATIC_ASSERT(
        (std::is_same<MemberTypedefT, MemberTypedefT>::value && phantom::IsTypeDefined<DontTouchThis>::value),
        "missing #include <phantom/typedef>");
        this->_addSymbol(m_pClassType, a_Name, [](MemberBuilder const& a_Member) {
            auto pAlias = a_Member.classType()->addAlias(PHANTOM_TYPEOF(NoFwdT), a_Member.name, lang::Modifier::None,
                                                         PHANTOM_R_FLAG_NATIVE);
            a_Member.apply(pAlias);
        });
        return static_cast<MostDerived&>(*this);
    }

    template<class MemberTypedefT>
    MostDerived& alias_(StringView a_Name)
    {
        return this->typedef_<MemberTypedefT>(a_Name);
    }

    MostDerived& using_(StringView a_Name)
    {
        this->_addSymbol(m_pClassType, a_Name, [](MemberBuilder const& a_Member) {
            auto                   pClassType = a_Member.classType();
            phantom::lang::Symbols symbols;
            phantom::lang::detail::findCppSymbols(a_Member.name, symbols, pClassType);
            PHANTOM_ASSERT(symbols.size(), "using_ : symbol not found '%.*s'",
                           PHANTOM_STRING_AS_PRINTF_ARG(a_Member.name));
            for (auto pSymbol : symbols)
            {
                auto pAlias =
                pClassType->addAlias(pSymbol, pSymbol->getName(), lang::Modifier::None, PHANTOM_R_FLAG_NATIVE);
                a_Member.apply(pAlias);
            }
        });
        return static_cast<MostDerived&>(*this);
    }

    // methods

    // deduced

    template<class R, class... Args>
    MostDerived& staticMethod(StringView a_Name, R (*a_Ptr)(Args...))
    {
        return staticMethod<R(Args...)>(a_Name, a_Ptr);
    }

#if PHANTOM_HAS_FASTCALL
    template<class R, class... Args>
    MostDerived& staticMethod(StringView a_Name, R(__fastcall* a_Ptr)(Args...))
    {
        return staticMethod<R __fastcall(Args...)>(a_Name, a_Ptr);
    }
#endif

#if PHANTOM_HAS_STDCALL
    template<class R, class... Args>
    MostDerived& staticMethod(StringView a_Name, R(__stdcall* a_Ptr)(Args...))
    {
        return staticMethod<R __stdcall(Args...)>(a_Name, a_Ptr);
    }
#endif

    template<class Sign>
    MostDerived& staticMethod(StringView       a_Name,
                              PHANTOM_TYPENAME FunctionTypeToFunctionPointerType<RemoveForwardT<Sign>>::type a_Ptr)
    {
        _PHNTM_REG_STATIC_ASSERT(
        IsTypeDefined<
        lang::FunctionProviderT<PHANTOM_TYPENAME FunctionTypeToFunctionPointerType<RemoveForwardT<Sign>>::type>>::value,
        "missing #include <phantom/static_method>");
        using FuncProviderNoFwd =
        lang::FunctionProviderT<PHANTOM_TYPENAME FunctionTypeToFunctionPointerType<RemoveForwardT<Sign>>::type>;
        _PHNTM_REG_STATIC_ASSERT(IsTypeDefined<FuncProviderNoFwd>::value, "missing #include <phantom/static_method>");
        using FuncPtrT = decltype(a_Ptr);
        if (a_Name.back() == '>')
        {
            size_t findStart = 0;
            if (a_Name.find("operator<") == 0)
            {
                findStart += 9;
                if (a_Name.find("operator<<") == 0)
                {
                    if (a_Name[findStart] == '<')
                        a_Name = a_Name.substr(0, findStart + 1);
                    else
                        a_Name = a_Name.substr(0, findStart);
                }
            }
            else
            {
                a_Name = a_Name.substr(0, a_Name.find_first_of("<", findStart));
            }
        }
        this->_addFunc(m_pClassType, a_Name, PHANTOM_REG_MEMBER_FORWARD_ARG(a_Ptr), [](MemberBuilder const& a_Member) {
            auto pFunc = FuncProviderNoFwd::CreateFunction(
            a_Member.classType(), a_Member.name, lang::SignatureH<Sign>::Create(a_Member.classType()->getSource()),
            PHANTOM_REG_MEMBER_GETBACK_ARG(0, FuncPtrT));
            a_Member.classType()->addFunction(a_Member.apply(pFunc));
        });
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator[](StringView a_DefaultArg)
    {
        PHANTOM_ASSERT(this->m_Members.back().isFunc, "last declaration does not accept default arguments");
        while (a_DefaultArg.size() && StringUtil::IsBlank(a_DefaultArg.front()))
            a_DefaultArg = a_DefaultArg.substr(1);
        this->m_Members.back().defaultArguments.push_back(a_DefaultArg);
        return static_cast<MostDerived&>(*this);
    }

    // deduced
    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...))
    {
        return method<R(Args...), Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...))
    {
        return method<R(Args...)>(a_Name, a_MPtr);
    }

    // deduced const

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const)
    {
        return method<R(Args...) const, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const)
    {
        return method<R(Args...) const>(a_Name, a_MPtr);
    }

    // deduced volatile

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) volatile)
    {
        return method<R(Args...) volatile, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) volatile)
    {
        return method<R(Args...) volatile>(a_Name, a_MPtr);
    }

    // deduced const volatile

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const volatile)
    {
        return method<R(Args...) const volatile, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const volatile)
    {
        return method<R(Args...) const volatile>(a_Name, a_MPtr);
    }

    // deduced &

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) &)
    {
        return method<R(Args...)&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) &)
    {
        return method<R(Args...)&>(a_Name, a_MPtr);
    }

    // deduced const &

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const&)
    {
        return method<R(Args...) const&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const&)
    {
        return method<R(Args...) const&>(a_Name, a_MPtr);
    }

    // deduced volatile &

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) volatile&)
    {
        return method<R(Args...) volatile&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) volatile&)
    {
        return method<R(Args...) volatile&>(a_Name, a_MPtr);
    }

    // deduced const volatile &

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const volatile&)
    {
        return method<R(Args...) const volatile&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const volatile&)
    {
        return method<R(Args...) const volatile&>(a_Name, a_MPtr);
    }

    // deduced &&

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) &&)
    {
        return method<R(Args...)&&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) &&)
    {
        return method<R(Args...) &&>(a_Name, a_MPtr);
    }

    // deduced const &&

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const&&)
    {
        return method<R(Args...) const&&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const&&)
    {
        return method<R(Args...) const&&>(a_Name, a_MPtr);
    }

    // deduced volatile &&

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) volatile&&)
    {
        return method<R(Args...) volatile&&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) volatile&&)
    {
        return method<R(Args...) volatile&&>(a_Name, a_MPtr);
    }

    // deduced const volatile &&

    template<int Modifiers, class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const volatile&&)
    {
        return method<R(Args...) const volatile&&, Modifiers>(a_Name, a_MPtr);
    }
    template<class R, class... Args>
    MostDerived& method(StringView a_Name, R (T::*a_MPtr)(Args...) const volatile&&)
    {
        return method<R(Args...) const volatile&&>(a_Name, a_MPtr);
    }

    // explicit signature

    /// @brief explicit method declaration : ex : method&lt;void(int) const&&gt("myMethod",
    /// &MyClass::myMethod)
    template<class Sign, int Modifiers = 0>
    MostDerived& method(StringView       a_Name,
                        PHANTOM_TYPENAME FunctionTypeToMethodPointerType<T, RemoveForwardT<Sign>>::type a_MPtr)
    {
        using SignNoFwd = RemoveForwardT<Sign>;
        _PHNTM_REG_STATIC_ASSERT(
        (phantom::IsTypeDefined<phantom::lang::detail::MethodPointerSimplifier<T, SignNoFwd>>::value),
        "missing #include <phantom/method>");
        auto simplified = ::phantom::lang::detail::MethodPointerSimplifier<T, SignNoFwd>::Simplify(a_MPtr);
        using SimplifiedType = decltype(simplified);

        if (a_Name.back() == '>')
            a_Name = a_Name.substr(0, a_Name.find_first_of("<"));
        this->_addMethod(
        m_pClassType, a_Name, PHANTOM_REG_MEMBER_FORWARD_ARG(simplified), [](MemberBuilder const& a_Member) {
            auto pMethod = a_Member.classType()->NewMeta<::phantom::lang::MethodT<SimplifiedType>>(
            a_Member.name, lang::SignatureH<Sign>::Create(a_Member.classType()->getSource()),
            PHANTOM_REG_MEMBER_GETBACK_ARG(0, SimplifiedType),
            lang::Modifiers(Modifiers & (lang::Modifier::Virtual | lang::Modifier::Override | lang::Modifier::Final)),
            PHANTOM_R_FLAG_NONE);
            pMethod = a_Member.apply(pMethod);
            static_cast<lang::ClassType*>(a_Member.owner)->addMethod(pMethod);
        });
        return static_cast<MostDerived&>(*this);
    }

    // #if PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
    //
    // 			template<bool Cond, typename F>
    // 			MostDerived& if_(F const& f)
    // 			{
    //                 return if_H<Cond>::Do(static_cast<MostDerived*>(this), f);
    //             }
    //
    // #define PHANTOM_IF(Cond, ...) \
// 	.PHANTOM_T if_ <(Cond)>([](auto identity) { using _ =
    // phantom::lang::BuilderProxyTypeT<std::remove_pointer_t<std::remove_reference_t<decltype(this_)>>>;
    // (*this_) __VA_ARGS__; })
    //
    // #else
    template<bool Cond, typename F>
    MostDerived& if_(F const& a_F)
    {
        phantom::static_if<Cond>([&](auto a_Idtty) { a_F(a_Idtty(this_())); });
        return static_cast<MostDerived&>(*this);
    }

#define PHANTOM_IF(Cond, ...)                                                                                          \
    .PHANTOM_T if_<(Cond)>([&](auto&& a_This) {                                                                        \
        PHANTOM_IF_NOT_COMPILER_VISUAL_STUDIO(                                                                         \
        using _ = phantom::lang::BuilderProxyTypeT<std::remove_reference_t<decltype(a_This)>>;)                        \
        a_This __VA_ARGS__;                                                                                            \
    })

    //#endif

    // field

    /// @brief explicit method declaration : ex : method&lt;void(int) const&&gt("myMethod",
    /// &MyClass::myMethod)

    template<int Modifiers, class ValueTypeFwd>
    MostDerived& field(StringView a_Name, RemoveForwardT<ValueTypeFwd>(T::*a_FPtr),
                       uint       a_FilterMask = PHANTOM_R_FILTER_FIELD)
    {
        using NoFwdT = RemoveForwardT<ValueTypeFwd>;
        using PtrType = decltype(a_FPtr);
        _PHNTM_REG_STATIC_ASSERT(phantom::IsTypeDefined<lang::FieldT<NoFwdT(T::*)>>::value,
                                 "missing #include <phantom/field>");
        this->_addField(m_pClassType, a_Name, PHANTOM_REG_MEMBER_FORWARD_ARG(a_FPtr), a_FilterMask,
                        [](MemberBuilder const& a_Member) {
                            static_cast<lang::ClassType*>(a_Member.owner)
                            ->addField(a_Member.apply(a_Member.classType()->NewMeta<lang::FieldT<NoFwdT(T::*)>>(
                            PHANTOM_TYPEOF(ValueTypeFwd), a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, PtrType),
                            a_Member.filter, lang::Modifiers(Modifiers))));
                        });
        return static_cast<MostDerived&>(*this);
    }

    template<int Modifiers, class ValueType>
    MostDerived& field(StringView a_Name, ValueType(T::*a_FPtr), uint a_FilterMask = PHANTOM_R_FILTER_FIELD)
    {
        using PtrType = decltype(a_FPtr);
        _PHNTM_REG_STATIC_ASSERT(phantom::IsTypeDefined<lang::FieldT<ValueType(T::*)>>::value,
                                 "missing #include <phantom/field>");
        this->_addField(m_pClassType, a_Name, PHANTOM_REG_MEMBER_FORWARD_ARG(a_FPtr), a_FilterMask,
                        [](MemberBuilder const& a_Member) {
                            static_cast<lang::ClassType*>(a_Member.owner)
                            ->addField(a_Member.apply(a_Member.classType()->NewMeta<lang::FieldT<ValueType(T::*)>>(
                            PHANTOM_TYPEOF(ValueType), a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, PtrType),
                            a_Member.filter, lang::Modifiers(Modifiers))));
                        });
        return static_cast<MostDerived&>(*this);
    }

    template<class ValueTypeFwd>
    MostDerived& field(StringView a_Name, RemoveForwardT<ValueTypeFwd>(T::*a_FPtr),
                       uint       a_FilterMask = PHANTOM_R_FILTER_FIELD)
    {
        return this->field<0, ValueTypeFwd>(a_Name, a_FPtr, a_FilterMask);
    }

    template<class ValueType>
    MostDerived& field(StringView a_Name, ValueType(T::*a_FPtr), uint a_FilterMask = PHANTOM_R_FILTER_FIELD)
    {
        return this->field<0, ValueType>(a_Name, a_FPtr, a_FilterMask);
    }

    // property (does not support forwarding and it's considered not critical for now)

    template<class CT, class ReturnType, class ParamType = ReturnType, class SignalSign = void(ParamType)>
    using _Prop = phantom::lang::PropertyT<CT, ParamType, ReturnType, SignalSign>;

#define _PHNTM_PROPERTY_STATIC_CHECKS_RET                                                                              \
    using ReturnTypeNoConstRef = PHANTOM_TYPENAME RemoveConstReference<ReturnType>::type;                              \
    _PHNTM_REG_STATIC_ASSERT(!std::is_reference<ReturnTypeNoConstRef>::value,                                          \
                             "property: types cannot be T& references, only T const& or T");

#define _PHNTM_PROPERTY_STATIC_CHECKS_PARAM                                                                            \
    using ParamTypeNoConstRef = PHANTOM_TYPENAME RemoveConstReference<ParamType>::type;                                \
    _PHNTM_REG_STATIC_ASSERT(!std::is_reference<ParamTypeNoConstRef>::value,                                           \
                             "property: types cannot be T& references, only T const& or T");

#define _PHNTM_PROPERTY_STATIC_CHECKS_PARAMS                                                                           \
    _PHNTM_PROPERTY_STATIC_CHECKS_RET                                                                                  \
    _PHNTM_PROPERTY_STATIC_CHECKS_PARAM                                                                                \
    _PHNTM_REG_STATIC_ASSERT((std::is_same<ParamTypeNoConstRef, ReturnTypeNoConstRef>::value),                         \
                             "property: set parameter type and get return type must be equal modulo 'const&'");

#define _PHNTM_PROPERTY_STATIC_CHECKS_SIGNAL                                                                           \
    _PHNTM_PROPERTY_STATIC_CHECKS_PARAMS                                                                               \
    _PHNTM_REG_STATIC_ASSERT(                                                                                          \
    (std::is_same<SignalSign, void()>::value || std::is_same<SignalSign, void(ParamType)>::value),                     \
    "property: signal parameters must be empty or equal to set signature parameters");

    template<class ParamType, class ReturnType, class SignalSign, class = std::enable_if_t<IsForward<ParamType>::value>>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const,
                          void (T::*a_Set)(RemoveForwardT<ParamType>), phantom::Signal<SignalSign>(T::*a_Signal),
                          uint a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        return this->property<RemoveForwardT<ParamType>, ReturnType, SignalSign>(a_Name, a_Get, a_Set, a_Signal,
                                                                                 a_FilterMask);
    }

    template<int Modifiers, class ParamType, class ReturnType, class SignalSign,
             class = std::enable_if_t<IsForward<ParamType>::value>>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const,
                          void (T::*a_Set)(RemoveForwardT<ParamType>), uint a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        return this->property<Modifiers, RemoveForwardT<ParamType>, ReturnType, SignalSign>(a_Name, a_Get, a_Set,
                                                                                            a_FilterMask);
    }

    template<class ParamType, class ReturnType, class = std::enable_if_t<IsForward<ParamType>::value>>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const,
                          void (T::*a_Set)(RemoveForwardT<ParamType>), uint a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        return this->property<RemoveForwardT<ParamType>, ReturnType>(a_Name, a_Get, a_Set, a_FilterMask);
    }

    template<int Modifiers, class ParamType, class ReturnType, class = std::enable_if_t<IsForward<ParamType>::value>>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const,
                          void (T::*a_Set)(RemoveForwardT<ParamType>), uint a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        return this->property<Modifiers, RemoveForwardT<ParamType>, ReturnType>(a_Name, a_Get, a_Set, a_FilterMask);
    }

    template<int Modifiers, class ParamType, class ReturnType = ParamType, class SignalSign = void(ParamType)>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const, void (T::*a_Set)(ParamType),
                          phantom::Signal<SignalSign>(T::*a_Signal), uint a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        using GetType = decltype(a_Get);
        using SetType = decltype(a_Set);
        using SignalType = decltype(a_Signal);
        _PHNTM_PROPERTY_STATIC_CHECKS_SIGNAL;
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<_Prop<T, ReturnType, ParamType, SignalSign>>::value),
                                 "missing #include <phantom/property>");
        this->_addProperty(m_pClassType, a_Name,
                           {PHANTOM_REG_MEMBER_FORWARD_ARG(a_Get), PHANTOM_REG_MEMBER_FORWARD_ARG(a_Set),
                            PHANTOM_REG_MEMBER_FORWARD_ARG(a_Signal)},
                           a_FilterMask, [](MemberBuilder const& a_Member) {
                               auto pProp = a_Member.classType()->NewMeta<_Prop<T, ReturnType, ParamType, SignalSign>>(
                               PHANTOM_TYPEOF(ParamType), a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, GetType),
                               PHANTOM_REG_MEMBER_GETBACK_ARG(1, SetType),
                               PHANTOM_REG_MEMBER_GETBACK_ARG(2, SignalType), a_Member.filter,
                               lang::Modifiers(Modifiers));
                               a_Member.classType()->addProperty(pProp);
                           });
        return static_cast<MostDerived&>(*this);
    }

    template<class ParamType, class ReturnType = ParamType, class SignalSign = void(ParamType)>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const, void (T::*a_Set)(ParamType),
                          phantom::Signal<SignalSign>(T::*a_Signal), uint a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        return property<0, ParamType, ReturnType, SignalSign>(a_Name, a_Get, a_Set, a_Signal, a_FilterMask);
    }

    template<int Modifiers, class ParamType, class ReturnType = ParamType>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const, void (T::*a_Set)(ParamType),
                          uint       a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        using GetType = decltype(a_Get);
        using SetType = decltype(a_Set);
        _PHNTM_PROPERTY_STATIC_CHECKS_PARAMS;
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<_Prop<T, ReturnType, ParamType>>::value),
                                 "missing #include <phantom/property>");
        this->_addProperty(
        m_pClassType, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_Get), PHANTOM_REG_MEMBER_FORWARD_ARG(a_Set)},
        a_FilterMask, [](MemberBuilder const& a_Member) {
            auto pProp = a_Member.classType()->NewMeta<_Prop<T, ReturnType, ParamType>>(
            PHANTOM_TYPEOF(ParamType), a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, GetType),
            PHANTOM_REG_MEMBER_GETBACK_ARG(1, SetType), nullptr, a_Member.filter, lang::Modifiers(Modifiers));
            a_Member.classType()->addProperty(a_Member.apply(pProp));
        });
        return static_cast<MostDerived&>(*this);
    }

    template<class ParamType, class ReturnType = ParamType>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const, void (T::*a_Set)(ParamType),
                          uint       a_FilterMask = PHANTOM_R_FILTER_PROPERTY)
    {
        return property<0, ParamType, ReturnType>(a_Name, a_Get, a_Set, a_FilterMask);
    }

    template<int Modifiers, class ReturnType>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const,
                          uint       a_FilterMask = 0) // 0 because a property without setter is hardly serializable ...
    {
        using GetType = decltype(a_Get);
        _PHNTM_PROPERTY_STATIC_CHECKS_RET;
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<_Prop<T, ReturnType>>::value),
                                 "missing #include <phantom/property>");
        this->_addProperty(m_pClassType, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_Get)}, a_FilterMask,
                           [](MemberBuilder const& a_Member) {
                               auto pProp = a_Member.classType()->NewMeta<_Prop<T, ReturnType>>(
                               PHANTOM_TYPEOF(ReturnType), a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, GetType),
                               nullptr, nullptr, a_Member.filter, lang::Modifiers(Modifiers));
                               a_Member.classType()->addProperty(a_Member.apply(pProp));
                           });
        return static_cast<MostDerived&>(*this);
    }

    template<class ReturnType>
    MostDerived& property(StringView a_Name, ReturnType (T::*a_Get)() const,
                          uint       a_FilterMask = 0) // 0 because a property without setter is hardly serializable ...
    {
        return property<0, ReturnType>(a_Name, a_Get, a_FilterMask);
    }

    template<class ReturnType, class = std::enable_if_t<IsForward<ReturnType>::value>>
    MostDerived& property(StringView a_Name, RemoveForwardT<ReturnType> (T::*a_Get)() const,
                          uint       a_FilterMask = 0) // 0 because a property without setter is hardly serializable ...
    {
        return property<0, RemoveForwardT<ReturnType>>(a_Name, a_Get, a_FilterMask);
    }

    template<int Modifiers, class ParamType>
    MostDerived& property(StringView a_Name, void (T::*a_Set)(ParamType),
                          uint       a_FilterMask = 0) // 0 because a property without setter is hardly serializable ...
    {
        using SetType = decltype(a_Set);
        _PHNTM_PROPERTY_STATIC_CHECKS_PARAM;
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<_Prop<T, ParamType>>::value),
                                 "missing #include <phantom/property>");
        this->_addProperty(
        m_pClassType, a_Name, {PHANTOM_REG_MEMBER_FORWARD_ARG(a_Set)}, a_FilterMask, [](MemberBuilder const& a_Member) {
            auto pProp = a_Member.classType()->NewMeta<_Prop<T, ParamType>>(
            PHANTOM_TYPEOF(ParamType), a_Member.name, nullptr, PHANTOM_REG_MEMBER_GETBACK_ARG(0, SetType), nullptr,
            a_Member.filter, lang::Modifiers(Modifiers));
            a_Member.classType()->addProperty(a_Member.apply(pProp));
        });
        return static_cast<MostDerived&>(*this);
    }

    template<class ParamType>
    MostDerived& property(StringView a_Name, void (T::*a_Set)(ParamType),
                          uint       a_FilterMask = 0) // 0 because a property without getter is hardly serializable ...
    {
        return property<0, ParamType>(a_Name, a_Set, a_FilterMask);
    }

    template<class ParamType, class = std::enable_if_t<IsForward<ParamType>::value>>
    MostDerived& property(StringView a_Name, void (T::*a_Set)(RemoveForwardT<ParamType>),
                          uint       a_FilterMask = 0) // 0 because a property without setter is hardly serializable ...
    {
        return property<0, RemoveForwardT<ParamType>>(a_Name, a_Set, a_FilterMask);
    }

    template<class FriendClass>
    MostDerived& friend_()
    {
        this->_addSymbol(m_pClassType, "", [](MemberBuilder const& a_Member) {
            a_Member.classType()->addFriend(PHANTOM_TYPEOF(FriendClass));
        });
        return static_cast<MostDerived&>(*this);
    }

    template<class FriendClass, class DontTouchThis = lang::Application>
    MostDerived& friend_(StringView a_SymbolName)
    {
        this->_addSymbol(m_pClassType, a_SymbolName, [](MemberBuilder const& a_Member) {
            _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<DontTouchThis>::value),
                                     "missing #include <phantom/friend>");
            a_Member.classType()->addFriend(DontTouchThis::Get()->findCppSymbol(a_Member.name, a_Member.classType()));
        });
        return static_cast<MostDerived&>(*this);
    }

    template<template<class, class> class AddOnType, class... Args>
    AddOnType<T, MostDerived>& addon(Args... a_Args)
    {
        _PHNTM_REG_STATIC_ASSERT((std::is_base_of<PhantomBuilderBase, AddOnType<T, MostDerived>>::value),
                                 "AddOnType must derived from PhantomBuilderBase");
        auto pType =
        phantom::new_<AddOnType<T, MostDerived>>(static_cast<MostDerived*>(this), std::forward<Args>(a_Args)...);
        this->addSubBuilder(pType);
        return *pType;
    }

private:
    union {
        lang::ClassType* m_pClassType;
    };
    SmallVector<MemberRegistrer, 10> m_TemplateParams;
    lang::LanguageElements           m_TemplateArguments;
    lang::Access                     m_CurrentAccess;
};

struct TemplateParamValueH
{
    template<class T>
    auto operator*(T c)
    {
        return __PHNTM_ApplicationAsElement()->NewMeta<lang::ConstantT<T>>(c, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE);
    }
};
struct TemplateParamValueUnsignedH
{
    template<class T>
    auto operator*(T c)
    {
        return __PHNTM_ApplicationAsElement()->NewMeta<lang::ConstantT<std::make_unsigned_t<T>>>(
        (std::make_unsigned_t<T>)c, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE);
    }
};

template<class T>
struct ClassTypeCtorOnCall
{
    ClassTypeCtorOnCall(PHANTOM_TYPENAME T::TopType* a_pTop, lang::Access a_Access,
                        TemplateSpecArgumentRegistrer a_SpecReg)
        : place(false), m_pTop(a_pTop), m_Access(a_Access), m_SpecReg(a_SpecReg)
    {
    }

    T& operator()()
    {
        if (!place)
            place.construct(Delegate<void()>(this, &ClassTypeCtorOnCall<T>::release), m_pTop, m_Access, m_SpecReg);
        return *place;
    }

    void release()
    {
        if (place)
            place.destroy();
    }

    StaticGlobal<T>  place;
    PHANTOM_TYPENAME T::TopType*  m_pTop;
    lang::Access                  m_Access;
    TemplateSpecArgumentRegistrer m_SpecReg;
};

#define PHANTOM_ACCESS(Access)                                                                                         \
    (this_()).Access##_();                                                                                             \
    PHANTOM_PP_CAT(Access, __LINE__)

#define PHANTOM_PUBLIC PHANTOM_ACCESS(public)
#define PHANTOM_PROTECTED PHANTOM_ACCESS(protected)
#define PHANTOM_PRIVATE PHANTOM_ACCESS(private)

} // namespace lang
} // namespace phantom

#define _PHNTM_ADL_WORKAROUND_VS // PHANTOM_IF_COMPILER_VISUAL_STUDIO(inline void
                                 // _PHNTM_TypeOf(...);)

#define _PHNTM_CREATE_TEMPLATE_ARG(T, A) PHANTOM_PP_CAT(_PHNTM_TemplateParamValue_, T)(A)

#define _PHNTM_CREATE_TPL_SPEC(TemplateTypes, TemplateArgs)                                                            \
    []() -> phantom::lang::LanguageElements {                                                                          \
        return {_PHNTM_SURROUND_TPLARG(_PHNTM_CREATE_TEMPLATE_ARG, TemplateTypes, TemplateArgs)};                      \
    }

#define _PHNTM_CREATE_TPL_SPEC_V(TemplateTypes, TemplateArgs)                                                          \
    []() -> phantom::lang::LanguageElements {                                                                          \
        return {_PHNTM_SURROUND_TPLARG(_PHNTM_CREATE_TEMPLATE_ARG, TemplateTypes, TemplateArgs)...};                   \
    }

#define _PHANTOM_CLASS_COMMON(TemplateSpecADL, TemplateSign0, TemplateSign1, DecoratedType, StartAccess, ExtraCode,    \
                              Suffix, ...)                                                                             \
    template<class T>                                                                                                  \
    struct _PHNTM_Registrer;                                                                                           \
    PHANTOM_PP_IDENTITY TemplateSpecADL;                                                                               \
    PHANTOM_PP_IDENTITY TemplateSign0 struct _PHNTM_Registrer<PHANTOM_PP_IDENTITY DecoratedType>                       \
        : phantom::lang::InheritsIfPossibleT<PHANTOM_PP_IDENTITY DecoratedType>                                        \
    {                                                                                                                  \
        using _PHTNM_RegistrerType = _PHNTM_Registrer<PHANTOM_PP_IDENTITY DecoratedType>;                              \
        _PHNTM_Registrer() = delete;                                                                                   \
        _PHNTM_Registrer(_PHTNM_RegistrerType const&) = delete;                                                        \
        _PHNTM_Registrer(_PHTNM_RegistrerType&&) = delete;                                                             \
        _PHTNM_RegistrerType& operator=(_PHTNM_RegistrerType const&) = delete;                                         \
        _PHTNM_RegistrerType& operator=(_PHTNM_RegistrerType&&) = delete;                                              \
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<PHANTOM_PP_IDENTITY DecoratedType>::value),                   \
                                 PHANTOM_PP_QUOTE_L5(PHANTOM_PP_IDENTITY DecoratedType) " is not defined");            \
        using _ =                                                                                                      \
        phantom::SelectTypeT<!std::is_class<PHANTOM_PP_IDENTITY DecoratedType>::value ||                               \
                             std::is_final<PHANTOM_PP_IDENTITY DecoratedType>::value,                                  \
                             PHANTOM_PP_IDENTITY DecoratedType, _PHNTM_Registrer<PHANTOM_PP_IDENTITY DecoratedType>>;  \
        struct _PHNTM_User : ::phantom::lang::_PHNTM_GlobalRegistrer                                                   \
        {                                                                                                              \
            template<class _PHNTM_T>                                                                                   \
            using typedef_ = ::phantom::lang::Forward<_PHNTM_T>;                                                       \
            template<class _PHNTM_T>                                                                                   \
            using notypedef = ::phantom::lang::RemoveForwardT<_PHNTM_T>;                                               \
            using _PHNTM_ThisType = PHANTOM_PP_IDENTITY DecoratedType;                                                 \
            using _PHNTM_Proxy = _;                                                                                    \
            _PHNTM_User(bool a_bIsTemplateDependant = false)                                                           \
                : ::phantom::lang::_PHNTM_GlobalRegistrer(                                                             \
                  {::phantom::RegistrationStep::ClassTypes, ::phantom::RegistrationStep::End},                         \
                  &::phantom::lang::TypeInfosOf<PHANTOM_PP_IDENTITY DecoratedType>::object, __FILE__, __LINE__, 0,     \
                  a_bIsTemplateDependant),                                                                             \
                  this_(this, phantom::lang::Access::StartAccess,                                                      \
                        phantom::lang::TemplateArgsOfADL((_PHNTM_ThisType**)0))                                        \
            {                                                                                                          \
                if (!a_bIsTemplateDependant)                                                                           \
                    _PHNTM_attach();                                                                                   \
            }                                                                                                          \
            virtual void _PHNTM_process(phantom::RegistrationStep s) override                                          \
            {                                                                                                          \
                if (s == ::phantom::RegistrationStep::End)                                                             \
                    this_() PHANTOM_PP_IDENTITY ExtraCode;                                                             \
                else                                                                                                   \
                {                                                                                                      \
                    PHANTOM_CUSTOM_CLASS_HEADER##Suffix _PHNTM_processUserCode(s);                                     \
                    PHANTOM_CUSTOM_CLASS_FOOTER##Suffix this_().end();                                                 \
                }                                                                                                      \
            }                                                                                                          \
            inline void _PHNTM_processUserCode(phantom::RegistrationStep);                                             \
            ::phantom::lang::ClassTypeCtorOnCall<PHANTOM_BUILDER_TYPE(_PHNTM_User, PHANTOM_PP_IDENTITY DecoratedType)> \
            this_;                                                                                                     \
        };                                                                                                             \
    };                                                                                                                 \
    _PHNTM_ADL_WORKAROUND_VS                                                                                           \
    __VA_ARGS__                                                                                                        \
    PHANTOM_PP_IDENTITY TemplateSign1 inline void                                                                      \
    _PHNTM_Registrer<PHANTOM_PP_IDENTITY DecoratedType>::_PHNTM_User::_PHNTM_processUserCode(                          \
    PHANTOM_MAYBE_UNUSED phantom::RegistrationStep PHANTOM_REGISTRATION_STEP)

PHANTOM_EXPORT_PHANTOM bool _PHTNM_moduleHasDependency(phantom::lang::Module* _module, phantom::lang::Module* _dep);

template<class RegistrerType>
auto _PHTNM_TemplateTypeOfH()
{
    using MetaPtr = decltype(reinterpret_cast<RegistrerType*>(nullptr)->this_()._PHNTM_getMeta());
    using Meta = std::remove_pointer_t<MetaPtr>;
    if (auto inAnotherModule = phantom::lang::TypeOfUndefined<typename RegistrerType::_PHNTM_ThisType>::object())
    {
        if (phantom::lang::detail::currentModule() == inAnotherModule->getModule() ||
            _PHTNM_moduleHasDependency(phantom::lang::detail::currentModule(), inAnotherModule->getModule()))
            return static_cast<MetaPtr>(inAnotherModule);
    }
    static RegistrerType re(true);
    static MetaPtr       meta = nullptr;
    if (meta == nullptr)
    {
        meta = re.this_()._PHNTM_getMeta();
        PHANTOM_ASSERT(meta);
        re.__PHNTM_process(phantom::RegistrationStep::ClassTypes);
    }
    return meta;
}

#define _PHNTM_TEMPLATE_TYPEOF_BY_ADL(TemplateSign, DecoratedType)                                                     \
    PHANTOM_PP_IDENTITY TemplateSign inline auto _PHNTM_TypeOf(PHANTOM_PP_IDENTITY DecoratedType**)                    \
    {                                                                                                                  \
        return _PHTNM_TemplateTypeOfH<PHANTOM_TYPENAME _PHNTM_Registrer<                                               \
        phantom::lang::RemoveForwardTemplateT<PHANTOM_PP_IDENTITY DecoratedType>>::_PHNTM_User>();                     \
    }

#define _PHNTM_TEMPLATE_TYPEOF_BY_UNDEFINED(TemplateSign, DecoratedType)                                               \
    PHANTOM_PP_IDENTITY TemplateSign inline auto _PHNTM_TypeOf(PHANTOM_PP_IDENTITY DecoratedType**)                    \
    {                                                                                                                  \
        return phantom::lang::TypeOfUndefined<                                                                         \
        phantom::lang::RemoveForwardTemplateT<PHANTOM_PP_IDENTITY DecoratedType>>::object();                           \
    }

#define _PHANTOM_CLASS_T_COMMON(TemplateSpec, TemplateSign, DecoratedType, TypeName, StartAccess, ExtraCode, ...)      \
    _PHANTOM_CLASS_COMMON(TemplateSpec, TemplateSign, TemplateSign, DecoratedType, StartAccess, ExtraCode, _T,         \
                          __VA_ARGS__ _PHNTM_TEMPLATE_TYPEOF_BY_ADL(TemplateSign, DecoratedType))

#define _PHANTOM_CLASS_NO_TEMPLATE(DecoratedType, StartAccess, ExtraCode, RegName, ...)                                \
    _PHANTOM_CLASS_COMMON(                                                                                             \
    (), (template<>), (), DecoratedType, StartAccess, ExtraCode, , PHANTOM_IF_NOT_TEMPLATE_ONLY(namespace {            \
        _PHNTM_Registrer<PHANTOM_PP_IDENTITY DecoratedType>::_PHNTM_User PHANTOM_PP_CAT(RegName, _PHANTOM_);           \
    }) __VA_ARGS__)

#define _PHANTOM_CLASS_FULL_SPEC(TemplateSign0, TemplateSign1, DecoratedType, StartAccess, ExtraCode, RegName, ...)    \
    _PHANTOM_CLASS_COMMON(                                                                                             \
    (), TemplateSign0, TemplateSign1, DecoratedType, StartAccess, ExtraCode, ,                                         \
    PHANTOM_IF_NOT_TEMPLATE_ONLY(namespace {PHANTOM_REGISTER(ClassTypes) {                                          \
        static _PHNTM_Registrer<PHANTOM_PP_IDENTITY DecoratedType>::_PHNTM_User PHANTOM_PP_CAT(RegName, _PHANTOM_);    \
PHANTOM_PP_CAT(RegName, _PHANTOM_).this_()._PHNTM_setFullSpec();                                                       \
    }                                                                                                                  \
    })                                                                                                                \
    __VA_ARGS__)

#define _PHANTOM_CLASS(TypeName, StartAccess, ExtraCode)                                                               \
    _PHANTOM_CLASS_NO_TEMPLATE((TypeName), StartAccess, ExtraCode, TypeName)

#define _PHANTOM_CLASS_T(TemplateTypes, TemplateParams, TypeName, StartAccess, ExtraCode)                              \
    PHANTOM_IF_NOT_TEMPLATE_ONLY(class _PHNTM_HERE; namespace {                                                        \
        ::phantom::lang::TemplateRegistrer PHANTOM_PP_CAT(_PHNTM_TplRegistrer_, __COUNTER__)(                          \
        [](int id) -> phantom::StringView {                                                                            \
            switch (id)                                                                                                \
            {                                                                                                          \
            case 0:                                                                                                    \
                return phantom::lang::TypeInfosOf<_PHNTM_HERE>::object().scope();                                      \
            case 1:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateTypes;                                                                 \
            case 2:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateParams;                                                                \
            case 3:                                                                                                    \
                return PHANTOM_PP_QUOTE(TypeName);                                                                     \
            }                                                                                                          \
            return phantom::StringView();                                                                              \
        },                                                                                                             \
        __FILE__, __LINE__, __COUNTER__);                                                                              \
    })                                                                                                                 \
    _PHANTOM_CLASS_T_COMMON((template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams)>                                   \
                             phantom::lang::TemplateSpecArgumentRegistrer _PHNTM_TemplateArgsADL(                      \
                             TypeName<PHANTOM_PP_IDENTITY TemplateParams>**) {                                         \
                                 return _PHNTM_CREATE_TPL_SPEC(TemplateTypes, TemplateParams);                         \
                             }),                                                                                       \
                            (template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams)>),                                 \
                            (TypeName<PHANTOM_PP_IDENTITY TemplateParams>), TypeName, StartAccess, ExtraCode,          \
                            template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams)>                                    \
                            TypeName<_PHNTM_SURROUND_TPLARG(PHANTOM_REMOVE_FWD, TemplateTypes, TemplateParams)>*       \
                            _PHNTM_RemoveTemplateForwardADL(TypeName<PHANTOM_PP_IDENTITY TemplateParams>**);)

#define _PHANTOM_CLASS_S(TemplateArgs, TypeName, StartAccess, ExtraCode)                                               \
    _PHANTOM_CLASS_FULL_SPEC(                                                                                          \
    (template<>), (), (TypeName<PHANTOM_PP_IDENTITY TemplateArgs>), StartAccess, ExtraCode,                            \
    PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)(TypeName, __COUNTER__),                                                        \
    _PHNTM_TEMPLATE_TYPEOF_BY_UNDEFINED((template<>), (TypeName<PHANTOM_PP_IDENTITY TemplateArgs>)))

#define _PHANTOM_CLASS_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, StartAccess, ExtraCode)               \
    PHANTOM_IF_NOT_TEMPLATE_ONLY(class _PHNTM_HERE; namespace {                                                        \
        ::phantom::lang::TemplatePartialRegistrer PHANTOM_PP_CAT(_PHNTM_TplRegistrer_, __COUNTER__)(                   \
        [](int id) -> phantom::StringView {                                                                            \
            switch (id)                                                                                                \
            {                                                                                                          \
            case 0:                                                                                                    \
                return phantom::lang::TypeInfosOf<_PHNTM_HERE>::object().scope();                                      \
            case 1:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateTypes;                                                                 \
            case 2:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateParams;                                                                \
            case 3:                                                                                                    \
                return PHANTOM_PP_QUOTE(TypeName);                                                                     \
            case 4:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateArgs;                                                                  \
            }                                                                                                          \
            return phantom::StringView();                                                                              \
        },                                                                                                             \
        __FILE__, __LINE__, __COUNTER__);                                                                              \
    })                                                                                                                 \
    _PHANTOM_CLASS_T_COMMON((), (template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams)>),                             \
                            (TypeName<PHANTOM_PP_IDENTITY TemplateArgs>), TypeName, StartAccess, ExtraCode)

#define _PHANTOM_CLASS_V(VariadicPair, TypeName, StartAccess, ExtraCode)                                               \
    PHANTOM_IF_NOT_TEMPLATE_ONLY(class _PHNTM_HERE; namespace {                                                        \
        ::phantom::lang::TemplateRegistrer PHANTOM_PP_CAT(_PHNTM_TplRegistrer_, __COUNTER__)(                          \
        [](int id) -> phantom::StringView {                                                                            \
            switch (id)                                                                                                \
            {                                                                                                          \
            case 0:                                                                                                    \
                return phantom::lang::TypeInfosOf<_PHNTM_HERE>::object().scope();                                      \
            case 1:                                                                                                    \
                return PHANTOM_PP_QUOTE_ARG_0 VariadicPair "...";                                                      \
            case 2:                                                                                                    \
                return PHANTOM_PP_QUOTE_ARG_1 VariadicPair;                                                            \
            case 3:                                                                                                    \
                return PHANTOM_PP_QUOTE(TypeName);                                                                     \
            }                                                                                                          \
            return phantom::StringView();                                                                              \
        },                                                                                                             \
        __FILE__, __LINE__, __COUNTER__);                                                                              \
    })                                                                                                                 \
    _PHANTOM_CLASS_T_COMMON((template<PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>                  \
                             phantom::lang::TemplateSpecArgumentRegistrer _PHNTM_TemplateArgsADL(                      \
                             TypeName<PHANTOM_PP_ARG_1 VariadicPair...>**) {                                           \
                                 return _PHNTM_CREATE_TPL_SPEC_V(TemplateTypes, (PHANTOM_PP_ARG_1 VariadicPair));      \
                             }),                                                                                       \
                            (template<PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>),                \
                            (TypeName<PHANTOM_PP_ARG_1 VariadicPair...>), TypeName, StartAccess, ExtraCode,            \
                            template<PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>                   \
                            TypeName<_PHNTM_SURROUND_TPLARG(PHANTOM_REMOVE_FWD, (PHANTOM_PP_ARG_0 VariadicPair),       \
                                                            (PHANTOM_PP_ARG_1 VariadicPair))...>*                      \
                            _PHNTM_RemoveTemplateForwardADL(TypeName<PHANTOM_PP_ARG_1 VariadicPair...>**);)

#define _PHANTOM_CLASS_VS(VariadicPair, TemplateArgs, TypeName, StartAccess, ExtraCode)                                \
    _PHANTOM_CLASS_T_COMMON((), (template<PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>),            \
                            (TypeName<PHANTOM_PP_IDENTITY TemplateArgs>), TypeName, StartAccess, ExtraCode)

#define _PHANTOM_CLASS_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, StartAccess, ExtraCode)               \
    PHANTOM_IF_NOT_TEMPLATE_ONLY(class _PHNTM_HERE; namespace {                                                        \
        ::phantom::lang::TemplateRegistrer PHANTOM_PP_CAT(_PHNTM_TplRegistrer_, __COUNTER__)(                          \
        [](int id) -> phantom::StringView {                                                                            \
            switch (id)                                                                                                \
            {                                                                                                          \
            case 0:                                                                                                    \
                return phantom::lang::TypeInfosOf<_PHNTM_HERE>::object().scope();                                      \
            case 1:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateTypes "," PHANTOM_PP_QUOTE_ARG_0 VariadicPair "...";                   \
            case 2:                                                                                                    \
                return PHANTOM_PP_QUOTE TemplateParams "," PHANTOM_PP_QUOTE_ARG_1 VariadicPair;                        \
            case 3:                                                                                                    \
                return PHANTOM_PP_QUOTE(TypeName);                                                                     \
            }                                                                                                          \
            return phantom::StringView();                                                                              \
        },                                                                                                             \
        __FILE__, __LINE__, __COUNTER__);                                                                              \
    })                                                                                                                 \
    _PHANTOM_CLASS_T_COMMON(                                                                                           \
    (template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams),                                                           \
              PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>                                          \
     phantom::lang::TemplateSpecArgumentRegistrer _PHNTM_TemplateArgsADL(                                              \
     TypeName<PHANTOM_PP_IDENTITY TemplateParams, PHANTOM_PP_ARG_1 VariadicPair...>**) {                               \
         return _PHNTM_CREATE_TPL_SPEC_V((PHANTOM_PP_IDENTITY TemplateTypes, PHANTOM_PP_ARG_0 VariadicPair),           \
                                         (PHANTOM_PP_IDENTITY TemplateParams, PHANTOM_PP_ARG_1 VariadicPair));         \
     }),                                                                                                               \
    (template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams),                                                           \
              PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>                                          \
     ),                                                                                                                \
    (TypeName<PHANTOM_PP_IDENTITY TemplateParams, PHANTOM_PP_ARG_1 VariadicPair...>), TypeName, StartAccess,           \
    ExtraCode,                                                                                                         \
    template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams),                                                            \
             PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>                                           \
    TypeName<_PHNTM_SURROUND_TPLARG(PHANTOM_REMOVE_FWD,                                                                \
                                    (PHANTOM_PP_IDENTITY TemplateTypes, PHANTOM_PP_ARG_0 VariadicPair),                \
                                    (PHANTOM_PP_IDENTITY TemplateParams, PHANTOM_PP_ARG_1 VariadicPair))...>*          \
    _PHNTM_RemoveTemplateForwardADL(                                                                                   \
    TypeName<PHANTOM_PP_IDENTITY TemplateParams, PHANTOM_PP_ARG_1 VariadicPair...>**);)

#define _PHANTOM_CLASS_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, StartAccess,           \
                           ExtraCode)                                                                                  \
    _PHANTOM_CLASS_T_COMMON((),                                                                                        \
                            (template<PHANTOM_PP_MIX(TemplateTypes, TemplateParams),                                   \
                                      PHANTOM_PP_ARG_0 VariadicPair... PHANTOM_PP_ARG_1 VariadicPair>                  \
                             ),                                                                                        \
                            (TypeName<PHANTOM_PP_IDENTITY TemplateArgs>), TypeName, StartAccess, ExtraCode)

#define _PHNTM_TemplateParamValue_class(T) PHANTOM_TYPEOF(T)
#define _PHNTM_TemplateParamValue_typename(T) PHANTOM_TYPEOF(T)
#define _PHNTM_TemplateParamValue_unsigned phantom::lang::TemplateParamValueUnsignedH()*
#define _PHNTM_TemplateParamValue_bool(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_char(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_short(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_int(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_long(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_size_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_ptrdiff_t(V) phantom::lang::TemplateParamValueH() * V

PHANTOM_EXPORT_PHANTOM phantom::lang::LanguageElement* __PHNTM_ApplicationAsElement();

namespace _PHNTM_TemplateParamValue_std
{
inline phantom::lang::LanguageElement* size_t(std::size_t v)
{
    return __PHNTM_ApplicationAsElement()->NewMeta<phantom::lang::ConstantT<std::size_t>>(v, PHANTOM_R_NONE,
                                                                                          PHANTOM_R_FLAG_NATIVE);
}
inline phantom::lang::LanguageElement* ptrdiff_t(std::ptrdiff_t v)
{
    return __PHNTM_ApplicationAsElement()->NewMeta<phantom::lang::ConstantT<std::ptrdiff_t>>(v, PHANTOM_R_NONE,
                                                                                             PHANTOM_R_FLAG_NATIVE);
}
inline phantom::lang::LanguageElement* intptr_t(std::intptr_t v)
{
    return __PHNTM_ApplicationAsElement()->NewMeta<phantom::lang::ConstantT<std::intptr_t>>(v, PHANTOM_R_NONE,
                                                                                            PHANTOM_R_FLAG_NATIVE);
}
} // namespace _PHNTM_TemplateParamValue_std
namespace _PHNTM_TemplateParamValue_phantom
{
#define _PHNTM_TemplateParamValue_phantom_x(Type)                                                                      \
    inline phantom::lang::LanguageElement* Type(phantom::Type v)                                                       \
    {                                                                                                                  \
        return __PHNTM_ApplicationAsElement()->NewMeta<phantom::lang::ConstantT<phantom::Type>>(                       \
        v, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE);                                                                     \
    }

_PHNTM_TemplateParamValue_phantom_x(size_t);
_PHNTM_TemplateParamValue_phantom_x(schar);
_PHNTM_TemplateParamValue_phantom_x(uchar);
_PHNTM_TemplateParamValue_phantom_x(sshort);
_PHNTM_TemplateParamValue_phantom_x(ushort);
_PHNTM_TemplateParamValue_phantom_x(sint);
_PHNTM_TemplateParamValue_phantom_x(uint);
_PHNTM_TemplateParamValue_phantom_x(slong);
_PHNTM_TemplateParamValue_phantom_x(ulong);
_PHNTM_TemplateParamValue_phantom_x(slonglong);
_PHNTM_TemplateParamValue_phantom_x(ulonglong);

} // namespace _PHNTM_TemplateParamValue_phantom

namespace PHANTOM_TEMPLATE_VALUE_PARAM_std
{
inline constexpr std::size_t size_t(std::size_t s)
{
    return s;
}
inline constexpr std::ptrdiff_t ptrdiff_t(std::ptrdiff_t s)
{
    return s;
}
inline constexpr std::intptr_t intptr_t(std::intptr_t s)
{
    return s;
}
} // namespace PHANTOM_TEMPLATE_VALUE_PARAM_std
namespace PHANTOM_TEMPLATE_VALUE_PARAM_phantom
{
#define PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(Type)                                                                   \
    inline constexpr phantom::Type Type(phantom::Type s) { return s; }

PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(size_t);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(schar);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(uchar);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(sshort);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(ushort);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(sint);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(uint);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(slong);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(ulong);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(slonglong);
PHANTOM_TEMPLATE_VALUE_PARAM_phantom_x(ulonglong);

} // namespace PHANTOM_TEMPLATE_VALUE_PARAM_phantom

#define _PHNTM_TemplateParamValue_unsigned phantom::lang::TemplateParamValueUnsignedH()*
#define _PHNTM_TemplateParamValue_bool(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_char(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_short(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_int(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_long(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_size_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_ptrdiff_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_int8_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_uint8_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_int16_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_uint16_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_int32_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_uint32_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_int64_t(V) phantom::lang::TemplateParamValueH() * V
#define _PHNTM_TemplateParamValue_uint64_t(V) phantom::lang::TemplateParamValueH() * V

#define PHANTOM_TEMPLATE_VALUE_PARAM_class(T) PHANTOM_TYPENAME phantom::lang::RemoveForward<T>::type
#define PHANTOM_TEMPLATE_VALUE_PARAM_typename(T) PHANTOM_TYPENAME phantom::lang::RemoveForward<T>::type
#define PHANTOM_TEMPLATE_VALUE_PARAM_unsigned unsigned
#define PHANTOM_TEMPLATE_VALUE_PARAM_bool(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_char(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_short(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_int(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_long(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_size_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_ptrdiff_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_int8_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_uint8_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_int16_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_uint16_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_int32_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_uint32_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_int64_t(V) V
#define PHANTOM_TEMPLATE_VALUE_PARAM_uint64_t(V) V

#define PHANTOM_DECL_TEMPLATE_PARAM(EnumType)                                                                          \
    namespace                                                                                                          \
    {                                                                                                                  \
    constexpr EnumType PHANTOM_TEMPLATE_VALUE_PARAM_EnumType(EnumType m) { return m; }                                 \
    }

#define PHANTOM_REMOVE_FWD(T, P) PHANTOM_PP_CAT(PHANTOM_TEMPLATE_VALUE_PARAM_, T)(P)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define _PHNTM_SURROUND_TPLARG(Macro, TemplateTypes, TemplateParams)                                               \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_SURROUND_TPLARG_, PHANTOM_PP_ARGCOUNT TemplateTypes),                     \
                       (Macro, TemplateTypes, TemplateParams))
#else
#    define _PHNTM_SURROUND_TPLARG(Macro, TemplateTypes, TemplateParams)                                               \
        PHANTOM_PP_CAT(_PHNTM_SURROUND_TPLARG_, PHANTOM_PP_ARGCOUNT TemplateTypes)                                     \
        (Macro, TemplateTypes, TemplateParams)
#endif

#define _PHNTM_SURROUND_TPLARG_1(Macro, TemplateTypes, TemplateParams)                                                 \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_0 TemplateTypes, PHANTOM_PP_ARG_0 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_2(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_1(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_1 TemplateTypes, PHANTOM_PP_ARG_1 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_3(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_2(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_2 TemplateTypes, PHANTOM_PP_ARG_2 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_4(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_3(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_3 TemplateTypes, PHANTOM_PP_ARG_3 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_5(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_4(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_4 TemplateTypes, PHANTOM_PP_ARG_4 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_6(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_5(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_5 TemplateTypes, PHANTOM_PP_ARG_5 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_7(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_6(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_6 TemplateTypes, PHANTOM_PP_ARG_6 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_8(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_7(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_7 TemplateTypes, PHANTOM_PP_ARG_7 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_9(Macro, TemplateTypes, TemplateParams)                                                 \
    _PHNTM_SURROUND_TPLARG_8(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_8 TemplateTypes, PHANTOM_PP_ARG_8 TemplateParams)

#define _PHNTM_SURROUND_TPLARG_10(Macro, TemplateTypes, TemplateParams)                                                \
    _PHNTM_SURROUND_TPLARG_9(Macro, TemplateTypes, TemplateParams),                                                    \
    PHANTOM_PP_IDENTITY(Macro)(PHANTOM_PP_ARG_9 TemplateTypes, PHANTOM_PP_ARG_9 TemplateParams)

#if !defined(PHANTOM_CUSTOM_CLASS_HEADER)
#    define PHANTOM_CUSTOM_CLASS_HEADER
#endif

#if !defined(PHANTOM_CUSTOM_CLASS_FOOTER)
#    define PHANTOM_CUSTOM_CLASS_FOOTER
#endif

#if !defined(PHANTOM_CUSTOM_CLASS_HEADER_T)
#    define PHANTOM_CUSTOM_CLASS_HEADER_T
#endif

#if !defined(PHANTOM_CUSTOM_CLASS_FOOTER_T)
#    define PHANTOM_CUSTOM_CLASS_FOOTER_T
#endif
