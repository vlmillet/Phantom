// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/traits/IsTypeDefined.h>
#include <phantom/utils/StringView.h>

namespace phantom
{
namespace lang
{
template<class MostDerived>
struct ScopeBuilderT
{
public:
    // class

    template<class MemberClassT>
    auto& class_()
    {
        using T = RemoveForwardT<MemberClassT>;
        using BuilderType = PHANTOM_BUILDER_TYPE(MostDerived, T);
        _PHNTM_REG_STATIC_ASSERT(std::is_class<T>::value, "class_<T> : T must be a class");
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<BuilderType>::value), "missing #include <phantom/class>");
        auto pType =
        new_<BuilderType>(BuilderReleaser{}, static_cast<MostDerived*>(this), lang::Access::Private, nullptr);
        static_cast<MostDerived*>(this)->addSubBuilder(pType);
        static_cast<MostDerived*>(this)->_PHNTM_setLastSymbol(pType->_PHNTM_getMeta());
        return *pType;
    }

    // struct ( void = anonymous )

    template<class MemberClassT = AnonymousStructProxy>
    auto& struct_()
    {
        using T = RemoveForwardT<MemberClassT>;
        using BuilderType = PHANTOM_BUILDER_TYPE(MostDerived, T);
        _PHNTM_REG_STATIC_ASSERT(std::is_class<T>::value, "struct_<T> : T must be a struct");
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<BuilderType>::value), "missing #include <phantom/struct>");
        auto pType =
        new_<BuilderType>(BuilderReleaser{}, static_cast<MostDerived*>(this), lang::Access::Public, nullptr);
        static_cast<MostDerived*>(this)->addSubBuilder(pType);
        static_cast<MostDerived*>(this)->_PHNTM_setLastSymbol(pType->_PHNTM_getMeta());
        return *pType;
    }

    // union ( void = anonymous )

    template<class MemberUnionT = AnonymousUnionProxy>
    auto& union_()
    {
        using T = RemoveForwardT<MemberUnionT>;
        using BuilderType = PHANTOM_BUILDER_TYPE(MostDerived, T);
        _PHNTM_REG_STATIC_ASSERT(std::is_union<T>::value, "union_<T> : T must be an union");
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<BuilderType>::value), "missing #include <phantom/union>");
        auto pType =
        new_<BuilderType>(BuilderReleaser{}, static_cast<MostDerived*>(this), lang::Access::Public, nullptr);
        static_cast<MostDerived*>(this)->addSubBuilder(pType);
        static_cast<MostDerived*>(this)->_PHNTM_setLastSymbol(pType->_PHNTM_getMeta());
        return *pType;
    }

    // enum

    template<class MemberEnumT = int>
    auto& enum_()
    {
        using T = RemoveForwardT<MemberEnumT>;
        using BuilderType = PHANTOM_BUILDER_TYPE(MostDerived, T);
        _PHNTM_REG_STATIC_ASSERT((std::is_integral<T>::value || std::is_enum<T>::value),
                                 "enum_<T> : T an enum or integral type for anonymous enums");
        _PHNTM_REG_STATIC_ASSERT((phantom::IsTypeDefined<BuilderType>::value), "missing #include <phantom/enum>");
        auto pType = new_<BuilderType>(BuilderReleaser{}, static_cast<MostDerived*>(this));
        static_cast<MostDerived*>(this)->addSubBuilder(pType);
        static_cast<MostDerived*>(this)->_PHNTM_setLastSymbol(pType->_PHNTM_getMeta());
        return *pType;
    }
};

} // namespace lang
} // namespace phantom
