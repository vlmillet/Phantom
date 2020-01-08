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
#include <phantom/utils/Functor.h>

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
        static void _() {}                                                                                             \
    };                                                                                                                 \
    phantom::reflection::Main _PHNTM_main(PHANTOM_MODULE_HANDLE((void*)&_PHNTM_main_s::_), name, __FILE__, flags,      \
                                          argc, argv);

namespace phantom
{
using MessageReportFunc =
Functor<bool(StringView expression, StringView file, int line, const char* format, va_list arglist)>;

using LogFunc = Functor<void(MessageType msgType, StringView file, int line, const char* format, va_list arglist)>;

namespace reflection
{
enum class ClassHookOp
{
    InstanceRegistered,
    InstanceUnregistering,
    KindCreated,
    KindDestroying,
    ClassDestroying,
};

using ClassHookFunc = Functor<void(ClassHookOp op, Class* a_pClass, void* a_pInstance)>;

class PHANTOM_EXPORT_PHANTOM Main
{
public:
    Main(int (*_mainFuncPtr)(int, char**), StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, argc, argv, _allocator, a_ClassHookFunc,
               a_strMainCppFile, a_uiFlags)
    {
    }
    Main(int (*_mainFuncPtr)(), StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, argc, argv, _allocator, a_ClassHookFunc,
               a_strMainCppFile, a_uiFlags)
    {
    }
    Main(void* _mainFuncPtr, StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, argc, argv, _allocator, a_ClassHookFunc,
               a_strMainCppFile, a_uiFlags)
    {
    }
    Main(size_t a_ModuleHandle, StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0);
    ~Main();

    void setAssertFunc(MessageReportFunc a_func);
    void setErrorFunc(MessageReportFunc a_func);
    void setLogFunc(LogFunc a_func);
    void setWarningFunc(MessageReportFunc a_func);
};
} // namespace reflection
} // namespace phantom

#define o_main PHANTOM_MAIN
