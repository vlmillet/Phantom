// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/lang/reflection.h>
#include <phantom/utils/Functor.h>

namespace phantom
{
using MessageReportFunc =
Functor<bool(StringView expression, StringView file, int line, const char* format, va_list arglist)>;

using LogFunc = Functor<void(MessageType msgType, StringView file, int line, const char* format, va_list arglist)>;

namespace lang
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
} // namespace lang
} // namespace phantom
