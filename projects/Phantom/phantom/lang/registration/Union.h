// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "ClassType.h"

#include <phantom/lang/UnionT.h>

namespace phantom
{
namespace lang
{
template<class T, class Top, class MostDerived>
struct UnionBuilderT : ClassTypeBuilderT<T, Top, MostDerived>
{
    _PHNTM_REG_STATIC_ASSERT(phantom::IsTypeDefined<UnionT<T>>::value, "missing #include <phantom/union>");
    _PHNTM_REG_FRIENDS;

public:
    using SelfType = UnionBuilderT<T, Top, MostDerived>;
    using BaseType = ClassTypeBuilderT<T, Top, MostDerived>;
    using TopType = Top;

    UnionBuilderT(BuilderReleaser _releaser, Top* a_pTop, phantom::lang::TemplateSpecArgumentRegistrer a_TArgs)
        : BaseType(_releaser, lang::Access::Public, a_pTop, a_TArgs)
    {
        m_pUnion = this->_PHNTM_getMeta();
    }

private:
    typename BaseType::MetaType* m_pUnion;
};

template<class Top, class MostDerived>
struct UnionBuilderT<AnonymousUnionProxy, Top, MostDerived>
    : MemberAnonymousSectionBuilderT<lang::MemberAnonymousUnion, Top>
{
    UnionBuilderT(BuilderReleaser _releaser, Top* a_pTop, TemplateSpecArgumentRegistrer)
        : MemberAnonymousSectionBuilderT<lang::MemberAnonymousUnion, Top>(_releaser, a_pTop)
    {
    }
};

template<class T, class Top>
struct DefaultUnionBuilderT : UnionBuilderT<T, Top, DefaultUnionBuilderT<T, Top>>
{
    using BaseType = UnionBuilderT<T, Top, DefaultUnionBuilderT<T, Top>>;
    DefaultUnionBuilderT(BuilderReleaser _releaser, Top* a_pTop, Access, TemplateSpecArgumentRegistrer a_TArgs)
        : BaseType(_releaser, a_pTop, a_TArgs)
    {
    }
};

} // namespace lang
} // namespace phantom

#define PHANTOM_UNION(TypeName, ...) _PHANTOM_CLASS(TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_T(TemplateTypes, TemplateParams, TypeName, ...)                                                  \
    _PHANTOM_CLASS_T(TemplateTypes, TemplateParams, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_S(TemplateArgs, TypeName, ...) _PHANTOM_CLASS_S(TemplateArgs, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, ...)                                   \
    _PHANTOM_CLASS_TS(TemplateTypes, TemplateParams, TemplateArgs, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_V(VariadicPair, TypeName, ...) _PHANTOM_CLASS_V(VariadicPair, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, ...)                                   \
    _PHANTOM_CLASS_TV(TemplateTypes, TemplateParams, VariadicPair, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_VS(VariadicPair, TemplateArgs, TypeName, ...)                                                    \
    _PHANTOM_CLASS_VS(VariadicPair, TemplateArgs, TypeName, Public, (__VA_ARGS__))
#define PHANTOM_UNION_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, ...)                    \
    _PHANTOM_CLASS_TVS(TemplateTypes, TemplateParams, VariadicPair, TemplateArgs, TypeName, Public, (__VA_ARGS__))

#define PHANTOM_MUNION(...) PHANTOM_MTYPE(union, ##__VA_ARGS__)
