#pragma once

#include <phantom/macros.h>

#define PHANTOM_DECLARE_TEMPLATE_PARAM_VALUE_TYPE(...)                                             \
    PHANTOM_PP_VARARG(_PHANTOM_DECLARE_TEMPLATE_PARAM_VALUE_TYPE_, ##__VA_ARGS__)

#define _PHANTOM_DECLARE_TEMPLATE_PARAM_VALUE_TYPE_1(TypeName)                                     \
    constexpr TypeName PHANTOM_PP_CAT(PHANTOM_TEMPLATE_VALUE_PARAM_, TypeName)(TypeName v)         \
    {                                                                                              \
        return v;                                                                                  \
    }

#define _PHANTOM_DECLARE_TEMPLATE_PARAM_VALUE_TYPE_2(Namespace, TypeName)                          \
    namespace PHANTOM_PP_CAT(PHANTOM_TEMPLATE_VALUE_PARAM_, PHANTOM_PP_ARG_0 Namespace)            \
    {                                                                                              \
        PHANTOM_OPEN_NAMESPACE(PHANTOM_PP_SUBLIST Namespace)                                       \
        constexpr PHANTOM_PP_CREATE_SCOPE Namespace ::TypeName PHANTOM_PP_CAT(                     \
        PHANTOM_TEMPLATE_VALUE_PARAM_, TypeName)(PHANTOM_PP_CREATE_SCOPE Namespace ::TypeName v)   \
        {                                                                                          \
            return v;                                                                              \
        }                                                                                          \
        PHANTOM_CLOSE_NAMESPACE Namespace
