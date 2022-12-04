// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/lang/MetaNewDelete.h>
#include <phantom/lang/MethodProviderT.h>
#include <phantom/lang/MethodT.h>
#include <phantom/lang/SignatureH.h>
#include <phantom/traits/FunctionTypeToMethodPointerType.h>

namespace phantom
{
namespace lang
{
template<class T, class Sign, class MethodPtrT, int Modifiers>
struct MethodH
{
    static lang::Method* Create(StringView a_Name, MethodPtrT a_Ptr) {}
};

} // namespace lang
} // namespace phantom

#define PHANTOM_METHOD(...) PHANTOM_PP_VARARG(_PHNTM_METHOD_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_METHOD(...) PHANTOM_PP_VARARG(_PHNTM_T_METHOD_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA

#define _PHNTM_METHOD_1(name) (this_()).method(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_METHOD_2(name, modifiers) (this_()).method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_METHOD_3(ret, name, params)                                                                             \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_()).method<_PHNTM_MthdFnT>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);                                \
    }
#define _PHNTM_METHOD_4(ret, name, params, modifiers)                                                                  \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_()).method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);                     \
    }
#define _PHNTM_METHOD_5(ret, name, params, modifiers, def)                                                             \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret                                                                         params;     \
        (this_()).method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name).defaults def;        \
    }

#define _PHNTM_T_METHOD_1(name) (this_()).PHANTOM_T method(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_T_METHOD_2(name, modifiers)                                                                             \
    (this_()).PHANTOM_T method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);

#define _PHNTM_T_METHOD_3(ret, name, params)                                                                           \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_()).PHANTOM_T        method<_PHNTM_MthdFnT>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);               \
    }
#define _PHNTM_T_METHOD_4(ret, name, params, modifiers)                                                                \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_()).PHANTOM_T        method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);    \
    }
#define _PHNTM_T_METHOD_5(ret, name, params, modifiers, def)                                                           \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_())                                                                                                      \
        .PHANTOM_T method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name)                     \
        .defaults  def;                                                                                                \
    }

#define PHANTOM_ASSIGN_OP(...) PHANTOM_PP_VARARG(_PHNTM_ASSIGN_OP_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_ASSIGN_OP(...) PHANTOM_PP_VARARG(_PHNTM_T_ASSIGN_OP_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA

#define _PHNTM_ASSIGN_OP_1(name) (this_()).method(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_ASSIGN_OP_2(name, modifiers)                                                                            \
    (this_()).method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_ASSIGN_OP_3(ret, name, params)                                                                          \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_()).method<_PHNTM_MthdFnT>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);                       \
    }
#define _PHNTM_ASSIGN_OP_4(ret, name, params, modifiers)                                                               \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_()).method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);            \
    }
#define _PHNTM_ASSIGN_OP_5(ret, name, params, modifiers, def)                                                          \
    {                                                                                                                  \
        using _PHNTM_MthdFnT = ret params;                                                                             \
        (this_())                                                                                                      \
        .method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name)                      \
        .defaults def;                                                                                                 \
    }

#define _PHNTM_T_ASSIGN_OP_1(name) (this_()).PHANTOM_T method(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_2(name, modifiers)                                                                          \
    (this_()).PHANTOM_T method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_3(ret, name, params)                                                                        \
    using _PHNTM_MthdFnT = ret params;                                                                                 \
    (this_()).PHANTOM_T        method<_PHNTM_MthdFnT>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_4(ret, name, params, modifiers)                                                             \
    using _PHNTM_MthdFnT = ret params;                                                                                 \
    (this_()).PHANTOM_T        method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_5(ret, name, params, modifiers, def)                                                        \
    using _PHNTM_MthdFnT = ret params;                                                                                 \
    (this_())                                                                                                          \
    .PHANTOM_T method<_PHNTM_MthdFnT, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name)                \
    .defaults  def;
