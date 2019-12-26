// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "GlobalRegistrer.h"
#include "Scope.h"
#include "phantom/reflection/MetaNewDelete.h"

#include <locale>
#include <phantom/Placement.h>
#include <phantom/StaticGlobals.h>
#include <phantom/new.h>
#include <phantom/reflection/Namespace.h>
#include <phantom/reflection/Source.h>
#include <phantom/reflection/Subroutine.h>
#include <phantom/traits/FunctionTypeToFunctionPointerType.h>
#include <phantom/traits/IsTypeDefined.h>

namespace phantom
{
namespace reflection
{
struct PHANTOM_EXPORT_PHANTOM NamespaceBuilder : PhantomBuilderBase,
                                                 _PHNTM_RegistrerKeyWords,
                                                 ScopeBuilderT<NamespaceBuilder>
{
    PHANTOM_DECL_OVERRIDE_DELETE_METHOD(NamespaceBuilder);

    _PHNTM_REG_FRIENDS_NO_GLOBAL;
    friend struct _PHNTM_GlobalRegistrer;

    using SelfType = NamespaceBuilder;
    using ReflectedType = void;

    NamespaceBuilder(_PHNTM_GlobalRegistrer* a_pRegistrer);

    NamespaceBuilder& this_()
    {
        return *this;
    }

    template<class R, class... Args>
    NamespaceBuilder& function(StringView a_Name, R (*a_Ptr)(Args...))
    {
        return function<R(Args...)>(a_Name, a_Ptr);
    }

#if PHANTOM_HAS_FASTCALL
    template<class R, class... Args>
    NamespaceBuilder& function(StringView a_Name, R(__fastcall* a_Ptr)(Args...))
    {
        return function<R __fastcall(Args...)>(a_Name, a_Ptr);
    }
#endif

#if PHANTOM_HAS_STDCALL
    template<class R, class... Args>
    NamespaceBuilder& function(StringView a_Name, R(__stdcall* a_Ptr)(Args...))
    {
        return function<R __stdcall(Args...)>(a_Name, a_Ptr);
    }
#endif

    NamespaceBuilder& operator[](StringView a_DefaultArg)
    {
        PHANTOM_ASSERT(m_Symbols.size(), "last declaration does not accept default arguments");
        PHANTOM_ASSERT(m_Symbols.back()->asSubroutine(), "last declaration does not accept default arguments");
        auto        pFunc = static_cast<reflection::Subroutine*>(m_Symbols.back());
        StringViews defaultArgs = pFunc->getNativeDefaultArgumentStrings();
        while (a_DefaultArg.size() && std::isblank(a_DefaultArg.front(), std::locale::classic()))
            a_DefaultArg = a_DefaultArg.substr(1);
        defaultArgs.push_back(a_DefaultArg);
        pFunc->setNativeDefaultArgumentStrings(defaultArgs);
        return *this;
    }

    template<class Sign>
    NamespaceBuilder& function(StringView a_Name, PHANTOM_TYPENAME FunctionTypeToFunctionPointerType<Sign>::type a_Ptr)
    {
        _PHNTM_REG_STATIC_ASSERT(
        IsTypeDefined<
        reflection::FunctionProviderT<PHANTOM_TYPENAME FunctionTypeToFunctionPointerType<Sign>::type>>::value,
        "missing #include <phantom/function>");
        auto pFunc =
        reflection::FunctionProviderT<PHANTOM_TYPENAME FunctionTypeToFunctionPointerType<Sign>::type>::CreateFunction(
        a_Name, reflection::SignatureH<Sign>::Create(), a_Ptr);
        _PHNTM_pNamespace->addFunction(pFunc);
        _PHNTM_pSource->addFunction(pFunc);
        m_Symbols.push_back(pFunc);
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(pFunc);
        return *this;
    }

    template<class ConstantT>
    NamespaceBuilder& constant(StringView a_Name, ConstantT a_Val)
    {
        _PHNTM_REG_STATIC_ASSERT(IsTypeDefined<reflection::ConstantT<ConstantT>>::value,
                                 "missing #include <phantom/constant>");
        auto pConst = PHANTOM_META_NEW(reflection::ConstantT<ConstantT>)(a_Name, a_Val, reflection::Modifier::None,
                                                                         PHANTOM_R_FLAG_NATIVE);
        _PHNTM_pNamespace->addConstant(pConst);
        _PHNTM_pSource->addConstant(pConst);
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(pConst);
        m_Symbols.push_back(pConst);
        return *this;
    }

    template<class T>
    NamespaceBuilder& variable(StringView a_Name, T* a_pVar)
    {
        _PHNTM_REG_STATIC_ASSERT(IsTypeDefined<reflection::VariableT<T>>::value, "missing #include <phantom/variable>");
        auto pVar = PHANTOM_META_NEW(reflection::VariableT<T>)(a_Name, a_pVar);
        _PHNTM_pNamespace->addVariable(pVar);
        _PHNTM_pSource->addVariable(pVar);
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(pVar);
        m_Symbols.push_back(pVar);
        return *this;
    }
    template<class T>
    NamespaceBuilder& variable(StringView a_Name, T& a_Var)
    {
        return variable(a_Name, &a_Var);
    }

    template<class TypedefT, class DontTouchThis = reflection::Alias>
    NamespaceBuilder& typedef_(StringView a_Name)
    {
        _PHNTM_REG_STATIC_ASSERT(phantom::IsTypeDefined<DontTouchThis>::value, "missing #include <phantom/typedef>");
        return _PHNTM_typedef(a_Name, TypeIdHash<RemoveForwardT<TypedefT>>(), PHANTOM_TYPEOF(TypedefT));
    }

    template<class MemberTypedefT>
    NamespaceBuilder& alias_(StringView a_Name)
    {
        return this->typedef_<MemberTypedefT>(a_Name);
    }

    NamespaceBuilder& using_(StringView a_Name);

    NamespaceBuilder& namespace_alias(StringView a_Name, StringView a_Namespace);

    NamespaceBuilder& operator()(StringView a_Name, Variant&& a_Value)
    {
        if (m_Symbols.empty())
            _PHNTM_pNamespace->setMetaData(a_Name, std::move(a_Value));
        else
            m_Symbols.back()->setMetaData(a_Name, std::move(a_Value));
        return *this;
    }

    NamespaceBuilder& operator()(_PHNTM_MetaDatas& a_Metas)
    {
        if (m_Symbols.empty())
            _PHNTM_pNamespace->addMetaDatas(a_Metas);
        else
            m_Symbols.back()->addMetaDatas(a_Metas);
        return *this;
    }

    reflection::Namespace* _PHNTM_getMeta() const
    {
        return _PHNTM_pNamespace;
    }

    reflection::Source* _PHNTM_getSource();
    reflection::Source* _PHNTM_getOwnerScope()
    {
        return _PHNTM_pSource;
    }
    reflection::Namespace* _PHNTM_getNamingScope()
    {
        return _PHNTM_pNamespace;
    }

    void end()
    {
    }

    _PHNTM_GlobalRegistrer* _PHNTM_getRegistrer() const
    {
        return _PHNTM_pRegistrer;
    }

private:
    void _PHNTM_setLastSymbol(reflection::Symbol* a_pSymbol)
    {
        _PHNTM_pRegistrer->_PHNTM_setLastSymbol(a_pSymbol);
    }
    NamespaceBuilder& _PHNTM_typedef(StringView a_Name, uint64_t a_Hash, Type* a_pType);

private:
    reflection::Namespace*           _PHNTM_pNamespace;
    SmallVector<PhantomBuilderBase*> _PHNTM_m_Types;
    reflection::Source*              _PHNTM_pSource;
    _PHNTM_GlobalRegistrer*          _PHNTM_pRegistrer;
    reflection::Symbols              m_Symbols;
};

template<class T>
struct NamespaceBuilderConstructOnCall
{
    NamespaceBuilderConstructOnCall(_PHNTM_GlobalRegistrer* a_pGlobalRegistrer)
        : place(false), globalReg(a_pGlobalRegistrer)
    {
    }

    T& operator()()
    {
        if (!place)
            place.construct(globalReg);
        return *place;
    }

    StaticGlobal<T>         place;
    _PHNTM_GlobalRegistrer* globalReg;
};

} // namespace reflection
} // namespace phantom

/// @brief used to insert registration code into the successive Phantom registration steps (see
/// phantom::RegistrationStep to see the different values of 'Step' you can give)

#define PHANTOM_REGISTER(...) PHANTOM_PP_VARARG_0_X(_PHNTM_REGISTER_, ##__VA_ARGS__)

#define _PHNTM_REGISTER_X(...) _PHNTM_REGISTER_X_I(__COUNTER__, ##__VA_ARGS__)
#define _PHNTM_REGISTER_X_I(Counter, ...) _PHNTM_REGISTER_X_II(Counter, ##__VA_ARGS__)
#define _PHNTM_REGISTER_X_II(Counter, ...)                                                                              \
    class _PHNTM_NSDeduce;                                                                                              \
    namespace                                                                                                           \
    {                                                                                                                   \
    struct PHANTOM_PP_CAT(_PHNTM_, Counter) : phantom::reflection::_PHNTM_GlobalRegistrer                               \
    {                                                                                                                   \
        PHANTOM_PP_CAT(_PHNTM_, Counter)                                                                                \
        ()                                                                                                              \
            : phantom::reflection::_PHNTM_GlobalRegistrer(                                                              \
              {PHANTOM_PP_ADD_PREFIX_EACH_NO_CAT(phantom::RegistrationStep::, __VA_ARGS__)},                            \
              &phantom::reflection::TypeInfosOf<_PHNTM_NSDeduce>::object, __FILE__, __LINE__, Counter, false),          \
              namespace_(this)                                                                                          \
        {                                                                                                               \
            _PHNTM_attach();                                                                                            \
        }                                                                                                               \
        virtual void _PHNTM_process(phantom::RegistrationStep a_Step)                                                   \
        {                                                                                                               \
            _PHNTM_processUserCode(a_Step);                                                                             \
            namespace_().end();                                                                                         \
        }                                                                                                               \
        inline void _PHNTM_processUserCode(phantom::RegistrationStep);                                                  \
        phantom::reflection::NamespaceBuilderConstructOnCall<phantom::reflection::NamespaceBuilder> namespace_;         \
        decltype(namespace_)&                                                                       this_ = namespace_; \
    } PHANTOM_PP_CAT(_PHNTM_i_, Counter);                                                                               \
    }                                                                                                                   \
    void PHANTOM_PP_CAT(_PHNTM_, Counter)::_PHNTM_processUserCode(phantom::RegistrationStep PHANTOM_REGISTRATION_STEP)

#define _PHNTM_REGISTER_0()                                                                                            \
    _PHNTM_REGISTER_X(Start, Namespaces, Enums, ClassTypes, PostClassTypes, TemplateSignatures, Typedefs,              \
                      PostTypedefs, PostTypes, Variables, PostVariables, Functions, End)

#define PHANTOM_NAMESPACE PHANTOM_REGISTER()
