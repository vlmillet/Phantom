// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>
HAUNT_STOP;

#include <phantom/detail/preprocessor.h>
#include <phantom/traits/IntTypeBySize.h>

#define PHANTOM_PREVENT_MACRO_SUBSTITUTION

#define PHANTOM_TEMPLATE_TYPENAME template
#define PHANTOM_TYPENAME typename

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_DBL_TYPENAME PHANTOM_TYPENAME PHANTOM_TYPENAME
#else
#    define PHANTOM_DBL_TYPENAME PHANTOM_TYPENAME
#endif

//  ==================================================================================================
//        SHORTCUTS
//  ==================================================================================================

#define PHANTOM_TODO __FILE__ "(" PHANTOM_PP_QUOTE(__LINE__) ") : TODO: "

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_OPEN_NAMESPACE(...)                                                                                \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_OPEN_NAMESPACE_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#    define PHANTOM_CLOSE_NAMESPACE(...)                                                                               \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_CLOSE_NAMESPACE_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#    define PHANTOM_FWD(...)                                                                                           \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_FWD_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#    define PHANTOM_FWD_T(...)                                                                                         \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_FWD_T_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))

#else // !PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_OPEN_NAMESPACE(...)                                                                                \
        PHANTOM_PP_CAT(_PHNTM_OPEN_NAMESPACE_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_CLOSE_NAMESPACE(...)                                                                               \
        PHANTOM_PP_CAT(_PHNTM_CLOSE_NAMESPACE_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_FWD(...) PHANTOM_PP_CAT(_PHNTM_FWD_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_FWD_T(...) PHANTOM_PP_CAT(_PHNTM_FWD_T_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)

#endif // PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#define _PHNTM_OPEN_NAMESPACE_0()
#define _PHNTM_OPEN_NAMESPACE_1(n0)                                                                                    \
    namespace n0                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_2(n0, n1)                                                                                \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_3(n0, n1, n2)                                                                            \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_4(n0, n1, n2, n3)                                                                        \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_5(n0, n1, n2, n3, n4)                                                                    \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {                                                                                                                  \
    namespace n4                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_6(n0, n1, n2, n3, n4, n5)                                                                \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {                                                                                                                  \
    namespace n4                                                                                                       \
    {                                                                                                                  \
    namespace n5                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)                                                            \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {                                                                                                                  \
    namespace n4                                                                                                       \
    {                                                                                                                  \
    namespace n5                                                                                                       \
    {                                                                                                                  \
    namespace n6                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)                                                        \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {                                                                                                                  \
    namespace n4                                                                                                       \
    {                                                                                                                  \
    namespace n5                                                                                                       \
    {                                                                                                                  \
    namespace n6                                                                                                       \
    {                                                                                                                  \
    namespace n7                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)                                                    \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {                                                                                                                  \
    namespace n4                                                                                                       \
    {                                                                                                                  \
    namespace n5                                                                                                       \
    {                                                                                                                  \
    namespace n6                                                                                                       \
    {                                                                                                                  \
    namespace n7                                                                                                       \
    {                                                                                                                  \
    namespace n8                                                                                                       \
    {
#define _PHNTM_OPEN_NAMESPACE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)                                               \
    namespace n0                                                                                                       \
    {                                                                                                                  \
    namespace n1                                                                                                       \
    {                                                                                                                  \
    namespace n2                                                                                                       \
    {                                                                                                                  \
    namespace n3                                                                                                       \
    {                                                                                                                  \
    namespace n4                                                                                                       \
    {                                                                                                                  \
    namespace n5                                                                                                       \
    {                                                                                                                  \
    namespace n6                                                                                                       \
    {                                                                                                                  \
    namespace n7                                                                                                       \
    {                                                                                                                  \
    namespace n8                                                                                                       \
    {                                                                                                                  \
    namespace n9                                                                                                       \
    {
#define _PHNTM_CLOSE_NAMESPACE_0()
#define _PHNTM_CLOSE_NAMESPACE_1(n0) }
#define _PHNTM_CLOSE_NAMESPACE_2(n0, n1)                                                                               \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_3(n0, n1, n2)                                                                           \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_4(n0, n1, n2, n3)                                                                       \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_5(n0, n1, n2, n3, n4)                                                                   \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_6(n0, n1, n2, n3, n4, n5)                                                               \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)                                                           \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)                                                       \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)                                                   \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }
#define _PHNTM_CLOSE_NAMESPACE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)                                              \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }                                                                                                                  \
    }

#define _PHNTM_FWD_2(t, c)                                                                                             \
    _PHNTM_OPEN_NAMESPACE_0() t c;                                                                                     \
    _PHNTM_CLOSE_NAMESPACE_0()
#define _PHNTM_FWD_3(t, n0, c)                                                                                         \
    _PHNTM_OPEN_NAMESPACE_1(n0) t c;                                                                                   \
    _PHNTM_CLOSE_NAMESPACE_1(n0)
#define _PHNTM_FWD_4(t, n0, n1, c)                                                                                     \
    _PHNTM_OPEN_NAMESPACE_2(n0, n1) t c;                                                                               \
    _PHNTM_CLOSE_NAMESPACE_2(n0, n1)
#define _PHNTM_FWD_5(t, n0, n1, n2, c)                                                                                 \
    _PHNTM_OPEN_NAMESPACE_3(n0, n1, n2) t c;                                                                           \
    _PHNTM_CLOSE_NAMESPACE_3(n0, n1, n2)
#define _PHNTM_FWD_6(t, n0, n1, n2, n3, c)                                                                             \
    _PHNTM_OPEN_NAMESPACE_4(n0, n1, n2, n3) t c;                                                                       \
    _PHNTM_CLOSE_NAMESPACE_4(n0, n1, n2, n3)
#define _PHNTM_FWD_7(t, n0, n1, n2, n3, n4, c)                                                                         \
    _PHNTM_OPEN_NAMESPACE_5(n0, n1, n2, n3, n4) t c;                                                                   \
    _PHNTM_CLOSE_NAMESPACE_5(n0, n1, n2, n3, n4)
#define _PHNTM_FWD_8(t, n0, n1, n2, n3, n4, n5, c)                                                                     \
    _PHNTM_OPEN_NAMESPACE_6(n0, n1, n2, n3, n4, n5) t c;                                                               \
    _PHNTM_CLOSE_NAMESPACE_6(n0, n1, n2, n3, n4, n5)
#define _PHNTM_FWD_9(t, n0, n1, n2, n3, n4, n5, n6, c)                                                                 \
    _PHNTM_OPEN_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6) t c;                                                           \
    _PHNTM_CLOSE_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)
#define _PHNTM_FWD_10(t, n0, n1, n2, n3, n4, n5, n6, n7, c)                                                            \
    _PHNTM_OPEN_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7) t c;                                                       \
    _PHNTM_CLOSE_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)
#define _PHNTM_FWD_11(t, n0, n1, n2, n3, n4, n5, n6, n7, n8, c)                                                        \
    _PHNTM_OPEN_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8) t c;                                                   \
    _PHNTM_CLOSE_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)
#define _PHNTM_FWD_12(t, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, c)                                                    \
    _PHNTM_OPEN_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9) t c;                                               \
    _PHNTM_CLOSE_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define _PHNTM_FWD_T_3(t, ts, c)                                                                                   \
        _PHNTM_OPEN_NAMESPACE_0()                                                                                      \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;
#    define _PHNTM_FWD_T_4(t, n0, ts, c)                                                                               \
        _PHNTM_OPEN_NAMESPACE_1(n0)                                                                                    \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_1(n0)
#    define _PHNTM_FWD_T_5(t, n0, n1, ts, c)                                                                           \
        _PHNTM_OPEN_NAMESPACE_2(n0, n1)                                                                                \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_2(n0, n1)
#    define _PHNTM_FWD_T_6(t, n0, n1, n2, ts, c)                                                                       \
        _PHNTM_OPEN_NAMESPACE_3(n0, n1, n2)                                                                            \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_3(n0, n1, n2)
#    define _PHNTM_FWD_T_7(t, n0, n1, n2, n3, ts, c)                                                                   \
        _PHNTM_OPEN_NAMESPACE_4(n0, n1, n2, n3)                                                                        \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_4(n0, n1, n2, n3)
#    define _PHNTM_FWD_T_8(t, n0, n1, n2, n3, n4, ts, c)                                                               \
        _PHNTM_OPEN_NAMESPACE_5(n0, n1, n2, n3, n4)                                                                    \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_5(n0, n1, n2, n3, n4)
#    define _PHNTM_FWD_T_9(t, n0, n1, n2, n3, n4, n5, ts, c)                                                           \
        _PHNTM_OPEN_NAMESPACE_6(n0, n1, n2, n3, n4, n5)                                                                \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_6(n0, n1, n2, n3, n4, n5)
#    define _PHNTM_FWD_T_10(t, n0, n1, n2, n3, n4, n5, n6, ts, c)                                                      \
        _PHNTM_OPEN_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)                                                            \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)
#    define _PHNTM_FWD_T_11(t, n0, n1, n2, n3, n4, n5, n6, n7, ts, c)                                                  \
        _PHNTM_OPEN_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)                                                        \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)
#    define _PHNTM_FWD_T_12(t, n0, n1, n2, n3, n4, n5, n6, n7, n8, ts, c)                                              \
        _PHNTM_OPEN_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)                                                    \
        {                                                                                                              \
            template<PHANTOM_PP_IDENTITY ts>                                                                           \
            t c;                                                                                                       \
            _PHNTM_CLOSE_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)
#    define _PHNTM_FWD_T_13(t, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, ts, c)                                          \
        _PHNTM_OPEN_NAMESPACE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)                                               \
        {                                                                                                              \
            template<PHANTOM_PP_IDENTITY ts>                                                                           \
            t c;                                                                                                       \
            _PHNTM_CLOSE_NAMESPACE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)

#else

#    define _PHNTM_FWD_T_3(t, ts, c)                                                                                   \
        _PHNTM_OPEN_NAMESPACE_0()                                                                                      \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;
#    define _PHNTM_FWD_T_4(t, n0, ts, c)                                                                               \
        _PHNTM_OPEN_NAMESPACE_1(n0)                                                                                    \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_1(n0)
#    define _PHNTM_FWD_T_5(t, n0, n1, ts, c)                                                                           \
        _PHNTM_OPEN_NAMESPACE_2(n0, n1)                                                                                \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_2(n0, n1)
#    define _PHNTM_FWD_T_6(t, n0, n1, n2, ts, c)                                                                       \
        _PHNTM_OPEN_NAMESPACE_3(n0, n1, n2)                                                                            \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_3(n0, n1, n2)
#    define _PHNTM_FWD_T_7(t, n0, n1, n2, n3, ts, c)                                                                   \
        _PHNTM_OPEN_NAMESPACE_4(n0, n1, n2, n3)                                                                        \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_4(n0, n1, n2, n3)
#    define _PHNTM_FWD_T_8(t, n0, n1, n2, n3, n4, ts, c)                                                               \
        _PHNTM_OPEN_NAMESPACE_5(n0, n1, n2, n3, n4)                                                                    \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_5(n0, n1, n2, n3, n4)
#    define _PHNTM_FWD_T_9(t, n0, n1, n2, n3, n4, n5, ts, c)                                                           \
        _PHNTM_OPEN_NAMESPACE_6(n0, n1, n2, n3, n4, n5)                                                                \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_6(n0, n1, n2, n3, n4, n5)
#    define _PHNTM_FWD_T_10(t, n0, n1, n2, n3, n4, n5, n6, ts, c)                                                      \
        _PHNTM_OPEN_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)                                                            \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_7(n0, n1, n2, n3, n4, n5, n6)
#    define _PHNTM_FWD_T_11(t, n0, n1, n2, n3, n4, n5, n6, n7, ts, c)                                                  \
        _PHNTM_OPEN_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)                                                        \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_8(n0, n1, n2, n3, n4, n5, n6, n7)
#    define _PHNTM_FWD_T_12(t, n0, n1, n2, n3, n4, n5, n6, n7, n8, ts, c)                                              \
        _PHNTM_OPEN_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)                                                    \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)
#    define _PHNTM_FWD_T_13(t, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, ts, c)                                          \
        _PHNTM_OPEN_NAMESPACE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)                                               \
        template<PHANTOM_PP_IDENTITY ts>                                                                               \
        t c;                                                                                                           \
        _PHNTM_CLOSE_NAMESPACE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)

#endif

#define PHANTOM_REBIND(_t_class_)                                                                                      \
    template<typename t_OtherTy>                                                                                       \
    struct rebind                                                                                                      \
    {                                                                                                                  \
        typedef _t_class_<t_OtherTy> other;                                                                            \
    };

#define PHANTOM_R_NONE (::phantom::lang::Modifier::None)

/// FLAGS
#define PHANTOM_R_FLAG_NONE (0)
#define PHANTOM_R_FLAG_NATIVE (0x00000001 | PHANTOM_R_ALWAYS_VALID)
// #define o_defined                   0x00000004
// #define o_finalized                 0x00000008

#define PHANTOM_R_FLAG_TERMINATED 0x00000010
// #define o_sized                     0x00000020
#define PHANTOM_R_FLAG_IMPORTABLE 0x00000040
#define PHANTOM_R_FLAG_SHARED 0x00000080

#define PHANTOM_R_DEFERRED 0x00000100
#define PHANTOM_R_ALWAYS_VALID 0x00000200
#define PHANTOM_R_INCOMPLETE 0x00000400
#define PHANTOM_R_FLAG_TEMPLATE_DEPENDANT 0x00000800

#define PHANTOM_R_FLAG_INHERITED 0x00001000 /// for inherited constructors
#define PHANTOM_R_FLAG_IMPLICIT 0x00002000  /// for trivial member functions

// META SPECIFIERS
#define PHANTOM_R_FLAG_TEMP 0x00010000
#define PHANTOM_R_FLAG_NO_DEFAULT_CTOR 0x00020000
#define PHANTOM_R_FLAG_NO_COPY 0x00040000
#define PHANTOM_R_FLAG_NO_MOVE 0x00080000

// #define ?                  (0x00100000|PHANTOM_R_FLAG_PRIVATE_VIS) /// for archived sources

#define PHANTOM_R_FLAG_TEMPLATE_ELEM 0x01000000
#define PHANTOM_R_FLAG_PUBLIC_VIS                                                                                      \
    0x02000000 /// default visibility /!\ visibility is not access ! use <phantom/public_access> for
               /// accessibility
#define PHANTOM_R_FLAG_PROTECTED_VIS                                                                                   \
    0x04000000 /// allows you to hide elements from underlying scopes (anonymous elements) in scope
               /// resolution
#define PHANTOM_R_FLAG_PRIVATE_VIS 0x08000000 /// allows you to hide elements from scope resolution

#define PHANTOM_R_FLAG_MANDATORY 0x10000000
#define PHANTOM_R_FLAG_READONLY 0x20000000

/// module flags (only one for now)
#define PHANTOM_PLUGIN_REGISTER_CLASS_MEMBERS_ON_ACCESS                                                                \
    0x40000000 // register class members only when they are requested on a class, either directly
               // via get functions or via parsing/finding, avoiding heavy executable loads at
               // startup and allocation of unused memory.

// CONSTANTS

#define PHANTOM_NO_COPY_OF(...) (PHANTOM_R_FLAG_NO_COPY * (::phantom::HasCopyDisabled<__VA_ARGS__>::value))

#define PHANTOM_MATCH(_flags_, _flag_) (((_flags_) & (_flag_)) == _flag_)

#define PHANTOM_NO_ARG ((void**)0)

// specifiers

#if PHANTOM_HAS_NOEXCEPT
#    define PHANTOM_NOEXCEPT noexcept
#    define PHANTOM_NOEXCEPT_IF(pred) noexcept((pred))
#else
#    define PHANTOM_NOEXCEPT
#    define PHANTOM_NOEXCEPT_IF(pred)
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    define PHANTOM_IF_LIKELY(x) if (__builtin_expect(!!(x), 1))
#    define PHANTOM_IF_UNLIKELY(x) if (__builtin_expect(!!(x), 0))
#else
#    define PHANTOM_IF_LIKELY(x) if (!!(x))
#    define PHANTOM_IF_UNLIKELY(x) if (!!(x))
#endif

#if !defined(__DOXYGEN__)

#    if (PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO)
#        define PHANTOM_NORETURN __declspec(noreturn)
#        define PHANTOM_FORCEINLINE __forceinline
#        define PHANTOM_NOINLINE __declspec(noinline)
#        define PHANTOM_INIT_PRIO(N)
#        define PHANTOM_VISIBILITY_HIDDEN
#        define PHANTOM_VISIBILITY_EXPORT __declspec(dllexport)
#        define PHANTOM_VISIBILITY_IMPORT __declspec(dllimport)
#        define PHANTOM_CALLCONV(abi) PHANTOM_PP_CAT(__, abi)
#        define PHANTOM_OUTSIDE_EXPLICIT_FULL_SPEC

#    else
#        define PHANTOM_NORETURN __attribute__((noreturn))
#        define PHANTOM_FORCEINLINE __inline __attribute__((always_inline))
#        define PHANTOM_NOINLINE __attribute__((noinline))
#        define PHANTOM_INIT_PRIO(N) // __attribute__((init_priority(N)))
#        define PHANTOM_VISIBILITY_HIDDEN __attribute__((visibility("hidden")))
#        define PHANTOM_VISIBILITY_EXPORT __attribute__((visibility("default")))
#        define PHANTOM_VISIBILITY_IMPORT __attribute__((visibility("default")))
#        define PHANTOM_CALLCONV(abi) __attribute__((abi))
#        define PHANTOM_OUTSIDE_EXPLICIT_FULL_SPEC

#    endif

#else
#    define PHANTOM_FORCEINLINE inline
#    define PHANTOM_NOINLINE
#    define PHANTOM_INIT_PRIO(N)
#    define PHANTOM_VISIBILITY_HIDDEN
#    define PHANTOM_VISIBILITY_EXPORT
#    define PHANTOM_VISIBILITY_IMPORT
#    define PHANTOM_CALLCONV(abi)
#    define PHANTOM_OUTSIDE_EXPLICIT_FULL_SPEC
// ignoring BOOST macros
#    define BOOST_MPL_HAS_XXX_TRAIT_DEF(...)
#endif

// DESTRUCTOR SPECIFIERS
#define PHANTOM_DTOR

// COMPILER SPECIFIC C FUNCTIONS

#define PHANTOM_CHAR_IS_BLANK(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n') || ((c) == '\r'))
#define PHANTOM_CHAR_IS_CPP_IDENTIFIER(c)                                                                              \
    ((((c) >= '0') && ((c) <= '9')) || ((((c) | 0x20) >= 'a') && (((c) | 0x20) <= 'z')) || (c) == '_')

// TEMPLATE HELPER

#define PHANTOM_TT(_t_, ...) PHANTOM_TT_Bis(_t_, __VA_ARGS__)
#define PHANTOM_TT_Bis(_t_, ...) _t_<__VA_ARGS__>

// C Style Allocation

#define PHANTOM_ALLOCA(_size_) alloca(_size_)
#define PHANTOM_MALLOC(_size_) phantom::allocate(_size_, 1)
#define PHANTOM_MALLOC_ALIGNED(_size_, _align_) phantom::allocate(_size_, _align_)
#define PHANTOM_REALLOC(_ptr_, _size_) phantom::reallocate(_ptr_, _size_, 1)
#define PHANTOM_FREE(_ptr_) phantom::deallocate(_ptr_)
#define PHANTOM_FREE_ALIGNED PHANTOM_FREE

/*************************************************************************************************
 * ASSERTION & OTHER DEBUG MESSAGES
 *************************************************************************************************/
#if PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG || PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    define PHANTOM_DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#    error This compiler is not supported
#endif

namespace static_warning_detail
{
struct true_type
{
};
struct false_type
{
};
template<int test>
struct converter : public true_type
{
};
template<>
struct converter<0> : public false_type
{
};
} // namespace static_warning_detail

#define PHANTOM_STATIC_WARNING(cond, msg)                                                                              \
    struct PHANTOM_PP_CAT(static_warning, __LINE__)                                                                    \
    {                                                                                                                  \
        PHANTOM_DEPRECATE(void _(::static_warning_detail::false_type const&), msg){};                                  \
        void _(::static_warning_detail::true_type const&){};                                                           \
        PHANTOM_PP_CAT(static_warning, __LINE__)() { _(::static_warning_detail::converter<(cond)>()); }                \
    }

#define PHANTOM_STATIC_WARNING_TEMPLATE(token, cond, msg)                                                              \
    PHANTOM_STATIC_WARNING(cond, msg) PHANTOM_PP_CAT(PHANTOM_PP_CAT(_localvar_, token), __LINE__)

#define PHANTOM_UNUSED(var) (void)var

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_ASSERT(...)                                                                                        \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_ASSERT_, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__)), (__VA_ARGS__))
#    define PHANTOM_ERROR(...)                                                                                         \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_ERROR_, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__)), (__VA_ARGS__))
#    define PHANTOM_WARNING(...)                                                                                       \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_WARNING_, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__)), (__VA_ARGS__))
#    define PHANTOM_STATIC_ASSERT(...)                                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(PHANTOM_STATIC_ASSERT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#else
#    define PHANTOM_ASSERT(...) PHANTOM_PP_CAT(_PHNTM_ASSERT_, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_ERROR(...) PHANTOM_PP_CAT(_PHNTM_ERROR_, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_WARNING(...) PHANTOM_PP_CAT(_PHNTM_WARNING_, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_STATIC_ASSERT(...)                                                                                 \
        PHANTOM_PP_CAT(PHANTOM_STATIC_ASSERT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_T
#else
#    define PHANTOM_T template
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_CURRENT_FUNCTION __FUNCTION__
#else
#    define PHANTOM_CURRENT_FUNCTION
#endif

#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#    define PHANTOM_ASSERT_DEBUG PHANTOM_ASSERT
#else
#    define PHANTOM_ASSERT_DEBUG(...)
#endif

#if PHANTOM_DEBUG_LEVEL

#    define PHANTOM_STATIC_ASSERT_1(a)                                                                                 \
        static_assert(PHANTOM_PP_REMOVE_PARENS(a), PHANTOM_PP_QUOTE(PHANTOM_PP_REMOVE_PARENS(a)))
#    define PHANTOM_STATIC_ASSERT_2(a, m) static_assert(PHANTOM_PP_REMOVE_PARENS(a), m)

#    define _PHNTM_WARNING_1(expression) _PHNTM_WARNING_X(expression, "no detail about the warning")

#    define _PHNTM_WARNING_X(expression, ...)                                                                          \
        {                                                                                                              \
            ((!!(expression)) ||                                                                                       \
             (phantom::warning PHANTOM_PREVENT_MACRO_SUBSTITUTION(#expression, __FILE__, __LINE__, __VA_ARGS__), 0));  \
        }

#    define _PHNTM_ASSERT_1(expression) _PHNTM_ASSERT_X(expression, "no detail about the assertion")

#    define _PHNTM_ASSERT_X(condition, ...)                                                                            \
        do                                                                                                             \
        {                                                                                                              \
            if (!(condition))                                                                                          \
            {                                                                                                          \
                if (phantom::assertion PHANTOM_PREVENT_MACRO_SUBSTITUTION(#condition, __FILE__, __LINE__,              \
                                                                          __VA_ARGS__))                                \
                {                                                                                                      \
                    PHANTOM_DEBUG_BREAK();                                                                             \
                }                                                                                                      \
            }                                                                                                          \
        } while (0)

#    define PHANTOM_VERIFY(...) PHANTOM_ASSERT(__VA_ARGS__)
#    define PHANTOM_DEBUG_ONLY(...) __VA_ARGS__
#    define PHANTOM_IF_ASSERTS(...) __VA_ARGS__

#else // PHANTOM_DEBUG_LEVEL

#    define PHANTOM_STATIC_ASSERT_1(a)
#    define PHANTOM_STATIC_ASSERT_2(a, m)

#    define _PHNTM_ASSERT_1(condition)                                                                                 \
        do                                                                                                             \
        {                                                                                                              \
        } while (0)
#    define _PHNTM_ASSERT_X(condition, ...)                                                                            \
        do                                                                                                             \
        {                                                                                                              \
        } while (0)
#    define _PHNTM_WARNING_1(condition)
#    define _PHNTM_WARNING_X(condition, ...)
#    define PHANTOM_DEBUG_ONLY(...)
#    define PHANTOM_IF_ASSERTS(...)
#    define PHANTOM_VERIFY(condition, ...) ((void)(condition))

#endif // PHANTOM_DEBUG_LEVEL

#define PHANTOM_LOG(msgType, ...)                                                                                      \
    ::phantom::log PHANTOM_PREVENT_MACRO_SUBSTITUTION(::phantom::MessageType::msgType, __FILE__, __LINE__, __VA_ARGS__)

#define _PHNTM_ERROR_1(condition) _PHNTM_ERROR_X(condition, "no detail about the error")

#define _PHNTM_ERROR_X(condition, ...)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            phantom::error PHANTOM_PREVENT_MACRO_SUBSTITUTION(#condition, __FILE__, __LINE__, __VA_ARGS__);            \
            PHANTOM_DEBUG_BREAK();                                                                                     \
        }                                                                                                              \
    } while (0)

#define PHANTOM_ASSERT_NOT(exp) PHANTOM_ASSERT(!(exp))
#define PHANTOM_ASSERT_NO_IMPL() PHANTOM_ASSERT(false, "Not Implemented")
#define PHANTOM_UNREACHABLE() PHANTOM_ASSERT(false, "Unreachable code")
#define PHANTOM_ASSERT_FORBIDDEN_CALL() PHANTOM_ASSERT(false, "Call to this function forbidden")

/*************************************************************************************************
 * DEBUG BREAK
 *************************************************************************************************/

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_DEBUG_BREAK() __debugbreak()
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC
#    define PHANTOM_DEBUG_BREAK() __builtin_trap()
#elif PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG
#    define PHANTOM_DEBUG_BREAK() __builtin_trap()
#else
#    error TODO : define PHANTOM_DEBUG_BREAK for all compilers
#endif

/*************************************************************************************************
 * MISC
 *************************************************************************************************/
// defer at end of block

#define PHANTOM_DEFER(...) _PHNTM_DEFER_I(__COUNTER__, __VA_ARGS__)
#define _PHNTM_DEFER_I(counter, ...) _PHNTM_DEFER_II(counter, __VA_ARGS__)
#define _PHNTM_DEFER_II(counter, ...)                                                                                  \
    struct PHANTOM_PP_CAT(_PHNTM_defer, counter)                                                                       \
    {                                                                                                                  \
        PHANTOM_PP_CAT(_PHNTM_defer, counter)(std::function<void()> a_lambda) : m_lambda(a_lambda) {}                  \
        ~PHANTOM_PP_CAT(_PHNTM_defer, counter)() { m_lambda(); }                                                       \
        std::function<void()> m_lambda;                                                                                \
    } PHANTOM_PP_CAT(_PHNTM_defer_i_, counter)([this]() { __VA_ARGS__; });

// min / max //
#define PHANTOM_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define PHANTOM_MAX(a, b) (((a) > (b)) ? (a) : (b))

// Mask test helper
#define PHANTOM_MASK_TEST(_variable_, _value_) ((_variable_ & _value_) == _value_)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_DIAGNOSTIC_FUNCTION __FUNCSIG__
#    define PHANTOM_PRETTY_FUNCTION_VAR __FUNCSIG__
#else
#    define PHANTOM_DIAGNOSTIC_FUNCTION "PHANTOM_DIAGNOSTIC_FUNCTION not defined for current compiler"
#    define PHANTOM_PRETTY_FUNCTION_VAR __PRETTY_FUNCTION__
#endif
