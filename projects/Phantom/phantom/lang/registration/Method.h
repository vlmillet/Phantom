// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/reflection/MetaNewDelete.h>
#include <phantom/reflection/MethodProviderT.h>
#include <phantom/reflection/MethodT.h>
#include <phantom/reflection/SignatureH.h>
#include <phantom/traits/FunctionTypeToMethodPointerType.h>

namespace phantom
{
namespace reflection
{
template<class T, class Sign, class MethodPtrT, int Modifiers>
struct MethodH
{
    static reflection::Method* Create(StringView a_Name, MethodPtrT a_Ptr)
    {
    }
};

} // namespace reflection
} // namespace phantom

#define PHANTOM_METHOD(...) PHANTOM_PP_VARARG(_PHNTM_METHOD_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_METHOD(...) PHANTOM_PP_VARARG(_PHNTM_T_METHOD_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA

#define _PHNTM_METHOD_1(name) (this_()).method(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_METHOD_2(name, modifiers) (this_()).method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_METHOD_3(ret, name, params) (this_()).method<ret params>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_METHOD_4(ret, name, params, modifiers)                                                                  \
    (this_()).method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_METHOD_5(ret, name, params, modifiers, def)                                                             \
    (this_()).method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name).defaults def;

#define _PHNTM_T_METHOD_1(name) (this_()).PHANTOM_T method(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_T_METHOD_2(name, modifiers)                                                                             \
    (this_()).PHANTOM_T method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_T_METHOD_3(ret, name, params)                                                                           \
    (this_()).PHANTOM_T method<ret params>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_T_METHOD_4(ret, name, params, modifiers)                                                                \
    (this_()).PHANTOM_T method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name);
#define _PHNTM_T_METHOD_5(ret, name, params, modifiers, def)                                                           \
    (this_()).PHANTOM_T method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_SCOPE::name).defaults def;

#define PHANTOM_ASSIGN_OP(...) PHANTOM_PP_VARARG(_PHNTM_ASSIGN_OP_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA
#define PHANTOM_T_ASSIGN_OP(...) PHANTOM_PP_VARARG(_PHNTM_T_ASSIGN_OP_, ##__VA_ARGS__) _PHNTM_MTRAILING_METADATA

#define _PHNTM_ASSIGN_OP_1(name) (this_()).method(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_ASSIGN_OP_2(name, modifiers)                                                                            \
    (this_()).method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_ASSIGN_OP_3(ret, name, params)                                                                          \
    (this_()).method<ret params>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_ASSIGN_OP_4(ret, name, params, modifiers)                                                               \
    (this_()).method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_ASSIGN_OP_5(ret, name, params, modifiers, def)                                                          \
    (this_()).method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name).defaults def;

#define _PHNTM_T_ASSIGN_OP_1(name) (this_()).PHANTOM_T method(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_2(name, modifiers)                                                                          \
    (this_()).PHANTOM_T method<modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_3(ret, name, params)                                                                        \
    (this_()).PHANTOM_T method<ret params>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_4(ret, name, params, modifiers)                                                             \
    (this_()).PHANTOM_T method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name);
#define _PHNTM_T_ASSIGN_OP_5(ret, name, params, modifiers, def)                                                        \
    (this_())                                                                                                          \
    .PHANTOM_T method<ret params, modifiers>(PHANTOM_PP_QUOTE(name), &PHANTOM_REFLECTED_TYPE::name)                    \
    .defaults  def;
