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
    namespace lang                                                                                               \
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

#define PHANTOM_META_TYPE_T(template_types, template_params, attached_type, MetaType)                                  \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    namespace lang                                                                                               \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(template_types, template_params)>                                                          \
    struct MetaTypeOf<attached_type<PHANTOM_PP_IDENTITY template_params> >                                             \
    {                                                                                                                  \
        typedef MetaType<attached_type<PHANTOM_PP_IDENTITY template_params> > type;                                    \
    };                                                                                                                 \
    template<PHANTOM_PP_MIX(template_types, template_params)>                                                          \
    struct MetaTypeOf<MetaType<attached_type<PHANTOM_PP_IDENTITY template_params> > >                                  \
    {                                                                                                                  \
        typedef Class type;                                                                                            \
    };                                                                                                                 \
    }                                                                                                                  \
    }
