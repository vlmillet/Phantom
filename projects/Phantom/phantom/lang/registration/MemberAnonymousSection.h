// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "Type.h"

#include <phantom/lang/MemberAnonymousStruct.h>
#include <phantom/lang/MemberAnonymousUnion.h>

namespace phantom
{
namespace lang
{
template<class T>
struct MemberAnonymousSectionRoot
{
    static T* Root(T* a_pTop) { return a_pTop; }
};

template<class Meta, class Top>
struct MemberAnonymousSectionRoot<MemberAnonymousSectionBuilderT<Meta, Top>>
{
    static auto Root(MemberAnonymousSectionBuilderT<Meta, Top>* a_pTop)
    {
        return MemberAnonymousSectionRoot<Top>::Root(&a_pTop->end());
    }
};

template<class Meta, class Top>
struct MemberAnonymousSectionBuilderT : PhantomBuilderBase
{
    _PHNTM_REG_FRIENDS;

    using SelfType = MemberAnonymousSectionBuilderT<Meta, Top>;
    using BaseType = TypeBuilderBase;
    using MostDerived = SelfType;
    using TopType = Top;
    using ReflectedType = PHANTOM_TYPENAME    Top::ReflectedType;
    using BuilderProxyType = PHANTOM_TYPENAME Top::_PHNTM_Proxy;

    MemberAnonymousSectionBuilderT(Top* a_pTop)
        : m_pMeta(a_pTop->_PHNTM_getOwnerScope()->New<Meta>(lang::Modifier::None, PHANTOM_R_FLAG_NATIVE)),
          m_pTop(a_pTop)
    {
        m_pTop->_PHNTM_getOwnerScope()->addMemberAnonymousSection(m_pMeta);
    }

    ~MemberAnonymousSectionBuilderT() override
    {
        for (auto pSec : m_MASections)
        {
            phantom::deleteVirtual(pSec);
        }
    }

    Meta* _PHNTM_getOwnerScope() { return m_pMeta; }

    // member anonymous struct

    template<class DontTouchThis = void>
    MemberAnonymousSectionBuilderT<lang::MemberAnonymousStruct, SelfType>& struct_()
    {
        _PHNTM_REG_STATIC_ASSERT((std::is_same<DontTouchThis, void>::value),
                                 "struct_<>() requires empty template signature if no name provided");
        auto pSec = PHANTOM_NEW(MemberAnonymousSectionBuilderT<lang::MemberAnonymousStruct, SelfType>)(this);
        this->m_MASections.push_back(pSec);
        return *pSec;
    }

    // member anonymous union

    template<class DontTouchThis = void>
    MemberAnonymousSectionBuilderT<lang::MemberAnonymousUnion, SelfType>& union_()
    {
        _PHNTM_REG_STATIC_ASSERT(std::is_same<DontTouchThis, void>::value,
                                 "union_<>() requires empty template signature if no name provided");
        auto pSec = PHANTOM_NEW(MemberAnonymousSectionBuilderT<lang::MemberAnonymousUnion, SelfType>)(this);
        this->m_MASections.push_back(pSec);
        return *pSec;
    }

    template<class ValueType, int Modifiers = 0>
    MostDerived& field(StringView a_Name, ValueType(ReflectedType::*a_FPtr), uint a_FilterMask = ~uint(0))
    {
        using MetaType = Meta;
        using FieldPtrT = decltype(a_FPtr);
        _PHNTM_REG_STATIC_ASSERT(phantom::IsTypeDefined<lang::FieldT<ValueType(ReflectedType::*)>>::value,
                                 "missing #include <phantom/field>");
        _root()->_addField(
        m_pMeta, a_Name, PHANTOM_REG_MEMBER_FORWARD_ARG(a_FPtr), a_FilterMask, [](MemberBuilder const& a_Member) {
            static_cast<MetaType*>(a_Member.owner)
            ->addField(a_Member.apply(a_Member.classType()->NewMeta<lang::FieldT<ValueType(ReflectedType::*)>>(
            PHANTOM_TYPEOF(ValueType), a_Member.name, PHANTOM_REG_MEMBER_GETBACK_ARG(0, FieldPtrT), a_Member.filter,
            lang::Modifiers(Modifiers))));
        });
        return *this;
    }

    Top& end() { return *m_pTop; }

    Meta* _PHNTM_getMeta() const { return m_pMeta; }

private:
    auto _root() { return MemberAnonymousSectionRoot<Top>::Root(m_pTop); }

private:
    Top*                             m_pTop;
    Meta*                            m_pMeta;
    SmallVector<PhantomBuilderBase*> m_MASections;
};
} // namespace lang
} // namespace phantom
