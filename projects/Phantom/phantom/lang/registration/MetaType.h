// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/lang/TypeOf.h>

#define PHANTOM_META_TYPE(attached_type, MetaType)                                                                     \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    namespace lang                                                                                                     \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct MetaTypeOf<attached_type>                                                                                   \
    {                                                                                                                  \
        typedef PHANTOM_PP_REMOVE_PARENS(MetaType) type;                                                               \
    };                                                                                                                 \
    template<>                                                                                                         \
    struct MetaTypeOf<PHANTOM_PP_REMOVE_PARENS(MetaType)>                                                              \
    {                                                                                                                  \
        typedef Class type;                                                                                            \
    };                                                                                                                 \
    }                                                                                                                  \
    }

#define PHANTOM_META_TYPE_T(...) PHANTOM_PP_VARARG(PHANTOM_META_TYPE_T_, ##__VA_ARGS__)

#define PHANTOM_META_TYPE_T_4(template_types, template_params, attached_type, MetaType)                                \
    PHANTOM_META_TYPE_T_5(template_types, template_params, attached_type, MetaType,                                    \
                          (attached_type<PHANTOM_PP_IDENTITY template_params>))
#define PHANTOM_META_TYPE_T_5(template_types, template_params, attached_type, MetaType, MetaTypeTArgs)                 \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    namespace lang                                                                                                     \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(template_types, template_params)>                                                          \
    struct MetaTypeOf<attached_type<PHANTOM_PP_IDENTITY template_params> >                                             \
    {                                                                                                                  \
        using type = MetaType<PHANTOM_PP_IDENTITY MetaTypeTArgs>;                                                      \
    };                                                                                                                 \
    template<PHANTOM_PP_MIX(template_types, template_params)>                                                          \
    struct MetaTypeOf<MetaType<PHANTOM_PP_IDENTITY MetaTypeTArgs> >                                                    \
    {                                                                                                                  \
        using type = Class;                                                                                            \
    };                                                                                                                 \
    }                                                                                                                  \
    }
