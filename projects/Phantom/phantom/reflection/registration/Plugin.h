// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/String.h>
#include <phantom/reflection/reflection.h>
#include <phantom/reflection/registration/registration.h>
/* **************** Declarations ***************** */
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include <windows.h>
#elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS
#    include <_kernel.h>
#elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
#    include <dlfcn.h>
#endif
/* *********************************************** */

#if !defined(PHANTOM_PLUGIN_DEPENDENCIES)
//#	if defined(PHANTOM_STATIC_LIB_HANDLE)
#    define PHANTOM_PLUGIN_DEPENDENCIES_MISSING_MSG                                                                    \
        "static lib has missing PHANTOM_PLUGIN_DEPENDENCIES macro defined at the project level "                       \
        "to ensure correct module auto-installation order, for example "                                               \
        "'PHANTOM_PLUGIN_DEPENDENCIES=(\"MyDependencyProject0\", \"MyDependencyProject1\")'"
#    if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#        pragma message(PHANTOM_PLUGIN_DEPENDENCIES_MISSING_MSG)
#    else
#        pragma message PHANTOM_PLUGIN_DEPENDENCIES_MISSING_MSG
#    endif
//#	endif
#    define PHANTOM_PLUGIN_DEPENDENCIES PHANTOM_PP_LEFT_PAREN PHANTOM_PP_RIGHT_PAREN
#endif

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define PHANTOM_PLUGIN(...)                                                                                        \
        PHANTOM_PP_CAT(PHANTOM_PP_CAT(_PHNTM_PLUGIN_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__)), (__VA_ARGS__))
#else
#    define PHANTOM_PLUGIN(...) PHANTOM_PP_CAT(_PHNTM_PLUGIN_, PHANTOM_PP_ARGCOUNT(__VA_ARGS__))(__VA_ARGS__)
#endif

inline void _PHNTM_PLUGIN_EMPTY_FUNC()
{
}

#define _PHNTM_PLUGIN_1(name) _PHNTM_PLUGIN_4(name, PHANTOM_R_NONE, _PHNTM_PLUGIN_EMPTY_FUNC, _PHNTM_PLUGIN_EMPTY_FUNC);
#define _PHNTM_PLUGIN_2(name, flags) _PHNTM_PLUGIN_4(name, flags, _PHNTM_PLUGIN_EMPTY_FUNC, _PHNTM_PLUGIN_EMPTY_FUNC);
#define _PHNTM_PLUGIN_3(name, flags, func_on_load) _PHNTM_PLUGIN_4(name, flags, func_on_load, _PHNTM_PLUGIN_EMPTY_FUNC);

#define PHANTOM_PLUGIN_DEPENDENCIES_UNROLL(m) PHANTOM_PP_IDENTITY m

#if defined(PHANTOM_STATIC_LIB_HANDLE)

#    define _PHNTM_PLUGIN_4(name, flags, func_on_load, func_on_unload)                                                 \
        static struct PHANTOM_PP_CAT(_PHNTM_static_plugin, PHANTOM_STATIC_LIB_HANDLE)                                  \
        {                                                                                                              \
            PHANTOM_PP_CAT(_PHNTM_static_plugin, PHANTOM_STATIC_LIB_HANDLE)()                                          \
            {                                                                                                          \
                static int address_dummy;                                                                              \
                ::phantom::reflection::detail::registerModule(                                                         \
                PHANTOM_STATIC_LIB_HANDLE, name, "", __FILE__, flags,                                                  \
                {PHANTOM_PLUGIN_DEPENDENCIES_UNROLL(PHANTOM_PLUGIN_DEPENDENCIES)}, func_on_load, func_on_unload);      \
            }                                                                                                          \
            ~PHANTOM_PP_CAT(_PHNTM_static_plugin, PHANTOM_STATIC_LIB_HANDLE)()                                         \
            {                                                                                                          \
            }                                                                                                          \
        } PHANTOM_PP_CAT(_PHNTM_static_plugin_instance, PHANTOM_STATIC_LIB_HANDLE);

#else // DYNAMIC LINKING

#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

#        define _PHNTM_PLUGIN_4(name, flags, func_on_load, func_on_unload)                                             \
            BOOL WINAPI DllMain(HINSTANCE _HDllHandle, DWORD _Reason, LPVOID)                                          \
            {                                                                                                          \
                switch (_Reason)                                                                                       \
                {                                                                                                      \
                case DLL_PROCESS_ATTACH:                                                                               \
                {                                                                                                      \
                    static char moduleName[512];                                                                       \
                    GetModuleFileNameA(_HDllHandle, moduleName, 512);                                                  \
                    static_assert(sizeof(name) > 1, "plugin must have a non-empty name");                              \
                    ::phantom::reflection::detail::registerModule(                                                     \
                    (::phantom::size_t)_HDllHandle, name, moduleName, __FILE__, flags,                                 \
                    {PHANTOM_PLUGIN_DEPENDENCIES_UNROLL(PHANTOM_PLUGIN_DEPENDENCIES)}, func_on_load, func_on_unload);  \
                }                                                                                                      \
                break;                                                                                                 \
                case DLL_PROCESS_DETACH:                                                                               \
                    ::phantom::reflection::detail::unregisterModule((::phantom::size_t)_HDllHandle);                   \
                    break;                                                                                             \
                case DLL_THREAD_ATTACH:                                                                                \
                    break;                                                                                             \
                case DLL_THREAD_DETACH:                                                                                \
                    break;                                                                                             \
                }                                                                                                      \
                return TRUE;                                                                                           \
            }

#    elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS

#        define _PHNTM_PLUGIN_4(name, flags, func_on_load, func_on_unload)                                             \
            int module_start(size_t argc, const void* argv)                                                            \
            {                                                                                                          \
                ::phantom::reflection::detail::registerModule(                                                         \
                (size_t)module_stop, name, *(const char**)argv, __FILE__, flags,                                       \
                {PHANTOM_PLUGIN_DEPENDENCIES_UNROLL(PHANTOM_PLUGIN_DEPENDENCIES)}, func_on_load, func_on_unload);      \
                return 0;                                                                                              \
            }                                                                                                          \
            int module_stop(size_t, const void*)                                                                       \
            {                                                                                                          \
                ::phantom::reflection::detail::unregisterModule((size_t)module_stop);                                  \
                return 0;                                                                                              \
            }

#    elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX // -- other unix

#        define _PHNTM_PLUGIN_4(name, flags, func_on_load, func_on_unload)                                             \
            __attribute__((constructor(50000))) static void _shared_library_posix_load()                               \
            {                                                                                                          \
                static Dl_info info;                                                                                   \
                dladdr((const void*)&_shared_library_posix_load, &info);                                               \
                ::phantom::reflection::detail::registerModule(                                                         \
                (size_t)info.dli_fbase, name, info.dli_fname, __FILE__, flags,                                         \
                {PHANTOM_PLUGIN_DEPENDENCIES_UNROLL(PHANTOM_PLUGIN_DEPENDENCIES)}, func_on_load, func_on_unload);      \
            }                                                                                                          \
            __attribute__((destructor(500))) static void _shared_library_posix_unload()                                \
            {                                                                                                          \
                static Dl_info info;                                                                                   \
                dladdr((const void*)&_shared_library_posix_unload, &info);                                             \
                ::phantom::reflection::detail::unregisterModule((size_t)info.dli_fbase);                               \
            }
#    else

#        error no shared library system handled by this platform

#    endif

#endif // PHANTOM_STATIC_LIB_HANDLE

#define o_plugin PHANTOM_PLUGIN
