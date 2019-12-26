// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "Scope.h"

#include <phantom/Delegate.h>
#include <phantom/StringView.h>
#include <phantom/StringViews.h>
#include <phantom/TypeInstallationInfo.h>
#include <phantom/Variant.h>
#include <phantom/reflection/MetaNewDelete.h>
#include <phantom/reflection/Symbol.h>
#include <phantom/traits/IsTypeDefined.h>
#include <phantom/traits/SelectType.h>

#define PHANTOM_REFLECTED_TYPE                                                                                         \
    phantom::reflection::ReflectedTypeOfT<std::remove_reference_t<decltype(_PHNTM_IDTTY(this_()))>>

namespace phantom
{
namespace reflection
{
struct MemberBuilder;
using MemberRegistrer = void (*)(MemberBuilder const&);
using TemplateSpecArgumentRegistrer = LanguageElements (*)();
using InheritanceRegistrer = void (*)(reflection::Class*);

template<class Builder>
using ReflectedTypeOfT = PHANTOM_TYPENAME Builder::ReflectedType;

struct ArgFwd
{
    void* d[4];
};

template<size_t s>
struct ArgFwdT
{
    static_assert(s < (sizeof(void*) * 4), "invalid arg forwarding, too big");
    ArgFwdT(void* ptr)
    {
        memcpy(fwd.d, ptr, s);
    }

    operator ArgFwd() const
    {
        return fwd;
    }
    ArgFwd fwd;
};

#define PHANTOM_REG_MEMBER_FORWARD_ARG(any) phantom::reflection::ArgFwdT<sizeof(any)>(&any)
#define PHANTOM_REG_MEMBER_GETBACK_ARG(idx, any) (*(any*)(&a_Member.fwdArgs[idx]))

struct PHANTOM_EXPORT_PHANTOM MemberBuilder
{
    MemberRegistrer        registrer;
    String                 name;
    Symbol*                owner;
    MetaDatas              metaDatas;
    Strings                annotations;
    StringViews            defaultArguments;
    SmallVector<ArgFwd, 4> fwdArgs;
    uint                   filter;
    bool                   isSymbol;
    bool                   isFunc;
    reflection::Modifiers  modifiers;
    uint                   flags;

    ClassType* classType() const;
    Class*     class_() const;

    template<class T>
    T* apply(T* a_pSymbol) const
    {
        _apply(a_pSymbol);
        return a_pSymbol;
    }

private:
    void _apply(reflection::Symbol* a_pSymbol) const;
    void _apply(reflection::Subroutine* a_pSubroutine) const;
};

struct PHANTOM_EXPORT_PHANTOM TypeBuilderBase : PhantomBuilderBase
{
    PHANTOM_DECL_ABSTRACT_DELETE_METHOD(TypeBuilderBase);
    _PHNTM_REG_FRIENDS;

protected:
    TypeBuilderBase(reflection::Source* a_pSource, Scope* a_pNamingScope, Type* a_pType,
                    TemplateSpecArgumentRegistrer a_Arguments);
    TypeBuilderBase(reflection::Scope*, Scope* a_pNamingScope, Type* a_pType,
                    TemplateSpecArgumentRegistrer a_Arguments);

    virtual void _installFunc(reflection::Type* a_pType, TypeInstallationStep a_Step);
    void         _registerTypeInstallationInfo(TypeInstallationInfo* a_pTii);
    Template*    _getClassTemplate(reflection::ClassType* a_pClass, Namespace* a_pScope);
    Template*    _getClassTemplate(reflection::ClassType* a_pClass, Scope* a_pScope);
    void         _splitEnumValues(StringView a_NameList, StringViews& a_Names);

    // meta data

    void operator()(reflection::MetaDatas&& a_MD);

    void operator()(StringView a_Name, Variant&& a_Value);
    void operator()(StringView a_Annot);
    void operator()(uint a_Flags);
    void operator()(reflection::Modifiers a_Modifiers);

    void addMember(reflection::Symbol* a_pOwner, MemberBuilder&& a_Member);
    void addMember(reflection::Symbol* a_pOwner, StringView a_Name, std::initializer_list<ArgFwd> a_Args,
                   uint a_UserInt, MemberRegistrer a_Reg);

private:
    ClassType* _PHNTM_getOwnerScope();
    ClassType* _PHNTM_getNamingScope();
    void       _PHNTM_setLastSymbol(reflection::Symbol*)
    {
    } // skip

protected:
    void _addAccess(reflection::Symbol* a_pOwner, Access _access);
    void _addSymbol(reflection::Symbol* a_pOwner, StringView a_Name, MemberRegistrer a_Reg);
    void _addSymbol(reflection::Symbol* a_pOwner, StringView a_Name, std::initializer_list<ArgFwd> a_Args,
                    MemberRegistrer a_Reg);
    void _addCtor(reflection::Symbol* a_pOwner, MemberRegistrer a_Reg);
    void _addFunc(reflection::Symbol* a_pOwner, StringView a_Name, ArgFwd a_Arg, MemberRegistrer a_Reg);
    void _addMethod(reflection::Symbol* a_pOwner, StringView a_Name, ArgFwd a_Arg, MemberRegistrer a_Reg);
    void _addField(reflection::Symbol* a_pOwner, StringView a_Name, ArgFwd a_Arg, uint a_Filter, MemberRegistrer a_Reg);
    void _addCustom(reflection::Symbol* a_pOwner, std::initializer_list<ArgFwd> a_Arg, uint a_UserInt,
                    MemberRegistrer a_Reg);
    void _addProperty(reflection::Symbol* a_pOwner, StringView a_Name, std::initializer_list<ArgFwd> a_Args,
                      uint a_Filter, MemberRegistrer a_Reg);

    Scope*                         m_pNamingScope;
    SmallVector<MemberBuilder, 32> m_Members;
    TypeInstallationInfo           m_TypeInstallationInfo;
    MetaDatas                      m_MetaDatas;
    Strings                        m_Annotations;
    uint                           m_Flags = 0;
    reflection::Modifiers          m_Modifiers = 0;
    TemplateSpecArgumentRegistrer  m_TemplateSpecArgumentRegistrer;
    InheritanceRegistrer           m_Inheritance = nullptr;
};

template<class T, class Top, class MostDerived, class Meta = PHANTOM_TYPENAME MetaTypeOf<T>::type>
struct TypeBuilderT : TypeBuilderBase
{
    _PHNTM_REG_FRIENDS;

    using MetaType = Meta;
    using BaseType = TypeBuilderBase;
    using SelfType = TypeBuilderT<T, Top, MostDerived, Meta>;
    using ReflectedType = T;

    PHANTOM_DECL_VIRTUAL_DELETE_METHOD(SelfType);

    TypeBuilderT(Top* a_pTop, TemplateSpecArgumentRegistrer a_Arguments)
        : TypeBuilderBase(a_pTop->_PHNTM_getOwnerScope(), a_pTop->_PHNTM_getNamingScope(),
                          _createMetaType(std::is_fundamental<T>::value ? "" : TypeInfosOf<T>::object().name()),
                          a_Arguments),
          m_pTop(a_pTop)
    {
        _installRtti();
    }

    MetaType* _PHNTM_getMeta() const
    {
        return m_pType;
    }

    PHANTOM_TYPENAME SelectType<std::is_same<Top, _PHNTM_GlobalRegistrer>::value, void, Top>::type& end()
    {
        PHANTOM_ASSERT(!m_Ended);
        m_Ended = true;
        _registerTypeInstallationInfo(&m_TypeInstallationInfo);
        return *m_pTop;
    }

    MostDerived& this_()
    {
        return static_cast<MostDerived&>(*this);
    }
    MostDerived& this_(reflection::MetaDatas&& a_MD)
    {
        m_MetaDatas = std::move(a_MD);
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()()
    {
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()(reflection::MetaDatas&& a_MD)
    {
        BaseType::operator()(std::move(a_MD));
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()(StringView a_Name, Variant&& a_Value)
    {
        BaseType::operator()(a_Name, std::move(a_Value));
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()(StringView a_Name)
    {
        BaseType::operator()(a_Name);
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()(reflection::Modifiers a_Modifiers)
    {
        BaseType::operator()(a_Modifiers);
        return static_cast<MostDerived&>(*this);
    }

    MostDerived& operator()(uint a_Flags)
    {
        BaseType::operator()(a_Flags);
        return static_cast<MostDerived&>(*this);
    }

private:
    MetaType* _createMetaType(StringView a_Name)
    {
        return m_pType = (new (PHANTOM_ALLOCATE(MetaType)) MetaType(a_Name));
    }
    void _installRtti()
    {
        auto pMetaClass = MetaType::MetaClass();
        if (pMetaClass == nullptr)
        {
            PHANTOM_ASSERT((std::is_same<MetaType, PHANTOM_TYPENAME MetaTypeOf<reflection::Class>::type>::value),
                           "you probably forgot a PHANTOM_DECLARE_META_CLASS() for your derivation of Class");
            pMetaClass = Class::metaClass =
            (decltype(pMetaClass))(m_pType); /// meta meta type (the meta type of the meta type of
                                             /// 'Class' is the meta type of class itself)
        }
        m_pType->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance = m_pType;
        m_pType->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.dynamicDeleteFunc = &DynamicProxyDeleter<MetaType>::dynamicDelete;
        m_pType->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass = pMetaClass;
        pMetaClass->registerInstance(m_pType);
    }

protected:
    MetaType* m_pType;
    Top*      m_pTop;
    bool      m_Ended = false;
};

template<class T, class Ret = decltype(_PHNTM_TemplateArgsADL((T**)0))>
auto TemplateArgsOfADL(T** t)
{
    return _PHNTM_TemplateArgsADL(t);
}
inline std::nullptr_t TemplateArgsOfADL(void*)
{
    return nullptr;
}
} // namespace reflection

} // namespace phantom

#define PHANTOM_MTYPE(keyword, ...) PHANTOM_PP_VARARG_1_X(_PHNTM_MTYPE_, keyword, ##__VA_ARGS__)

#define _PHNTM_MTYPE_1(keyword)                                                                                        \
    auto& PHANTOM_PP_CAT(keyword, __LINE__) = this_();                                                                 \
    if (auto this_ =                                                                                                   \
        phantom::reflection::makeScopedIf(PHANTOM_PP_CAT(keyword, __LINE__).PHANTOM_PP_CAT(keyword, _) < > ()))

#define _PHNTM_MTYPE_X(keyword, Name, ...)                                                                             \
    auto& PHANTOM_PP_CAT(keyword, __LINE__) = this_();                                                                 \
    if (auto this_ = phantom::reflection::makeScopedIf(                                                                \
        PHANTOM_PP_CAT(keyword, __LINE__).PHANTOM_PP_CAT(keyword, _) <                                                 \
        phantom::reflection::BuilderProxyTypeT<decltype(PHANTOM_PP_CAT(keyword, __LINE__))>::Name > () __VA_ARGS__))

#define PHANTOM_T_MTYPE(keyword, ...) PHANTOM_PP_VARARG_1_X(_PHNTM_T_MTYPE_, keyword, ##__VA_ARGS__)

#define _PHNTM_T_MTYPE_1(keyword)                                                                                      \
    auto& PHANTOM_PP_CAT(keyword, __LINE__) = this_();                                                                 \
    if (auto this_ = phantom::reflection::makeScopedIf(                                                                \
        PHANTOM_PP_CAT(keyword, __LINE__).PHANTOM_T PHANTOM_PP_CAT(keyword, _) < > ()))

#define _PHNTM_T_MTYPE_X(keyword, Name, ...)                                                                           \
    auto& PHANTOM_PP_CAT(keyword, __LINE__) = this_();                                                                 \
    if (auto this_ = phantom::reflection::makeScopedIf(                                                                \
        PHANTOM_PP_CAT(keyword, __LINE__).PHANTOM_T PHANTOM_PP_CAT(keyword, _) <                                       \
        PHANTOM_TYPENAME phantom::reflection::BuilderProxyTypeT<decltype(PHANTOM_PP_CAT(keyword, __LINE__))>::Name >   \
        () __VA_ARGS__))
