// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/reflection/reflection.h>

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_MAIN(...)                                                                                          \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_MAIN_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#else
#    define PHANTOM_MAIN(...) PHANTOM_PP_CAT(_PHNTM_MAIN_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

#define _PHNTM_MAIN_MODULE_HANDLE PHANTOM_MODULE_HANDLE((void*)&_PHNTM_main_s::_)

#define _PHNTM_MAIN_1(name) _PHNTM_MAIN_2(name, PHANTOM_R_NONE)

#define _PHNTM_MAIN_2(name, flags) _PHNTM_MAIN_4(name, flags, 0, nullptr)

#define _PHNTM_MAIN_3(name, argc, argv) _PHNTM_MAIN_4(name, PHANTOM_R_NONE, argc, argv)

#define _PHNTM_MAIN_4(name, flags, argc, argv)                                                                         \
    struct _PHNTM_main_s                                                                                               \
    {                                                                                                                  \
        static void _()                                                                                                \
        {                                                                                                              \
        }                                                                                                              \
    };                                                                                                                 \
    phantom::reflection::Main _PHNTM_main(PHANTOM_MODULE_HANDLE((void*)&_PHNTM_main_s::_), name, __FILE__, flags,      \
                                          argc, argv);

namespace phantom
{
namespace reflection
{
class PHANTOM_EXPORT_PHANTOM Main
{
public:
    Main(int (*_mainFuncPtr)(int, char**), StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, argc, argv, a_strMainCppFile, a_uiFlags)
    {
    }
    Main(int (*_mainFuncPtr)(), StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, argc, argv, a_strMainCppFile, a_uiFlags)
    {
    }
    Main(void* _mainFuncPtr, StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, argc, argv, a_strMainCppFile, a_uiFlags)
    {
    }
    Main(size_t a_ModuleHandle, StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         StringView a_strMainCppFile = "", uint a_uiFlags = 0);
    ~Main();
};
} // namespace reflection
} // namespace phantom

#define o_main PHANTOM_MAIN
