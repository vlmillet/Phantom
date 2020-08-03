// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Plugin.h"

#include "Application.h"
#include "Module.h"
#include "Source.h"

#include <phantom/utils/Path.h>
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include "windows.h"

#    include <TlHelp32.h>
#elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS
#    include <_kernel.h>
#elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
#    include <dlfcn.h>
#endif
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_MACOSX
#    include <mach-o/dyld.h>
#    include <mach-o/getsect.h>
#endif
#include "Function.h"
#include "Package.h"
#include "SourceStream.h"
#include "registration/registration.h"

#include <fstream>
#include <phantom/utils/StringUtil.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
static Plugins g_LoadingPlugins;
static Plugins g_UnloadingPlugins;

bool Plugin::HasLoadingInProgress()
{
    return g_LoadingPlugins.size() != 0;
}

bool Plugin::HasLoadingInProgress(Module* a_pModule)
{
    return std::find_if(g_LoadingPlugins.begin(), g_LoadingPlugins.end(), [=](Plugin* a_pPlugin) -> bool {
               return a_pPlugin->getName() == a_pModule->getName();
           }) != g_LoadingPlugins.end();
}

void Plugin::deleteOnDisk()
{
    PHANTOM_ASSERT(m_pModule == nullptr || !m_pModule->isNative(), "cannot delete native modules on disk");
    String name = m_strName;
    String path = m_strFilePath;
    PHANTOM_ASSERT(m_RefCount == 0);
    std::error_code ec;
    PHANTOM_VERIFY(Path::RemoveAll(path, ec) && ec.value() != 0);
}

bool Plugin::HasUnloadingInProgress()
{
    return g_UnloadingPlugins.size() != 0;
}

Plugins const& Plugin::GetLoadingPluginStack()
{
    return g_LoadingPlugins;
}

Plugins const& Plugin::GetUnloadingPluginStack()
{
    return g_UnloadingPlugins;
}

Plugin::Plugin(StringView a_strName, StringView a_strFilePath) : m_strFilePath(a_strFilePath), m_strName(a_strName)
{
    PHANTOM_ASSERT(Package::IsValidName(m_strName),
                   "invalid plugin name '%.*s' (only [a-zA-Z0-9_.] characters are allowed)",
                   PHANTOM_STRING_AS_PRINTF_ARG(a_strName));
    if (m_strFilePath.size())
        _loadFile();
}

Plugin::Plugin(StringView a_strFilePath) : m_strFilePath(a_strFilePath)
{
    PHANTOM_ASSERT(!a_strFilePath.empty());
    m_strName = Path(a_strFilePath).stem();
    _loadFile();
}

struct OperationCounterGuard
{
    OperationCounterGuard(size_t* counter) : m_counter(counter) { (*m_counter)++; }
    ~OperationCounterGuard() { (*m_counter)--; }
    size_t* m_counter;
};

struct LoadingPluginsGuard
{
    LoadingPluginsGuard(Plugins* plugins) : m_guarded(plugins) {}
    ~LoadingPluginsGuard() { m_guarded->pop_back(); }
    Plugins* m_guarded;
};

bool Plugin::_isLoadingModule(Module* a_pModule) const
{
    PHANTOM_ASSERT(a_pModule->getName() == m_strName);
    return (g_LoadingPlugins.size() != 0);
}

bool Plugin::_loadNative(StringView a_strPath)
{
#if PHANTOM_OPERATING_SYSTEM ==                                                                                        \
PHANTOM_OPERATING_SYSTEM_WINDOWS // ===================================================================================================

    HMODULE hmodule = 0;
    if (hmodule = LoadLibraryA(String(a_strPath).c_str()))
    {
        detail::pushInstallation();
        detail::installModules();
        detail::popInstallation();
        PHANTOM_LOG(Information, "Dll loaded : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(a_strPath));
        PHANTOM_ASSERT(m_pModule == nullptr);
        m_pModule = Application::Get()->getModule(m_strName);
        if (m_pModule == nullptr)
        {
            PHANTOM_LOG(Error,
                        "'PHANTOM_PLUGIN(\"%s\");' was not found in any loaded module while "
                        "loading the plugin '%s' ; aborting ...",
                        m_strName.c_str());
            FreeLibrary(hmodule);
            return false;
        }
        return true;
    }
    else
    {
        DWORD dw = GetLastError();
        PHANTOM_LOG(Error, "Cannot load module : %s", m_strName.c_str());
        LPVOID lpMsgBuf;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);

        String clampedMessage = (char*)lpMsgBuf;
        clampedMessage = clampedMessage.substr(0, clampedMessage.find_first_of("\r\n"));
        PHANTOM_LOG(Error, "System DLL loading failed : %s", clampedMessage.c_str());
        LocalFree(lpMsgBuf);
        return false;
    }

#elif PHANTOM_OPERATING_SYSTEM ==                                                                                      \
PHANTOM_OPERATING_SYSTEM_ORBIS // ===================================================================================================

    SceKernelModule dlhandle = -1;
    const char*     argv[1] = {m_pModule->getLibraryFullName().data()};
    if ((dlhandle = sceKernelLoadStartModule(m_pModule->getLibraryFullName().data(), 1, argv, 0, NULL, NULL)) >= 0)
    {
        detail::pushInstallation();
        detail::installModules();
        detail::popInstallation();
        PHANTOM_LOG(Information, "Module loaded : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(m_strName));
        PHANTOM_ASSERT(m_pModule == nullptr);
        m_pModule = Application::Get()->getModule(m_strName);
        if (m_pModule == nullptr)
        {
            if (g_LoadingPlugins.size() == 1)
                Application::Get()->_pluginLoadingFailed(this);
            PHANTOM_LOG(Error,
                        "'PHANTOM_PLUGIN(\"...\");' declaration for the current loaded module does "
                        "not match the plugin module file name '%s'",
                        m_strName.c_str());
            PHANTOM_VERIFY(sceKernelStopUnloadModule(dlhandle, 0, nullptr, 0, NULL, NULL) > 0);
            return false;
        }
        return true;
    }
    else
    {
        const char* msg;
        switch (dlhandle)
        {
        case SCE_KERNEL_ERROR_EINVAL:
            // 0x80020016
            msg = "flags or pOpt is invalid";
            break;

        case SCE_KERNEL_ERROR_ESTART:
            // 0x80020064
            msg = "module_start() returned a negative integer";
            break;

        case SCE_KERNEL_ERROR_ENOENT:
            // 0x80020002
            msg = "File specified in moduleFileName does not exist";
            break;

        case SCE_KERNEL_ERROR_ENOEXEC:
            // 0x80020008
            msg = "Cannot load because of abnormal file format";
            break;

        case SCE_KERNEL_ERROR_ENOMEM:
            // 0x8002000c
            msg = "Cannot load because it is not possible to allocate memory";
            break;

        case SCE_KERNEL_ERROR_EACCES:
            // 0x8002000d
            msg = "File specified with moduleFileName is placed in a forbidden location";
            break;

        case SCE_KERNEL_ERROR_EFAULT:
            // 0x8002000e
            msg = "moduleFileName points to invalid memory";
            break;

        case SCE_KERNEL_ERROR_EAGAIN:
            // 0x80020023
            msg = "Cannot load because of insufficient resources";
            break;

        case SCE_KERNEL_ERROR_ESDKVERSION:
            // 0x80020063
            msg = "Version of the SDK used to build the specified dynamic library is newer "
                  "than the system software version";
            break;

        default:
            msg = "unknown error";
            break;
        }
        PHANTOM_LOG(Error, "Cannot load module : %s", m_strName.c_str());
        PHANTOM_LOG(Error, "System DLL loading failed : %s", msg);
        return false;
    }

#elif PHANTOM_OPERATING_SYSTEM_FAMILY ==                                                                               \
PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX // ===================================================================================================
    void* dlhandle = nullptr;
    if ((dlhandle = dlopen(String(a_strPath).c_str(), RTLD_NOW)))
    {
        detail::pushInstallation();
        detail::installModules();
        detail::popInstallation();
        PHANTOM_LOG(Information, "Module loaded : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(m_strName));
        PHANTOM_ASSERT(m_pModule == nullptr);
        m_pModule = Application::Get()->getModule(m_strName);
        if (m_pModule == nullptr)
        {
            if (g_LoadingPlugins.size() == 1)
                Application::Get()->_pluginLoadingFailed(this);
            PHANTOM_LOG(Error,
                        "'PHANTOM_PLUGIN(\"...\");' declaration for the current loaded module does "
                        "not match the plugin module file name '%.*s'",
                        PHANTOM_STRING_AS_PRINTF_ARG(m_strName));
            dlclose(dlhandle);
            return false;
        }
        return true;
    }
    else
    {
        PHANTOM_LOG(Error, "Cannot load module : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(m_strName));
        PHANTOM_LOG(Error, "System DLL loading failed : %s", dlerror());
        return false;
    }
#else
#    if !defined(PHANTOM_STATIC_LIB_HANDLE)
    PHANTOM_LOG(Error, "the current platform does not support dynamic loading of shared libraries");
    return false;
#    endif
#endif
}

bool Plugin::_unloadNative()
{
#if defined(PHANTOM_STATIC_LIB_HANDLE)
    PHANTOM_LOG(Error, "cannot use native plugins in static link mode");
    return false;
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    if (FreeLibrary((HMODULE)m_pModule->getHandle()))
    {
        m_pModule = nullptr;
        PHANTOM_LOG(Information, "Module unloaded : %s", m_strName.c_str());
        return true;
    }
    else
    {
        DWORD dw = GetLastError();
        PHANTOM_LOG(Error, "Cannot unload module : %s", m_strName.c_str());
        LPVOID lpMsgBuf;

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                      dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

        String clampedMessage = (char*)lpMsgBuf;
        clampedMessage = clampedMessage.substr(0, clampedMessage.find_first_of("\r\n"));

        PHANTOM_LOG(Error, "System dynamic library unloading failed : %s", clampedMessage.c_str());

        LocalFree(lpMsgBuf);
        return false;
    }

#elif PHANTOM_OPERATING_SYSTEM ==                                                                                      \
PHANTOM_OPERATING_SYSTEM_ORBIS // ===================================================================================================

    SceKernelModule dlhandle = -1;
    if (sceKernelStopUnloadModule((SceKernelModule)m_pModule->getHandle(), 0, NULL, 0, NULL, NULL) >= 0)
    {
        m_pModule = nullptr;
        PHANTOM_LOG(Information, "Module unloaded : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(m_strName));
        return true;
    }
    else
    {
        const char* msg;
        switch (dlhandle)
        {
        case SCE_KERNEL_ERROR_EINVAL:
            // 0x80020016
            msg = "flags or pOpt is invalid";
            break;

        case SCE_KERNEL_ERROR_ESRCH:
            // 0x80020003
            msg = "handle is invalid(specified dynamic library is not loaded)";
            break;

        case SCE_KERNEL_ERROR_EBUSY:
            // 0x80020010
            msg = "Specified dynamic library is referenced by a thread other than the thread "
                  "that called this function";
            break;

        default:
            msg = "unknown error";
            break;
        }
        PHANTOM_LOG(Error, "Cannot unload module : %s", m_strName.c_str());
        PHANTOM_LOG(Error, "System DLL unloading failed : %s", msg);
        return false;
    }

#elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
    if (dlclose((void*)m_pModule->getHandle()))
    {
        PHANTOM_LOG(Information, "Module unloaded : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(m_pModule->getName()));
        return true;
    }
    else
    {
        PHANTOM_LOG(Error, "Cannot unload module : %.*s", PHANTOM_STRING_AS_PRINTF_ARG(m_pModule->getName()));
        PHANTOM_LOG(Error, "System dynamic library unloading failed : %s", dlerror());
        return false;
    }
#else
#    if !defined(PHANTOM_STATIC_LIB_HANDLE)
    PHANTOM_LOG(Error, "the current platform does not support dynamic loading of shared libraries");
    return false;
#    endif
#endif
}

bool Plugin::_loadFile()
{
    FILE*   file = fopen(m_strFilePath.c_str(), "r");
    char    linebuf[256];
    Strings lines;
    if (!file)
    {
        PHANTOM_LOG(Error, "failed to read plugin file");
        fclose(file);
        return false;
    }
    char* end = fgets(linebuf, 256, file);
    end += strlen(end);
    if (!end)
    {
        PHANTOM_LOG(Error, "failed to parse plugin file");
        fclose(file);
        return false;
    }
    while (*--end == '\n' || *end == '\r')
        *end = 0;
    if (strcmp(linebuf, "[Plugin]") != 0)
    {
        PHANTOM_LOG(Error, "failed to parse plugin file");
        fclose(file);
        return false;
    }
    while (feof(file) == 0 && (end = fgets(linebuf, 256, file)))
    {
        end += strlen(end);
        if (!end)
        {
            PHANTOM_LOG(Error, "failed to parse plugin file");
            fclose(file);
            return false;
        }
        while (*--end == '\n' || *end == '\r')
            *end = 0;
        String line(linebuf);
        if (StringUtil::RemoveExtraBlanks(line).empty())
            continue;
        StringViews parts;
        StringUtil::Split(parts, line, "=");
        if (parts.size() != 2)
        {
            PHANTOM_LOG(Error, "failed to parse plugin file");
            fclose(file);
            return false;
        }
        StringView key = StringUtil::RemoveExtraBlanks(parts[0]);
        StringView value = StringUtil::RemoveExtraBlanks(parts[1]);
        if (key == "Name")
            m_strName = value;
        else if (key == "DebugPath")
            m_DebugPath = value;
        else if (key == "ReleasePath")
            m_ReleasePath = value;
        else
        {
            PHANTOM_LOG(Warning, "%.*s : unknown entry '%.*s'", PHANTOM_STRING_AS_PRINTF_ARG(m_strFilePath),
                        PHANTOM_STRING_AS_PRINTF_ARG(value));
        }
    }
    fclose(file);
    return true;
}

bool Plugin::_refCountNative()
{
#if !defined(PHANTOM_STATIC_LIB_HANDLE)
    /// Increment native handlle reference count
#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    HMODULE hmodule = 0;
    if ((hmodule = LoadLibraryA(String(m_pModule->getLibraryFullName()).c_str())) == 0)
        return false;
#    elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS
    SceKernelModule dlhandle = -1;
    const char*     argv[1] = {m_pModule->getLibraryFullName().data()};
    if ((dlhandle = sceKernelLoadStartModule(String(m_pModule->getLibraryFullName()).c_str(), 1, argv, 0, NULL, NULL)) <
        0)
        return false;
#    elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
    void* dlhandle = nullptr;
    if ((dlhandle = dlopen(String(m_pModule->getLibraryFullName()).c_str(), RTLD_NOW)) == 0)
        return false;
#    endif
#else
    return false;
#endif
    return true;
}

bool Plugin::load()
{
    if (_load())
    {
        Application::Get()->_pluginLoaded(this);
        return true;
    }
    else
    {
        Application::Get()->_pluginLoadingFailed(this);
        return false;
    }
}

bool Plugin::loadAsDependency()
{
    return _load();
}

bool Plugin::unloadAsDependency()
{
    return _unload();
}

bool Plugin::_load()
{
    PHANTOM_ASSERT(Application::Get()->getPlugin(m_strName) == this,
                   "Plugin should be added to Application before being (un)loaded");
    Function* pLoadingExtensionFunction = nullptr;

    g_LoadingPlugins.push_back(this);
    LoadingPluginsGuard guard__(&g_LoadingPlugins);

    PHANTOM_ASSERT(m_RefCount == 0 || m_pModule);

    /// If module already loaded  we just increment ref counts
    if (m_RefCount > 0)
    {
        if (m_pModule->isNative())
            _refCountNative();
        m_RefCount++;
        return true;
    }

    OperationCounterGuard __guard_counter(&Application::Get()->m_OperationCounter);

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    char exe_path[MAX_PATH];
    GetModuleFileNameA(0, exe_path, MAX_PATH);
    Path exePath = Path(exe_path).parentPath();
#else
    Path exePath = "/dummy.exe";
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    define DYN_LIB_EXT ".dll"
#elif PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_MACOSX ||                                                   \
PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_IOS
#    define DYN_LIB_EXT ".dylib"
#elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
#    define DYN_LIB_EXT ".so"
#endif

#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
    auto dllRelPath = m_DebugPath;
#else
    auto dllRelPath = m_ReleasePath;
#endif

    if (dllRelPath.size())
    {
        Path nativeBinaryPath = exePath.childPath(dllRelPath + DYN_LIB_EXT);
        auto prev = Path::GetCurrentWorkingDir();
        Path::SetCurrentWorkingDir(Path(m_strFilePath).parentPath());
        if (!Path::Exists(nativeBinaryPath))
        {
            PHANTOM_LOG(Error, "dynamic library path '%s' does not exist", nativeBinaryPath.genericString().c_str());
            Path::SetCurrentWorkingDir(prev);
            return false;
        }
        nativeBinaryPath = nativeBinaryPath.absolute().canonical();
        Path::SetCurrentWorkingDir(prev);

        String runtimePluginPath;
        String pluginFilePath;

#if defined(PHANTOM_STATIC_LIB_HANDLE)
        PHANTOM_LOG(Error, "cannot use native plugins in static link mode");
        return false;
#endif
        if (!_loadNative(nativeBinaryPath.genericString()))
        {
            return false;
        }
    }

    if (pLoadingExtensionFunction)
    {
        void* this_ = this;
        void* args[2] = {&this_};
        bool  result = false;
        pLoadingExtensionFunction->call(args, &result);
        if (!result)
        {
            return false;
        }
    }

    if (!m_pModule)
        return false;

    PHANTOM_ASSERT(m_pModule->m_pPlugin == nullptr);
    m_pModule->m_pPlugin = this;

    m_RefCount++;
    return true;
}

bool Plugin::unload()
{
    PHANTOM_ASSERT(Application::Get()->getPlugin(m_strName) == this,
                   "Plugin should be added to Application before being (un)loaded");
    // PHANTOM_ASSERT_ON_MAIN_THREAD();
    LoadingPluginsGuard __guard(&g_UnloadingPlugins);
    g_UnloadingPlugins.push_back(this);
    if (m_RefCount == 1)
    {
        Application::Get()->_pluginAboutToBeUnloaded(this);
    }
    PHANTOM_ASSERT(m_pModule->m_pPlugin == this);
    m_pModule->m_pPlugin = nullptr;
    return _unload();
}

bool Plugin::_unload()
{
    Function* pUnloadingExtensionFunction = nullptr;

    PHANTOM_ASSERT(m_RefCount > 0 && m_pModule);
    m_RefCount--;
    if (m_RefCount > 0)
    {
        if (m_pModule->isNative())
            _unrefCountNative();
        return true;
    }

    // TODO : add signal declaration
    //             bool approved = true;
    //             PHANTOM_EMIT pluginUnloadApprovalRequested(pModule, approved, a_pMessage);
    //             if (!approved)
    //             {
    //                 return false;
    //             }

    String runtimePluginPath;

    OperationCounterGuard __guard_counter(&Application::Get()->m_OperationCounter);
    if (pUnloadingExtensionFunction)
    {
        void* this_ = this;
        void* args[2] = {&this_};
        bool  result = false;
        pUnloadingExtensionFunction->call(args, &result);
        if (!result)
        {
            return false;
        }
    }
    if (m_pModule->isNative())
    {
        String strPath = m_pModule->getLibraryFullName();
        if (!_unloadNative())
            return false;
    }
    return true;
}

bool Plugin::_unrefCountNative()
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    if (FreeLibrary((HMODULE)m_pModule->getHandle()) == FALSE)
    {
        return false;
    }
#elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX &&                                       \
PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
    if (dlclose((void*)m_pModule->getHandle()) == 0)
    {
        return false;
    }
#else
    return false;
#endif
    return true;
}

} // namespace lang
} // namespace phantom
