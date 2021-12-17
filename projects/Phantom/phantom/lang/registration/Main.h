// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "registration.h"

#include <haunt>
#include <initializer_list>
#include <phantom/lang/reflection.h>
#include <phantom/utils/Functor.h>
#include <phantom/utils/StringView.h>

namespace phantom
{
using MessageReportFunc = Functor<bool(StringView expression, StringView file, int line, StringView)>;

using LogFunc = Functor<void(MessageType msgType, StringView file, int line, StringView)>;

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
    static Main* Get();

    /// @off
    Main(int (*_mainFuncPtr)(int, char**), StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, {}, argc, argv, _allocator, a_ClassHookFunc,
               a_strMainCppFile, a_uiFlags)
    {
    }
    /// @off
    Main(int (*_mainFuncPtr)(int, char**), StringView a_strMainModuleName,
         std::initializer_list<StringView> _dependencies, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, _dependencies, argc, argv, _allocator,
               a_ClassHookFunc, a_strMainCppFile, a_uiFlags)
    {
    }
    /// @off
    Main(int (*_mainFuncPtr)(), StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, {}, argc, argv, _allocator, a_ClassHookFunc,
               a_strMainCppFile, a_uiFlags)
    {
    }
    /// @off
    Main(void* _mainFuncPtr, StringView a_strMainModuleName, int argc = 0, char** argv = nullptr,
         CustomAllocator _allocator = CustomAllocator::Default(), ClassHookFunc a_ClassHookFunc = ClassHookFunc(),
         StringView a_strMainCppFile = "", uint a_uiFlags = 0)
        : Main(PHANTOM_MODULE_HANDLE(_mainFuncPtr), a_strMainModuleName, {}, argc, argv, _allocator, a_ClassHookFunc,
               a_strMainCppFile, a_uiFlags)
    {
    }
    /// @off
    Main(size_t a_ModuleHandle, StringView a_strMainModuleName, std::initializer_list<StringView> _dependencies,
         int argc = 0, char** argv = nullptr, CustomAllocator _allocator = CustomAllocator::Default(),
         ClassHookFunc a_ClassHookFunc = ClassHookFunc(), StringView a_strMainCppFile = "", uint a_uiFlags = 0);
    ~Main();

    void setAssertFunc(MessageReportFunc a_func);
    void setErrorFunc(MessageReportFunc a_func);
    void setLogFunc(LogFunc const& a_func);
    void setWarningFunc(MessageReportFunc a_func);

    MessageReportFunc const& getAssertFunc(MessageReportFunc a_func) const;
    MessageReportFunc const& getErrorFunc(MessageReportFunc a_func) const;
    LogFunc const&           getLogFunc() const;
    MessageReportFunc const& getWarningFunc() const;
};
} // namespace lang
} // namespace phantom
