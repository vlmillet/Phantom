// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/customize.h>

// Preprocessor helpers

#define PHANTOM_PP_MACRO_CYCLE_L3(_var_) PHANTOM_PP_MACRO_CYCLE_L2(_var_)
#define PHANTOM_PP_MACRO_CYCLE_L2(_var_) PHANTOM_PP_MACRO_CYCLE_L1(_var_)
#define PHANTOM_PP_MACRO_CYCLE_L1(_var_) _var_

// Thanks to the genius who found that trick to count __VA_ARGS__ arguments

#define PHANTOM_PP_CAT_L1(x, y) x##y
#define PHANTOM_PP_CAT_L2(x, y) PHANTOM_PP_CAT_L1(x, y)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_CAT(x, y) PHANTOM_PP_CAT_L2(x, y)
#else
#    define PHANTOM_PP_CAT(x, y) PHANTOM_PP_CAT_L1(x, y)
#endif

#define PHANTOM_PP_CAT_P() PHANTOM_PP_CAT

/// PHANTOM_PP_REMOVE_PARENS (only works for 1 level on clang/gcc)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_PP_NOTHING_PHANTOM_PP_EXTRACT
#    define PHANTOM_PP_EXTRACT(...) PHANTOM_PP_EXTRACT __VA_ARGS__
#    define PHANTOM_PP_REMOVE_PARENS(x) PHANTOM_PP_CAT(PHANTOM_PP_NOTHING_, PHANTOM_PP_EXTRACT x)

#else

#    define _PHNTM_PP_REMOVE_PARENS_APPLY(macro, args) _PHNTM_PP_REMOVE_PARENS_APPLY_L1(macro, args)
#    define _PHNTM_PP_REMOVE_PARENS_APPLY_L1(macro, args) macro args

#    define _PHNTM_PP_REMOVE_PARENS_EVAL(test, x) _PHNTM_PP_REMOVE_PARENS_EVAL_L1(test, x)
#    define _PHNTM_PP_REMOVE_PARENS_EVAL_L1(test, x)                                               \
        _PHNTM_PP_REMOVE_PARENS_TEST(_PHNTM_PP_REMOVE_PARENS_TEST_ARITY test, x)

#    define _PHNTM_PP_REMOVE_PARENS_TEST_ARITY(...)                                                \
        _PHNTM_PP_REMOVE_PARENS_APPLY(_PHNTM_PP_REMOVE_PARENS_TEST_ARITY_L1, (__VA_ARGS__, 2, 1))
#    define _PHNTM_PP_REMOVE_PARENS_TEST_ARITY_L1(a, b, c, ...) c

#    define _PHNTM_PP_REMOVE_PARENS_TEST(cond, x) _PHNTM_PP_REMOVE_PARENS_TEST_L1(cond, x)
#    define _PHNTM_PP_REMOVE_PARENS_TEST_L1(cond, x)                                               \
        PHANTOM_PP_CAT(_PHNTM_PP_REMOVE_PARENS_TEST_, cond)(x)

#    define _PHNTM_PP_REMOVE_PARENS_TEST_1(x) x
#    define _PHNTM_PP_REMOVE_PARENS_TEST_2(x)                                                      \
        _PHNTM_PP_REMOVE_PARENS_APPLY(_PHNTM_PP_REMOVE_PARENS_TEST_2_L1, x)
#    define _PHNTM_PP_REMOVE_PARENS_TEST_2_L1(...) __VA_ARGS__

#    define _PHNTM_PP_REMOVE_PARENS_L1(...) 1, 1
#    define PHANTOM_PP_REMOVE_PARENS(x)                                                            \
        _PHNTM_PP_REMOVE_PARENS_EVAL((_PHNTM_PP_REMOVE_PARENS_L1 x), x)

#endif

// Multi level preprocessor quotifier
#define PHANTOM_PP_QUOTE_L8(...) PHANTOM_PP_QUOTE_L7(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L7(...) PHANTOM_PP_QUOTE_L6(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L6(...) PHANTOM_PP_QUOTE_L5(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L5(...) PHANTOM_PP_QUOTE_L4(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L4(...) PHANTOM_PP_QUOTE_L3(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L3(...) PHANTOM_PP_QUOTE_L2(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L2(...) PHANTOM_PP_QUOTE_L1(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L1(...) PHANTOM_PP_QUOTE_L0(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_L0(...) #__VA_ARGS__
#define PHANTOM_PP_QUOTE(...) PHANTOM_PP_QUOTE_L3(__VA_ARGS__)
#define PHANTOM_PP_QUOTE_ARG_0(a0, ...) PHANTOM_PP_QUOTE(a0)
#define PHANTOM_PP_QUOTE_ARG_1(a0, a1, ...) PHANTOM_PP_QUOTE(a1)

#define PHANTOM_PP_LEFT_PAREN (
#define PHANTOM_PP_RIGHT_PAREN )

#define PHANTOM_PP_ARG_N(                                                                          \
____1, ____2, ____3, ____4, ____5, ____6, ____7, ____8, ____9, ____10, ____11, ____12, ____13,     \
____14, ____15, ____16, ____17, ____18, ____19, ____20, ____21, ____22, ____23, ____24, ____25,    \
____26, ____27, ____28, ____29, ____30, ____31, ____32, ____33, ____34, ____35, ____36, ____37,    \
____38, ____39, ____40, ____41, ____42, ____43, ____44, ____45, ____46, ____47, ____48, ____49,    \
____50, ____51, ____52, ____53, ____54, ____55, ____56, ____57, ____58, ____59, ____60, ____61,    \
____62, ____63, ____64, ____65, ____66, ____67, ____68, ____69, ____70, ____N, ...)                \
    ____N

#define PHANTOM_PP_ARG_N_MINUS1(                                                                   \
____0, ____1, ____2, ____3, ____4, ____5, ____6, ____7, ____8, ____9, ____10, ____11, ____12,      \
____13, ____14, ____15, ____16, ____17, ____18, ____19, ____20, ____21, ____22, ____23, ____24,    \
____25, ____26, ____27, ____28, ____29, ____30, ____31, ____32, ____33, ____34, ____35, ____36,    \
____37, ____38, ____39, ____40, ____41, ____42, ____43, ____44, ____45, ____46, ____47, ____48,    \
____49, ____50, ____51, ____52, ____53, ____54, ____55, ____56, ____57, ____58, ____59, ____60,    \
____61, ____62, ____63, ____64, ____65, ____66, ____67, ____68, ____69, ____70, ____N, ...)        \
    ____N

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_PP_ARGCOUNT_OLD(...)                                                           \
        PHANTOM_PP_ARG_N PHANTOM_PP_LEFT_PAREN __VA_ARGS__##, 70, 69, 68, 67, 66, 65, 64, 63, 62,  \
        61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,    \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18,    \
        17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 PHANTOM_PP_RIGHT_PAREN
#    define PHANTOM_PP_ARGCOUNT_MINUS1(...)                                                        \
        PHANTOM_PP_ARG_N_MINUS1 PHANTOM_PP_LEFT_PAREN c, ##__VA_ARGS__##, 70, 69, 68, 67, 66, 65,  \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,    \
        42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,    \
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,                     \
        0 PHANTOM_PP_RIGHT_PAREN

#    define PHANTOM_PP_ARGCOUNT(...)                                                               \
        PHANTOM_PP_ARG_N_MINUS1 PHANTOM_PP_LEFT_PAREN c, ##__VA_ARGS__##, 70, 69, 68, 67, 66, 65,  \
        64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,    \
        42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,    \
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,                     \
        0 PHANTOM_PP_RIGHT_PAREN
#    define PHANTOM_PP_ARGCOUNT_0_X(...)                                                           \
        PHANTOM_PP_ARG_N_MINUS1 PHANTOM_PP_LEFT_PAREN c, ##__VA_ARGS__##, X, X, X, X, X, X, X, X,  \
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
        X, X, 0 PHANTOM_PP_RIGHT_PAREN
#    define PHANTOM_PP_ARGCOUNT_1_X(...)                                                           \
        PHANTOM_PP_ARG_N PHANTOM_PP_LEFT_PAREN __VA_ARGS__##, X, X, X, X, X, X, X, X, X, X, X, X,  \
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, 1,        \
        0 PHANTOM_PP_RIGHT_PAREN

#    define PHANTOM_PP_ARGCOUNT_1_2_X(...)                                                         \
        PHANTOM_PP_ARG_N PHANTOM_PP_LEFT_PAREN __VA_ARGS__##, X, X, X, X, X, X, X, X, X, X, X, X,  \
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, 2, 1,        \
        0 PHANTOM_PP_RIGHT_PAREN

#else

/// The one to use

#    define PHANTOM_PP_ARGCOUNT_OLD(...)                                                           \
        PHANTOM_PP_ARGCOUNT_(gcc_specific_C99_adapter, ##__VA_ARGS__)

#    define PHANTOM_PP_ARGCOUNT(...) _PHNTM_PP_ARGCOUNT_EX(c, ##__VA_ARGS__)
#    define _PHNTM_PP_ARGCOUNT_EX(...)                                                             \
        PHANTOM_PP_ARGCOUNT_MINUS1(gcc_specific_C99_adapter, ##__VA_ARGS__)

#    define PHANTOM_PP_ARGCOUNT_0_X(...) _PHNTM_PP_ARGCOUNT_0_X_EX(c, ##__VA_ARGS__)
#    define _PHNTM_PP_ARGCOUNT_0_X_EX(...)                                                         \
        PHANTOM_PP_ARGCOUNT_0_X_(gcc_specific_C99_adapter, ##__VA_ARGS__)

#    define PHANTOM_PP_ARGCOUNT_MINUS1(...)                                                        \
        PHANTOM_PP_ARGCOUNT_MINUS1_(gcc_specific_C99_adapter, ##__VA_ARGS__)

#    define PHANTOM_PP_ARGCOUNT_1_X(...)                                                           \
        PHANTOM_PP_ARGCOUNT_1_X_(gcc_specific_C99_adapter, ##__VA_ARGS__)

#    define PHANTOM_PP_ARGCOUNT_1_2_X(...)                                                         \
        PHANTOM_PP_ARGCOUNT_1_2_X_(gcc_specific_C99_adapter, ##__VA_ARGS__)

#    define PHANTOM_PP_ARGCOUNT_1_X_(...)                                                          \
        PHANTOM_PP_ARG_N(__VA_ARGS__, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
                         X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,   \
                         X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,   \
                         1, 0, gcc_specific_C99_arg)

#    define PHANTOM_PP_ARGCOUNT_0_X_(...)                                                          \
        PHANTOM_PP_ARG_N(__VA_ARGS__, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
                         X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,   \
                         X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,   \
                         0, gcc_specific_C99_arg)

#    define PHANTOM_PP_ARGCOUNT_1_2_X_(...)                                                        \
        PHANTOM_PP_ARG_N(__VA_ARGS__, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  \
                         X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,   \
                         X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, 2,   \
                         1, 0, gcc_specific_C99_arg)

#    define PHANTOM_PP_ARGCOUNT_(...)                                                              \
        PHANTOM_PP_ARG_N(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,  \
                         54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37,   \
                         36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,   \
                         18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,         \
                         gcc_specific_C99_arg)

#    define PHANTOM_PP_ARGCOUNT_MINUS1_(...)                                                       \
        PHANTOM_PP_ARG_N_MINUS1(__VA_ARGS__, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56,   \
                                55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,    \
                                39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,    \
                                23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7,   \
                                6, 5, 4, 3, 2, 1, 0, gcc_specific_C99_arg)

#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_COMMA_IF_NON_EMPTY(...)                                                     \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(_PHNTM_COMMA_IF_NON_EMPTY_, PHANTOM_PP_ARGCOUNT_0_X(__VA_ARGS__)),          \
        (__VA_ARGS__))
#else
#    define PHANTOM_PP_COMMA_IF_NON_EMPTY(...)                                                     \
        PHANTOM_PP_CAT(_PHNTM_COMMA_IF_NON_EMPTY_, PHANTOM_PP_ARGCOUNT_0_X(__VA_ARGS__))           \
        (__VA_ARGS__)
#endif

#define _PHNTM_COMMA_IF_NON_EMPTY_0()
#define _PHNTM_COMMA_IF_NON_EMPTY_X(...) ,

#define PHANTOM_PP_ARGCOUNT_FUNC() PHANTOM_PP_ARGCOUNT

#define PHANTOM_PP_ARG_0(_a0_, ...) _a0_
#define PHANTOM_PP_ARG_1(_a0_, _a1_, ...) _a1_
#define PHANTOM_PP_ARG_2(_a0_, _a1_, _a2_, ...) _a2_
#define PHANTOM_PP_ARG_3(_a0_, _a1_, _a2_, _a3_, ...) _a3_
#define PHANTOM_PP_ARG_4(_a0_, _a1_, _a2_, _a3_, _a4_, ...) _a4_
#define PHANTOM_PP_ARG_5(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, ...) _a5_
#define PHANTOM_PP_ARG_6(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, ...) _a6_
#define PHANTOM_PP_ARG_7(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, ...) _a7_
#define PHANTOM_PP_ARG_8(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, ...) _a8_
#define PHANTOM_PP_ARG_9(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, ...) _a9_
#define PHANTOM_PP_ARG_10(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_, ...)  \
    _a10_
#define PHANTOM_PP_ARG_11(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, ...)                                                              \
    _a11_
#define PHANTOM_PP_ARG_12(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, ...)                                                       \
    _a12_
#define PHANTOM_PP_ARG_13(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, ...)                                                \
    _a13_
#define PHANTOM_PP_ARG_14(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, _a14_, ...)                                         \
    _a14_
#define PHANTOM_PP_ARG_15(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, _a14_, _a15_, ...)                                  \
    _a15_
#define PHANTOM_PP_ARG_16(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, _a14_, _a15_, _a16_, ...)                           \
    _a16_
#define PHANTOM_PP_ARG_17(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, _a14_, _a15_, _a16_, _a17_, ...)                    \
    _a17_
#define PHANTOM_PP_ARG_18(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, _a14_, _a15_, _a16_, _a17_, _a18_, ...)             \
    _a18_
#define PHANTOM_PP_ARG_19(_a0_, _a1_, _a2_, _a3_, _a4_, _a5_, _a6_, _a7_, _a8_, _a9_, _a10_,       \
                          _a11_, _a12_, _a13_, _a14_, _a15_, _a16_, _a17_, _a18_, _a19_, ...)      \
    _a19_

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_PP_VARARG(Macro, ...)                                                          \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)(Macro, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),              \
        (__VA_ARGS__))
#    define PHANTOM_PP_VARARG_0_X(Macro, ...)                                                      \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)(Macro, PHANTOM_PP_ARGCOUNT_0_X(__VA_ARGS__)),          \
        (__VA_ARGS__))
#    define PHANTOM_PP_VARARG_1_X(Macro, ...)                                                      \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)(Macro, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__)),          \
        (__VA_ARGS__))
#    define PHANTOM_PP_VARARG_1_2_X(Macro, ...)                                                    \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)(Macro, PHANTOM_PP_ARGCOUNT_1_2_X(__VA_ARGS__)),        \
        (__VA_ARGS__))

#else

#    define PHANTOM_PP_VARARG(Macro, ...)                                                          \
        PHANTOM_PP_CAT(Macro, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_PP_VARARG_0_X(Macro, ...)                                                      \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)                                                        \
        (Macro, PHANTOM_PP_ARGCOUNT_0_X(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_PP_VARARG_1_X(Macro, ...)                                                      \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)                                                        \
        (Macro, PHANTOM_PP_ARGCOUNT_1_X(__VA_ARGS__))(__VA_ARGS__)
#    define PHANTOM_PP_VARARG_1_2_X(Macro, ...)                                                    \
        PHANTOM_PP_IDENTITY(PHANTOM_PP_CAT)                                                        \
        (Macro, PHANTOM_PP_ARGCOUNT_1_2_X(__VA_ARGS__))(__VA_ARGS__)

#endif

#define PHANTOM_PP_SUBLIST(skipped, ...) __VA_ARGS__

#define PHANTOM_PP_IDENTITY(...) __VA_ARGS__

#define PHANTOM_PP_IGNORE(...)

#define _PHNTM_PP_IDENTITY(...) _##__VA_ARGS__

#define PHANTOM_PMM_L3(mmm) PHANTOM_PMM_L2(mmm)
#define PHANTOM_PMM_L2(mmm) PHANTOM_PMM_L1(mmm)
#define PHANTOM_PMM_L1(mmm) #mmm
#define PHANTOM_PMM(mmm) PHANTOM_PMM_L1(mmm)

#define PHANTOM_PP_OUTPUT(...) message(PHANTOM_PP_QUOTE_L8(__VA_ARGS__))

#define PHANTOM_PP_POP_FRONT(l0)                                                                   \
    PHANTOM_PP_LEFT_PAREN PHANTOM_PP_POP_FRONT_ARG l0 PHANTOM_PP_RIGHT_PAREN

#define PHANTOM_PP_POP_FRONT_ARG(a, ...) __VA_ARGS__

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_BY_LIST(l0)                                      \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS l0

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS(l0, l1)                                          \
    PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_,               \
    PHANTOM_PP_ARGCOUNT_FUNC() l0        PHANTOM_PP_RIGHT_PAREN(l0, l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_1(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_1_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_2(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_1_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9(l0, l1)                                        \
    (PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_,            \
     PHANTOM_PP_ARGCOUNT_FUNC() l1        PHANTOM_PP_RIGHT_PAREN(l0, l1))

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_1_1(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_2_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_1_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_2_2(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_2_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_2_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_3(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_3(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_3_3((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_4(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_3(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_3((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_4(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_4_4((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_5(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_3(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_3((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_4(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_4((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_5(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_5_4((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_6(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_3(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_3((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_4(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_4((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_5(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_5((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_6(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_6_6((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_7(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_3(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_3((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_4(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_4((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_5(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_5((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_6(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_6((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_7(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_7_7((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_8(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_1(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_1((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_2(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_2((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_3(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_3((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_4(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_4((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_5(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_5((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_6(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_6((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_7(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_7((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_8(l0, l1)                                      \
    PHANTOM_PP_ARG_0 l0,                                                                           \
    PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_8_8((PHANTOM_PP_POP_FRONT_ARG l0), l1)

#define PHANTOM_PP_REPLACE_WITH_DEFAULT_ARGUMENTS_9_9(l0, l1) PHANTOM_PP_IDENTITY l1

#define PHANTOM_PP_BY_LIST_BIS_2(name, l0) PHANTOM_PP_BY_LIST_BIS(name, l0)

#define PHANTOM_PP_BY_LIST_BIS(name, l0) name l0

#define PHANTOM_PP_BY_LIST(name, l0) PHANTOM_PP_BY_LIST_BIS_2(name, l0)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define _PHNTM_MK_CLASS_T_SIGN(...)                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_MK_CLASS_T_SIGN_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),  \
                       (__VA_ARGS__))
#    define _PHNTM_MK_CLASS_T_ARGS(...)                                                            \
        PHANTOM_PP_CAT(_PHNTM_MK_CLASS_T_ARGS_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))
#else
#    define _PHNTM_MK_CLASS_T_SIGN(...)                                                            \
        PHANTOM_PP_CAT(_PHNTM_MK_CLASS_T_SIGN_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#    define _PHNTM_MK_CLASS_T_ARGS(...)                                                            \
        PHANTOM_PP_CAT(_PHNTM_MK_CLASS_T_ARGS_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_ADD_PREFIX_EACH(prefix, ...)                                                \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_ADD_PREFIX_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),             \
        (prefix, ##__VA_ARGS__))
#    define PHANTOM_PP_ADD_PREFIX_EACH_NO_CAT(prefix, ...)                                         \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_ADD_PREFIX_EACH_NO_CAT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),      \
        (prefix, ##__VA_ARGS__))
#else
#    define PHANTOM_PP_ADD_PREFIX_EACH(prefix, ...)                                                \
        PHANTOM_PP_CAT(PHANTOM_PP_ADD_PREFIX_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))              \
        (prefix, ##__VA_ARGS__)
#    define PHANTOM_PP_ADD_PREFIX_EACH_NO_CAT(prefix, ...)                                         \
        PHANTOM_PP_CAT(PHANTOM_PP_ADD_PREFIX_EACH_NO_CAT_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))       \
        (prefix, ##__VA_ARGS__)
#endif

#include "detail/PP/PHANTOM_PP_ADD_PREFIX_EACH.h"

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_ADD_SUFFIX_EACH(suffix, ...)                                                \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_ADD_SUFFIX_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),             \
        (suffix, ##__VA_ARGS__))
#else
#    define PHANTOM_PP_ADD_SUFFIX_EACH(suffix, ...)                                                \
        PHANTOM_PP_CAT_P()                                                                         \
        PHANTOM_PP_LEFT_PAREN PHANTOM_PP_ADD_SUFFIX_EACH_,                                         \
        PHANTOM_PP_ARGCOUNT(__VA_ARGS__) PHANTOM_PP_RIGHT_PAREN(suffix, ##__VA_ARGS__)
#endif

#define PHANTOM_PP_ADD_SUFFIX_EACH_1(suffix, p0) PHANTOM_PP_CAT(p0, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_2(suffix, p0, p1)                                               \
    PHANTOM_PP_ADD_SUFFIX_EACH_1(suffix, p0), PHANTOM_PP_CAT(p1, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_3(suffix, p0, p1, p2)                                           \
    PHANTOM_PP_ADD_SUFFIX_EACH_2(suffix, p0, p1), PHANTOM_PP_CAT(p2, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_4(suffix, p0, p1, p2, p3)                                       \
    PHANTOM_PP_ADD_SUFFIX_EACH_3(suffix, p0, p1, p2), PHANTOM_PP_CAT(p3, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_5(suffix, p0, p1, p2, p3, p4)                                   \
    PHANTOM_PP_ADD_SUFFIX_EACH_4(suffix, p0, p1, p2, p3), PHANTOM_PP_CAT(p4, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_6(suffix, p0, p1, p2, p3, p4, p5)                               \
    PHANTOM_PP_ADD_SUFFIX_EACH_5(suffix, p0, p1, p2, p3, p4), PHANTOM_PP_CAT(p5, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_7(suffix, p0, p1, p2, p3, p4, p5, p6)                           \
    PHANTOM_PP_ADD_SUFFIX_EACH_6(suffix, p0, p1, p2, p3, p4, p5), PHANTOM_PP_CAT(p6, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_8(suffix, p0, p1, p2, p3, p4, p5, p6, p7)                       \
    PHANTOM_PP_ADD_SUFFIX_EACH_7(suffix, p0, p1, p2, p3, p4, p5, p6), PHANTOM_PP_CAT(p7, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_9(suffix, p0, p1, p2, p3, p4, p5, p6, p7, p8)                   \
    PHANTOM_PP_ADD_SUFFIX_EACH_8(suffix, p0, p1, p2, p3, p4, p5, p6, p7), PHANTOM_PP_CAT(p8, suffix)

#define PHANTOM_PP_ADD_SUFFIX_EACH_10(suffix, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9)              \
    PHANTOM_PP_ADD_SUFFIX_EACH_9(suffix, p0, p1, p2, p3, p4, p5, p6, p7, p8),                      \
    PHANTOM_PP_CAT(p9, suffix)

#define PHANTOM_PP_ADD_PAREN_EACH_LIST(l0) (PHANTOM_PP_ADD_PAREN_EACH l0)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_ADD_PAREN_EACH(...)                                                         \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_ADD_PAREN_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),              \
        (__VA_ARGS__))
#else
#    define PHANTOM_PP_ADD_PAREN_EACH(...)                                                         \
        PHANTOM_PP_CAT_P()                                                                         \
        PHANTOM_PP_LEFT_PAREN PHANTOM_PP_ADD_PAREN_EACH_,                                          \
        PHANTOM_PP_ARGCOUNT(__VA_ARGS__) PHANTOM_PP_RIGHT_PAREN(__VA_ARGS__)
#endif

#define PHANTOM_PP_ADD_PAREN_EACH_1(p0) (p0)

#define PHANTOM_PP_ADD_PAREN_EACH_2(p0, p1) PHANTOM_PP_ADD_PAREN_EACH_1(p0), (p1)

#define PHANTOM_PP_ADD_PAREN_EACH_3(p0, p1, p2) PHANTOM_PP_ADD_PAREN_EACH_2(p0, p1), (p2)

#define PHANTOM_PP_ADD_PAREN_EACH_4(p0, p1, p2, p3) PHANTOM_PP_ADD_PAREN_EACH_3(p0, p1, p2), (p3)

#define PHANTOM_PP_ADD_PAREN_EACH_5(p0, p1, p2, p3, p4)                                            \
    PHANTOM_PP_ADD_PAREN_EACH_4(p0, p1, p2, p3), (p4)

#define PHANTOM_PP_ADD_PAREN_EACH_6(p0, p1, p2, p3, p4, p5)                                        \
    PHANTOM_PP_ADD_PAREN_EACH_5(p0, p1, p2, p3, p4), (p5)

#define PHANTOM_PP_ADD_PAREN_EACH_7(p0, p1, p2, p3, p4, p5, p6)                                    \
    PHANTOM_PP_ADD_PAREN_EACH_6(p0, p1, p2, p3, p4, p5), (p6)

#define PHANTOM_PP_ADD_PAREN_EACH_8(p0, p1, p2, p3, p4, p5, p6, p7)                                \
    PHANTOM_PP_ADD_PAREN_EACH_7(p0, p1, p2, p3, p4, p5, p6), (p7)

#define PHANTOM_PP_ADD_PAREN_EACH_9(p0, p1, p2, p3, p4, p5, p6, p7, p8)                            \
    PHANTOM_PP_ADD_PAREN_EACH_8(p0, p1, p2, p3, p4, p5, p6, p7), (p8)

#define PHANTOM_PP_REMOVE_PAREN_EACH_BY_LIST_3(l0) PHANTOM_PP_REMOVE_PAREN_EACH_BY_LIST_2 l0
#define PHANTOM_PP_REMOVE_PAREN_EACH_BY_LIST_2(l0) PHANTOM_PP_REMOVE_PAREN_EACH_BY_LIST l0
#define PHANTOM_PP_REMOVE_PAREN_EACH_BY_LIST(l0) PHANTOM_PP_REMOVE_PAREN_EACH l0

#define PHANTOM_PP_REMOVE_PAREN_EACH_LIST(l0) (PHANTOM_PP_REMOVE_PAREN_EACH l0)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_REMOVE_PAREN_EACH(...)                                                      \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_REMOVE_PAREN_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),           \
        (__VA_ARGS__))
#else
#    define PHANTOM_PP_REMOVE_PAREN_EACH(...)                                                      \
        PHANTOM_PP_CAT_P()                                                                         \
        PHANTOM_PP_LEFT_PAREN PHANTOM_PP_REMOVE_PAREN_EACH_,                                       \
        PHANTOM_PP_ARGCOUNT(__VA_ARGS__) PHANTOM_PP_RIGHT_PAREN(__VA_ARGS__)
#endif

#define PHANTOM_PP_REMOVE_PAREN_EACH_1(p0) PHANTOM_PP_IDENTITY p0

#define PHANTOM_PP_REMOVE_PAREN_EACH_2(p0, p1)                                                     \
    PHANTOM_PP_REMOVE_PAREN_EACH_1(p0), PHANTOM_PP_IDENTITY p1

#define PHANTOM_PP_REMOVE_PAREN_EACH_3(p0, p1, p2)                                                 \
    PHANTOM_PP_REMOVE_PAREN_EACH_2(p0, p1), PHANTOM_PP_IDENTITY p2

#define PHANTOM_PP_REMOVE_PAREN_EACH_4(p0, p1, p2, p3)                                             \
    PHANTOM_PP_REMOVE_PAREN_EACH_3(p0, p1, p2), PHANTOM_PP_IDENTITY p3

#define PHANTOM_PP_REMOVE_PAREN_EACH_5(p0, p1, p2, p3, p4)                                         \
    PHANTOM_PP_REMOVE_PAREN_EACH_4(p0, p1, p2, p3), PHANTOM_PP_IDENTITY p4

#define PHANTOM_PP_REMOVE_PAREN_EACH_6(p0, p1, p2, p3, p4, p5)                                     \
    PHANTOM_PP_REMOVE_PAREN_EACH_5(p0, p1, p2, p3, p4), PHANTOM_PP_IDENTITY p5

#define PHANTOM_PP_REMOVE_PAREN_EACH_7(p0, p1, p2, p3, p4, p5, p6)                                 \
    PHANTOM_PP_REMOVE_PAREN_EACH_6(p0, p1, p2, p3, p4, p5), PHANTOM_PP_IDENTITY p6

#define PHANTOM_PP_REMOVE_PAREN_EACH_8(p0, p1, p2, p3, p4, p5, p6, p7)                             \
    PHANTOM_PP_REMOVE_PAREN_EACH_7(p0, p1, p2, p3, p4, p5, p6), PHANTOM_PP_IDENTITY p7

#define PHANTOM_PP_REMOVE_PAREN_EACH_9(p0, p1, p2, p3, p4, p5, p6, p7, p8)                         \
    PHANTOM_PP_REMOVE_PAREN_EACH_8(p0, p1, p2, p3, p4, p5, p6, p7), PHANTOM_PP_IDENTITY p8

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_QUOTE_EACH(...)                                                             \
        PHANTOM_PP_CAT_P()                                                                         \
        PHANTOM_PP_LEFT_PAREN PHANTOM_PP_QUOTE_EACH_,                                              \
        PHANTOM_PP_ARGCOUNT(__VA_ARGS__) PHANTOM_PP_RIGHT_PAREN(__VA_ARGS__)
#else
#    define PHANTOM_PP_QUOTE_EACH(...)                                                             \
        PHANTOM_PP_CAT_P()(PHANTOM_PP_QUOTE_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#define PHANTOM_PP_QUOTE_EACH_1(p0) PHANTOM_PP_QUOTE(p0)

#define PHANTOM_PP_QUOTE_EACH_2(p0, p1) PHANTOM_PP_QUOTE_EACH_1(p0), PHANTOM_PP_QUOTE(p1)

#define PHANTOM_PP_QUOTE_EACH_3(p0, p1, p2) PHANTOM_PP_QUOTE_EACH_2(p0, p1), PHANTOM_PP_QUOTE(p2)

#define PHANTOM_PP_QUOTE_EACH_4(p0, p1, p2, p3)                                                    \
    PHANTOM_PP_QUOTE_EACH_3(p0, p1, p2), PHANTOM_PP_QUOTE(p3)

#define PHANTOM_PP_QUOTE_EACH_5(p0, p1, p2, p3, p4)                                                \
    PHANTOM_PP_QUOTE_EACH_4(p0, p1, p2, p3), PHANTOM_PP_QUOTE(p4)

#define PHANTOM_PP_QUOTE_EACH_6(p0, p1, p2, p3, p4, p5)                                            \
    PHANTOM_PP_QUOTE_EACH_5(p0, p1, p2, p3, p4), PHANTOM_PP_QUOTE(p5)

#define PHANTOM_PP_QUOTE_EACH_7(p0, p1, p2, p3, p4, p5, p6)                                        \
    PHANTOM_PP_QUOTE_EACH_6(p0, p1, p2, p3, p4, p5), PHANTOM_PP_QUOTE(p6)

#define PHANTOM_PP_QUOTE_EACH_8(p0, p1, p2, p3, p4, p5, p6, p7)                                    \
    PHANTOM_PP_QUOTE_EACH_7(p0, p1, p2, p3, p4, p5, p6), PHANTOM_PP_QUOTE(p7)

#define PHANTOM_PP_QUOTE_EACH_9(p0, p1, p2, p3, p4, p5, p6, p7, p8)                                \
    PHANTOM_PP_QUOTE_EACH_8(p0, p1, p2, p3, p4, p5, p6, p7), PHANTOM_PP_QUOTE(p8)

#define PHANTOM_PP_SURROUND_L1(p, s, arg) p##arg##s

#define PHANTOM_PP_SURROUND(p, s, arg) PHANTOM_PP_SURROUND_L1(p, s, arg)

#define PHANTOM_PP_SURROUND_EACH_BIS(...)                                                          \
    PHANTOM_PP_SURROUND_EACH PHANTOM_PP_LEFT_PAREN __VA_ARGS__ PHANTOM_PP_RIGHT_PAREN

#define PHANTOM_PP_SURROUND_EACH_BY_LIST(l0) PHANTOM_PP_SURROUND_EACH l0

#define PHANTOM_PP_SURROUND_EACH_BY_LIST_2(l0) PHANTOM_PP_SURROUND_EACH_BY_LIST l0

#define PHANTOM_PP_SURROUND_EACH_BY_LIST_3(l0) PHANTOM_PP_SURROUND_EACH_BY_LIST_2 l0
#define PHANTOM_PP_SURROUND_EACH_BY_LIST_4(l0) PHANTOM_PP_SURROUND_EACH_BY_LIST_3 l0
#define PHANTOM_PP_SURROUND_EACH_BY_LIST_5(l0) PHANTOM_PP_SURROUND_EACH_BY_LIST_4 l0

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_SURROUND_EACH(...)                                                          \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_SURROUND_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)),               \
        (__VA_ARGS__))
#else
#    define PHANTOM_PP_SURROUND_EACH(...)                                                          \
        PHANTOM_PP_CAT(PHANTOM_PP_SURROUND_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#define PHANTOM_PP_SURROUND_EACH_3(p, s, p0) PHANTOM_PP_SURROUND(p, s, p0)

#define PHANTOM_PP_SURROUND_EACH_4(p, s, p0, p1)                                                   \
    PHANTOM_PP_SURROUND_EACH_3(p, s, p0), PHANTOM_PP_SURROUND(p, s, p1)

#define PHANTOM_PP_SURROUND_EACH_5(p, s, p0, p1, p2)                                               \
    PHANTOM_PP_SURROUND_EACH_4(p, s, p0, p1), PHANTOM_PP_SURROUND(p, s, p2)

#define PHANTOM_PP_SURROUND_EACH_6(p, s, p0, p1, p2, p3)                                           \
    PHANTOM_PP_SURROUND_EACH_5(p, s, p0, p1, p2), PHANTOM_PP_SURROUND(p, s, p3)

#define PHANTOM_PP_SURROUND_EACH_7(p, s, p0, p1, p2, p3, p4)                                       \
    PHANTOM_PP_SURROUND_EACH_6(p, s, p0, p1, p2, p3), PHANTOM_PP_SURROUND(p, s, p4)

#define PHANTOM_PP_SURROUND_EACH_8(p, s, p0, p1, p2, p3, p4, p5)                                   \
    PHANTOM_PP_SURROUND_EACH_7(p, s, p0, p1, p2, p3, p4), PHANTOM_PP_SURROUND(p, s, p5)

#define PHANTOM_PP_SURROUND_EACH_9(p, s, p0, p1, p2, p3, p4, p5, p6)                               \
    PHANTOM_PP_SURROUND_EACH_8(p, s, p0, p1, p2, p3, p4, p5), PHANTOM_PP_SURROUND(p, s, p6)

#define PHANTOM_PP_SURROUND_EACH_10(p, s, p0, p1, p2, p3, p4, p5, p6, p7)                          \
    PHANTOM_PP_SURROUND_EACH_9(p, s, p0, p1, p2, p3, p4, p5, p6), PHANTOM_PP_SURROUND(p, s, p7)

#define PHANTOM_PP_SURROUND_EACH_11(p, s, p0, p1, p2, p3, p4, p5, p6, p7, p8)                      \
    PHANTOM_PP_SURROUND_EACH_10(p, s, p0, p1, p2, p3, p4, p5, p6, p7), PHANTOM_PP_SURROUND(p, s, p8)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, arg) tpl<arg>

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH(...)                                            \
        PHANTOM_PP_CAT(                                                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), \
        (__VA_ARGS__))
#else
#    define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH(...)                                            \
        PHANTOM_PP_CAT(PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))  \
        (__VA_ARGS__)
#endif

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_2(tpl, p0) PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p0)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_3(tpl, p0, p1)                                      \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_2(tpl, p0), PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p1)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_4(tpl, p0, p1, p2)                                  \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_3(tpl, p0, p1),                                         \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p2)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_5(tpl, p0, p1, p2, p3)                              \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_4(tpl, p0, p1, p2),                                     \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p3)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_6(tpl, p0, p1, p2, p3, p4)                          \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_5(tpl, p0, p1, p2, p3),                                 \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p4)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_7(tpl, p0, p1, p2, p3, p4, p5)                      \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_6(tpl, p0, p1, p2, p3, p4),                             \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p5)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_8(tpl, p0, p1, p2, p3, p4, p5, p6)                  \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_7(tpl, p0, p1, p2, p3, p4, p5),                         \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p6)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_9(tpl, p0, p1, p2, p3, p4, p5, p6, p7)              \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_8(tpl, p0, p1, p2, p3, p4, p5, p6),                     \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p7)

#define PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_10(tpl, p0, p1, p2, p3, p4, p5, p6, p7, p8)         \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE_EACH_9(tpl, p0, p1, p2, p3, p4, p5, p6, p7),                 \
    PHANTOM_PP_SURROUND_WITH_TEMPLATE(tpl, p8)

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_PP_MIX(l0, l1)                                                                 \
        PHANTOM_PP_CAT PHANTOM_PP_LEFT_PAREN PHANTOM_PP_MIX_,                                      \
        PHANTOM_PP_ARGCOUNT_FUNC() l0        PHANTOM_PP_RIGHT_PAREN(l0, l1)

#    define PHANTOM_PP_MIX_0(l0, l1)

#    define PHANTOM_PP_MIX_1(l0, l1)                                                               \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_0, l0))                                 \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_0, l1))

#    define PHANTOM_PP_MIX_2(l0, l1)                                                               \
        PHANTOM_PP_MIX_1(l0, l1),                                                                  \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_1, l0))                                 \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_1, l1))

#    define PHANTOM_PP_MIX_3(l0, l1)                                                               \
        PHANTOM_PP_MIX_2(l0, l1),                                                                  \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_2, l0))                                 \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_2, l1))

#    define PHANTOM_PP_MIX_4(l0, l1)                                                               \
        PHANTOM_PP_MIX_3(l0, l1),                                                                  \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_3, l0))                                 \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_3, l1))

#    define PHANTOM_PP_MIX_5(l0, l1)                                                               \
        PHANTOM_PP_MIX_4(l0, l1),                                                                  \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_4, l0))                                 \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_4, l1))

#    define PHANTOM_PP_MIX_6(l0, l1)                                                               \
        PHANTOM_PP_MIX_5(l0, l1),                                                                  \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_5, l0))                                 \
        PHANTOM_PP_BY_LIST(PHANTOM_PP_CAT, (PHANTOM_PP_ARG_5, l1))

#    define PHANTOM_PP_MIX_7(l0, l1)                                                               \
        PHANTOM_PP_MIX_6(l0, l1),                                                                  \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_6, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_6, l1)

#    define PHANTOM_PP_MIX_8(l0, l1)                                                               \
        PHANTOM_PP_MIX_7(l0, l1),                                                                  \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_7, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_7, l1)

#    define PHANTOM_PP_MIX_9(l0, l1)                                                               \
        PHANTOM_PP_MIX_8(l0, l1),                                                                  \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_8, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_8, l1)

#    define PHANTOM_PP_MIX_10(l0, l1)                                                              \
        PHANTOM_PP_MIX_9(l0, l1),                                                                  \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_9, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_9, l1)

#    define PHANTOM_PP_MIX_11(l0, l1)                                                              \
        PHANTOM_PP_MIX_10(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_10, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_10, l1)

#    define PHANTOM_PP_MIX_12(l0, l1)                                                              \
        PHANTOM_PP_MIX_11(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_11, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_11, l1)

#    define PHANTOM_PP_MIX_13(l0, l1)                                                              \
        PHANTOM_PP_MIX_12(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_12, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_12, l1)

#    define PHANTOM_PP_MIX_14(l0, l1)                                                              \
        PHANTOM_PP_MIX_13(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_13, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_13, l1)

#    define PHANTOM_PP_MIX_15(l0, l1)                                                              \
        PHANTOM_PP_MIX_14(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_14, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_14, l1)

#    define PHANTOM_PP_MIX_16(l0, l1)                                                              \
        PHANTOM_PP_MIX_15(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_15, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_15, l1)

#    define PHANTOM_PP_MIX_17(l0, l1)                                                              \
        PHANTOM_PP_MIX_16(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_16, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_16, l1)

#    define PHANTOM_PP_MIX_18(l0, l1)                                                              \
        PHANTOM_PP_MIX_17(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_17, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_17, l1)

#    define PHANTOM_PP_MIX_19(l0, l1)                                                              \
        PHANTOM_PP_MIX_18(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_18, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_18, l1)

#    define PHANTOM_PP_MIX_20(l0, l1)                                                              \
        PHANTOM_PP_MIX_19(l0, l1),                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_ARG_19, l0) PHANTOM_PP_CAT(PHANTOM_PP_ARG_19, l1)

#else

#    define PHANTOM_PP_MIX(l0, l1)                                                                 \
        PHANTOM_PP_CAT(PHANTOM_PP_MIX_, PHANTOM_PP_ARGCOUNT_FUNC() l0)(l0, l1)

#    define PHANTOM_PP_MIX_0(l0, l1)

#    define PHANTOM_PP_MIX_1(l0, l1) PHANTOM_PP_ARG_0 l0 PHANTOM_PP_ARG_0 l1

#    define PHANTOM_PP_MIX_2(l0, l1)                                                               \
        PHANTOM_PP_MIX_1(l0, l1), PHANTOM_PP_ARG_1 l0 PHANTOM_PP_ARG_1 l1

#    define PHANTOM_PP_MIX_3(l0, l1)                                                               \
        PHANTOM_PP_MIX_2(l0, l1), PHANTOM_PP_ARG_2 l0 PHANTOM_PP_ARG_2 l1

#    define PHANTOM_PP_MIX_4(l0, l1)                                                               \
        PHANTOM_PP_MIX_3(l0, l1), PHANTOM_PP_ARG_3 l0 PHANTOM_PP_ARG_3 l1

#    define PHANTOM_PP_MIX_5(l0, l1)                                                               \
        PHANTOM_PP_MIX_4(l0, l1), PHANTOM_PP_ARG_4 l0 PHANTOM_PP_ARG_4 l1

#    define PHANTOM_PP_MIX_6(l0, l1)                                                               \
        PHANTOM_PP_MIX_5(l0, l1), PHANTOM_PP_ARG_5 l0 PHANTOM_PP_ARG_5 l1

#    define PHANTOM_PP_MIX_7(l0, l1)                                                               \
        PHANTOM_PP_MIX_6(l0, l1), PHANTOM_PP_ARG_6 l0 PHANTOM_PP_ARG_6 l1

#    define PHANTOM_PP_MIX_8(l0, l1)                                                               \
        PHANTOM_PP_MIX_7(l0, l1), PHANTOM_PP_ARG_7 l0 PHANTOM_PP_ARG_7 l1

#    define PHANTOM_PP_MIX_9(l0, l1)                                                               \
        PHANTOM_PP_MIX_8(l0, l1), PHANTOM_PP_ARG_8 l0 PHANTOM_PP_ARG_8 l1

#    define PHANTOM_PP_MIX_10(l0, l1)                                                              \
        PHANTOM_PP_MIX_9(l0, l1), PHANTOM_PP_ARG_9 l0 PHANTOM_PP_ARG_9 l1

#    define PHANTOM_PP_MIX_11(l0, l1)                                                              \
        PHANTOM_PP_MIX_10(l0, l1), PHANTOM_PP_ARG_10 l0 PHANTOM_PP_ARG_10 l1

#    define PHANTOM_PP_MIX_12(l0, l1)                                                              \
        PHANTOM_PP_MIX_11(l0, l1), PHANTOM_PP_ARG_11 l0 PHANTOM_PP_ARG_11 l1

#    define PHANTOM_PP_MIX_13(l0, l1)                                                              \
        PHANTOM_PP_MIX_12(l0, l1), PHANTOM_PP_ARG_12 l0 PHANTOM_PP_ARG_12 l1

#    define PHANTOM_PP_MIX_14(l0, l1)                                                              \
        PHANTOM_PP_MIX_13(l0, l1), PHANTOM_PP_ARG_13 l0 PHANTOM_PP_ARG_13 l1

#    define PHANTOM_PP_MIX_15(l0, l1)                                                              \
        PHANTOM_PP_MIX_14(l0, l1), PHANTOM_PP_ARG_14 l0 PHANTOM_PP_ARG_14 l1

#    define PHANTOM_PP_MIX_16(l0, l1)                                                              \
        PHANTOM_PP_MIX_15(l0, l1), PHANTOM_PP_ARG_15 l0 PHANTOM_PP_ARG_15 l1

#    define PHANTOM_PP_MIX_17(l0, l1)                                                              \
        PHANTOM_PP_MIX_16(l0, l1), PHANTOM_PP_ARG_16 l0 PHANTOM_PP_ARG_16 l1

#    define PHANTOM_PP_MIX_18(l0, l1)                                                              \
        PHANTOM_PP_MIX_17(l0, l1), PHANTOM_PP_ARG_17 l0 PHANTOM_PP_ARG_17 l1

#    define PHANTOM_PP_MIX_19(l0, l1)                                                              \
        PHANTOM_PP_MIX_18(l0, l1), PHANTOM_PP_ARG_18 l0 PHANTOM_PP_ARG_18 l1

#    define PHANTOM_PP_MIX_20(l0, l1)                                                              \
        PHANTOM_PP_MIX_19(l0, l1), PHANTOM_PP_ARG_19 l0 PHANTOM_PP_ARG_19 l1

#endif

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST1 t_0
#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST2 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST1, t_1

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST3 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST2, t_2

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST4 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST3, t_3

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST5 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST4, t_4

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST6 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST5, t_5

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST7 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST6, t_6

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST8 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST7, t_7

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST9 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST8, t_8

#define PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST10 PHANTOM_PP_CREATE_TEMPLATE_ARGUMENT_LIST9, t_9

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PP_QUOTE_SCOPE(_namespace_)                                                    \
        PHANTOM_PP_QUOTE PHANTOM_PP_LEFT_PAREN PHANTOM_PP_CREATE_SCOPE _namespace_                 \
                                                                       PHANTOM_PP_RIGHT_PAREN
#else
#    define PHANTOM_PP_QUOTE_SCOPE(_namespace_)                                                    \
        PHANTOM_PP_QUOTE(PHANTOM_PP_CREATE_SCOPE _namespace_)
#endif

#define PHANTOM_PP_CREATE_SCOPE(...) PHANTOM_PP_VARARG(_PHNTM_PP_CREATE_SCOPE_, ##__VA_ARGS__)
#define PHANTOM_PP_CREATE_SCOPE_IDENTIFIER(...)                                                    \
    PHANTOM_PP_VARARG(_PHNTM_PP_CREATE_SCOPE_IDENTIFIER_, ##__VA_ARGS__)

#define _PHNTM_PP_CREATE_SCOPE_0()
#define _PHNTM_PP_CREATE_SCOPE_1(n0) n0
#define _PHNTM_PP_CREATE_SCOPE_2(n0, n1) n0::n1
#define _PHNTM_PP_CREATE_SCOPE_3(n0, n1, n2) n0::n1::n2
#define _PHNTM_PP_CREATE_SCOPE_4(n0, n1, n2, n3) n0::n1::n2::n3
#define _PHNTM_PP_CREATE_SCOPE_5(n0, n1, n2, n3, n4) n0::n1::n2::n3::n4
#define _PHNTM_PP_CREATE_SCOPE_6(n0, n1, n2, n3, n4, n5) n0::n1::n2::n3::n4::n5
#define _PHNTM_PP_CREATE_SCOPE_7(n0, n1, n2, n3, n4, n5, n6) n0::n1::n2::n3::n4::n5::n6
#define _PHNTM_PP_CREATE_SCOPE_8(n0, n1, n2, n3, n4, n5, n6, n7) n0::n1::n2::n3::n4::n5::n6::n7
#define _PHNTM_PP_CREATE_SCOPE_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)                               \
    n0::n1::n2::n3::n4::n5::n6::n7::n8
#define _PHNTM_PP_CREATE_SCOPE_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)                          \
    n0::n1::n2::n3::n4::n5::n6::n7::n8::n9

#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_0()
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_1(n0) n0
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_2(n0, n1) n0##_N##n1
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_3(n0, n1, n2) n0##_N##n1##_N##n2
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_4(n0, n1, n2, n3) n0##_N##n1##_N##n2##_N##n3
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_5(n0, n1, n2, n3, n4) n0##_N##n1##_N##n2##_N##n3##_N##n4
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_6(n0, n1, n2, n3, n4, n5)                                \
    n0##_N##n1##_N##n2##_N##n3##_N##n4##_N##n5
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_7(n0, n1, n2, n3, n4, n5, n6)                            \
    n0##_N##n1##_N##n2##_N##n3##_N##n4##_N##n5##_N##n6
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_8(n0, n1, n2, n3, n4, n5, n6, n7)                        \
    n0##_N##n1##_N##n2##_N##n3##_N##n4##_N##n5##_N##n6##_N##n7
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_9(n0, n1, n2, n3, n4, n5, n6, n7, n8)                    \
    n0##_N##n1##_N##n2##_N##n3##_N##n4##_N##n5##_N##n6##_N##n7##_N##n8
#define _PHNTM_PP_CREATE_SCOPE_IDENTIFIER_10(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9)               \
    n0##_N##n1##_N##n2##_N##n3##_N##n4##_N##n5##_N##n6##_N##n7##_N##n8##_N##n9

#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES(...)                                              \
    PHANTOM_PP_CAT(PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))()

#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_1() typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_2()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_1(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_3()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_2(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_4()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_3(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_5()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_4(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_6()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_5(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_7()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_6(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_8()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_7(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_9()                                               \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_8(), typename
#define PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_10()                                              \
    PHANTOM_PP_CREATE_DEFAULT_TEMPLATE_TYPES_9(), typename

#define PHANTOM_PP_CREATE_QUALIFIED_NAME(_namespaces_, _name_)                                     \
    ::PHANTOM_PP_CREATE_SCOPE _namespaces_ ::_name_

#define PHANTOM_PP_CREATE_QUALIFIED_NAME_RELATIVE(_namespaces_, _name_)                            \
    PHANTOM_PP_CREATE_SCOPE _namespaces_ ::_name_

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

#    define PHANTOM_PP_CREATE_QUALIFIED_IDENTIFIER_NAME(_namespaces_, _name_)                      \
        PHANTOM_PP_IDENTITY(                                                                       \
        PHANTOM_PP_IDENTITY PHANTOM_PP_LEFT_PAREN _PHNTM_PP_CREATE_SCOPE_IDENTIFIER                \
        _namespaces_##_##_name_ PHANTOM_PP_RIGHT_PAREN)

#    define PHANTOM_PP_CREATE_QUALIFIED_NAME_2(_namespaces_, _classes_, _name_)                    \
        ::PHANTOM_PP_CREATE_SCOPE _namespaces_ ::PHANTOM_PP_CREATE_SCOPE _classes_ ::_name_

#else

#    define PHANTOM_PP_CREATE_QUALIFIED_IDENTIFIER_NAME(_namespaces_, _name_)                      \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_PP_CREATE_SCOPE_IDENTIFIER _namespaces_, _), _name_)

#    define PHANTOM_PP_CREATE_QUALIFIED_NAME_2(_namespaces_, _classes_, _name_)                    \
        ::PHANTOM_PP_CREATE_SCOPE _namespaces_ ::PHANTOM_PP_CREATE_SCOPE _classes_ ::_name_

#endif

#define PHANTOM_PP_DEC(Num) _PHNTM_PP_DEC_I(Num)
#define _PHNTM_PP_DEC_I(Num) _PHNTM_PP_DEC_II(Num)
#define _PHNTM_PP_DEC_II(Num) PHANTOM_PP_DEC_##Num

#include "detail/PP/PHANTOM_PP_DEC.h"

// tests

#define _TEST_PP_VARARG(...) PHANTOM_PP_VARARG(_TEST_PP_VARARG, __VA_ARGS__)

#define _TEST_PP_VARARG0() 0
#define _TEST_PP_VARARG1(a) 1
#define _TEST_PP_VARARG2(a, b) 2

#if _TEST_PP_VARARG() != 0
#    error PHANTOM_PP_VARARG invalid
#endif
#if _TEST_PP_VARARG(a) != 1
#    error PHANTOM_PP_VARARG invalid
#endif
#if _TEST_PP_VARARG(a, b) != 2
#    error PHANTOM_PP_VARARG invalid
#endif
