// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/fwd.h>

/// @cond ADVANCED

#define PHANTOM_TRAITS_OP_T(_trait_, l0, op)                                                                           \
    PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN _PHNTM_TRTS_OP_T_,                                                            \
    PHANTOM_PP_ARGCOUNT_FUNC() l0        PHANTOM_PP_RIGHT_PAREN(_trait_, l0, op)

#define _PHNTM_TRTS_OP_T_1(_trait_, l0, op) _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_0, l0)>::value
#define _PHNTM_TRTS_OP_T_2(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_1(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_1, l0)>::value
#define _PHNTM_TRTS_OP_T_3(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_2(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_2, l0)>::value
#define _PHNTM_TRTS_OP_T_4(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_3(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_3, l0)>::value
#define _PHNTM_TRTS_OP_T_5(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_4(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_4, l0)>::value
#define _PHNTM_TRTS_OP_T_6(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_5(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_5, l0)>::value
#define _PHNTM_TRTS_OP_T_7(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_6(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_6, l0)>::value
#define _PHNTM_TRTS_OP_T_8(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_7(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_7, l0)>::value
#define _PHNTM_TRTS_OP_T_9(_trait_, l0, op)                                                                            \
    _PHNTM_TRTS_OP_T_8(_trait_, l0, op) op _trait_<PHANTOM_PP_CAT(PHANTOM_PP_ARG_8, l0)>::value

#define _PHNTM_TRTS_SPEC_NT(_trait_, _type_, _value_, _namespaces_, _template_types_, _template_params_, _name_)       \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_)>                                                      \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_namespaces_, _name_) < PHANTOM_PP_IDENTITY _template_params_>>    \
    final : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                   \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NPT(_trait_, _type_, _value_, _namespaces_, _template_types_, _template_params_,              \
                             _template_spec_, _name_)                                                                  \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_)>                                                      \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_namespaces_, _name_) < PHANTOM_PP_IDENTITY _template_spec_>>      \
    final : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                   \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_T(_trait_, _type_, _value_, _template_types_, _template_params_, _name_)                      \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_)>                                                      \
    struct _trait_< ::_name_<PHANTOM_PP_IDENTITY _template_params_> > final                                            \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_TP(_trait_, _type_, _value_, _template_types_, _template_params_, _spec_params_, _name_)      \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_)>                                                      \
    struct _trait_< ::_name_<PHANTOM_PP_IDENTITY _spec_params_> > final                                                \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_TF(_trait_, _type_, _value_, _spec_params_, _name_)                                           \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct _trait_< ::_name_<PHANTOM_PP_IDENTITY _template_spec_> > final                                              \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_CT(_trait_, _type_, _value_, _classes_, _template_types_, _template_params_, _name_)          \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_)>                                                      \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_classes_, _name_) < PHANTOM_PP_IDENTITY _template_params_>> final \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_N(_trait_, _type_, _value_, _namespaces_, _name_)                                             \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_namespaces_, _name_)> final                                       \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_C(_trait_, _type_, _value_, _classes_, _name_)                                                \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_classes_, _name_)> final                                          \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NC(_trait_, _type_, _value_, _namespaces_, _classes_, _name_)                                 \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME_2(_namespaces_, _classes_, _name_)> final                          \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NCT(_trait_, _type_, _value_, _namespaces_, _classes_, _template_types_, _template_params_,   \
                             _name_)                                                                                   \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_)>                                                      \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME_2(_namespaces_, _classes_, _name_) <                               \
                   PHANTOM_PP_IDENTITY _template_params_>> final                                                       \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_(_trait_, _type_, _value_, _name_)                                                            \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct _trait_< ::_name_> final : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>         \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, _name_, _type_, _value_)                                                \
    template<>                                                                                                         \
    struct _trait_<_name_> final : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>            \
    {                                                                                                                  \
    };

#define _PHNTM_TRTS_SPEC_TV(_trait_, _type_, _value_, _template_types_, _template_params_, _variadic_pair_, _name_)    \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_),                                                      \
             PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_< ::_name_<PHANTOM_PP_IDENTITY _template_params_, PHANTOM_PP_ARG_1 _variadic_pair_...> > final       \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_V(_trait_, _type_, _value_, _variadic_pair_, _name_)                                          \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_< ::_name_<PHANTOM_PP_ARG_1 _variadic_pair_...> > final                                              \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_CTV(_trait_, _type_, _value_, _classes_, _template_types_, _template_params_,                 \
                             _variadic_pair_, _name_)                                                                  \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_),                                                      \
             PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_classes_, _name_) < PHANTOM_PP_IDENTITY _template_params_,        \
                   PHANTOM_PP_ARG_1                                                          _variadic_pair_...>> final                                                         \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_CV(_trait_, _type_, _value_, _classes_, _variadic_pair_, _name_)                              \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_classes_, _name_) < PHANTOM_PP_ARG_1 _variadic_pair_...>> final   \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NTV(_trait_, _type_, _value_, _namespaces_, _template_types_, _template_params_,              \
                             _variadic_pair_, _name_)                                                                  \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_),                                                      \
             PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_namespaces_, _name_) < PHANTOM_PP_IDENTITY _template_params_,     \
                   PHANTOM_PP_ARG_1 _variadic_pair_...>> final                                                         \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NV(_trait_, _type_, _value_, _namespaces_, _variadic_pair_, _name_)                           \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME(_namespaces_, _name_) < PHANTOM_PP_ARG_1 _variadic_pair_...>>      \
    final : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                   \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NCTV(_trait_, _type_, _value_, _namespaces_, _classes_, _template_types_, _template_params_,  \
                              _variadic_pair_, _name_)                                                                 \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(_template_types_, _template_params_),                                                      \
             PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME_2(_namespaces_, _classes_, _name_) <                               \
                   PHANTOM_PP_IDENTITY _template_params_,                                                              \
                   PHANTOM_PP_ARG_1    _variadic_pair_...>> final                                                         \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define _PHNTM_TRTS_SPEC_NCV(_trait_, _type_, _value_, _namespaces_, _classes_, _variadic_pair_, _name_)               \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_ARG_0 _variadic_pair_... PHANTOM_PP_ARG_1 _variadic_pair_>                                     \
    struct _trait_<PHANTOM_PP_CREATE_QUALIFIED_NAME_2(_namespaces_, _classes_, _name_) <                               \
                   PHANTOM_PP_ARG_1 _variadic_pair_...>> final                                                         \
        : public std::integral_constant<_type_, PHANTOM_PP_REMOVE_PARENS(_value_)>                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#if PHANTOM_HAS_BUILT_IN_WCHAR_T
#    define _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_WCHAR_T(_trait_, _name_, _type_, _value_)                                 \
        _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, _name_, _type_, _value_)
#else
#    define _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_WCHAR_T(_trait_, _name_, _type_, _value_)
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR16_T
#    define _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_CHAR16_T(_trait_, _name_, _type_, _value_)                                \
        _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, _name_, _type_, _value_)
#else
#    define _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_CHAR16_T(_trait_, _name_, _type_, _value_)
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR32_T
#    define _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_CHAR32_T(_trait_, _name_, _type_, _value_)                                \
        _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, _name_, _type_, _value_)
#else
#    define _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_CHAR32_T(_trait_, _name_, _type_, _value_)
#endif

#define _PHNTM_TRTS_SPEC__FOR_FUNDAMENTALS(_trait_, _type_, _value_)                                                   \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, char, _type_, _value_)                                                      \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, uchar, _type_, _value_)                                                     \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, schar, _type_, _value_)                                                     \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_WCHAR_T(_trait_, wchar_t, _type_, _value_)                                        \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_CHAR16_T(_trait_, char16_t, _type_, _value_)                                      \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL_IF_CHAR32_T(_trait_, char32_t, _type_, _value_)                                      \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, short, _type_, _value_)                                                     \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, ushort, _type_, _value_)                                                    \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, int, _type_, _value_)                                                       \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, uint, _type_, _value_)                                                      \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, long, _type_, _value_)                                                      \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, ulong, _type_, _value_)                                                     \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, longlong, _type_, _value_)                                                  \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, ulonglong, _type_, _value_)                                                 \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, float, _type_, _value_)                                                     \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, double, _type_, _value_)                                                    \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, longdouble, _type_, _value_)                                                \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, bool, _type_, _value_)                                                      \
    _PHNTM_TRTS_SPEC__FUNDAMENTAL(_trait_, void, _type_, _value_)

namespace phantom
{
template<typename T>
struct move_if_rvalue
{
    PHANTOM_FORCEINLINE static T apply(T a) { return a; }
};

#if PHANTOM_HAS_RVALUE_REFERENCES
template<typename T>
struct move_if_rvalue<T&&>
{
    PHANTOM_FORCEINLINE static T&& apply(const T& a) { return (T &&) a; }
};
#endif
} // namespace phantom

#define PHANTOM_DISABLE_TRAIT(Trait, T)                                                                                \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct Trait<T> : std::false_type                                                                                  \
    {                                                                                                                  \
    };                                                                                                                 \
    }
#define PHANTOM_DISABLE_TRAIT_T(Trait, Types, Params, T)                                                               \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(Types, Params)>                                                                            \
    struct Trait<T<PHANTOM_PP_IDENTITY Params> > : std::false_type                                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }

#define PHANTOM_DISABLE_TRAIT_IF(Trait, T, ...)                                                                        \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct Trait<T> : std::integral_constant<bool, !(__VA_ARGS__)>                                                     \
    {                                                                                                                  \
    };                                                                                                                 \
    }
#define PHANTOM_DISABLE_TRAIT_IF_T(Trait, Types, Params, T, ...)                                                       \
    namespace phantom                                                                                                  \
    {                                                                                                                  \
    template<PHANTOM_PP_MIX(Types, Params)>                                                                            \
    struct Trait<T<PHANTOM_PP_IDENTITY Params> > : std::integral_constant<bool, !(__VA_ARGS__)>                        \
    {                                                                                                                  \
    };                                                                                                                 \
    }

/// @endcond
