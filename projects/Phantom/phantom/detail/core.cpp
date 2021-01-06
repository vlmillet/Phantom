// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "core.h"

#include <iostream>
#include <phantom/lang/SourceFile.h>
#include <phantom/lang/registration/Main.h>
#include <phantom/plugin>
#include <phantom/utils/StringUtil.h>
#include <sstream>

#ifndef __DOXYGEN__

#    include <phantom/utils/Path.h>
#    include <stdarg.h>
#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#        include <psapi.h>
#        include <windows.h>
#        include <winuser.h>
#    endif
#endif
/* *********************************************** */

#include <phantom/lang/Application.h>
#include <phantom/lang/Module.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/Source.h>
#include <phantom/utils/Console.h>
#if PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include <thread>
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_MACOSX
#    include <dlfcn.h>
#    include <mach-o/dyld.h>
#    include <mach-o/getsect.h>
void macosx_moduleAdded(const struct mach_header* mh, intptr_t vmaddr_slide)
{
    int i = 0;
    ++i;
}
#endif

#include "StaticGlobals.h"
#include "core_internal.h"

#include <phantom/alignof>
#include <phantom/lang/Plugin.h>
#include <phantom/traits/IntTypeBySize.h>
#include <phantom/utils/SmallMultimap.h>
#include <phantom/utils/SmallSet.h>
#include <unordered_map>

#if TYPE_REGISTRATION_KEY_DEBUG_ENABLED
#    include <map>
#endif
#include "StaticGlobalRegistrer.h"

#include <phantom/detail/ClassOfFwd.h>
#include <phantom/lang/reflection.h>
#include <phantom/lang/registration/GlobalRegistrer.h>
#include <phantom/lang/registration/Main.h>
#include <phantom/traits/FunctionTypeToMethodPointerType.h>
#include <phantom/utils/crc64.h>

#define PHANTOM_REFLECTION_DEBUG_ENABLED 0

#if PHANTOM_REFLECTION_DEBUG_ENABLED
#    define PHANTOM_LOG_NATIVE_REFLECTION(...) PHANTOM_LOG(Error, ##__VA_ARGS__)
#else
#    define PHANTOM_LOG_NATIVE_REFLECTION(...)
#endif

PHANTOM_STATIC_ASSERT(PHANTOM_PP_ARGCOUNT() == 0);
PHANTOM_STATIC_ASSERT(PHANTOM_PP_ARGCOUNT(0) == 1);
PHANTOM_STATIC_ASSERT(PHANTOM_PP_ARGCOUNT(0, 1) == 2);

namespace phantom
{
namespace lang
{
namespace detail
{
void InitGlobals();
void ReleaseGlobals();
} // namespace detail
} // namespace lang

PHANTOM_STATIC_ASSERT(PHANTOM_SIZE_OF_LONGLONG == sizeof(long long));

namespace
{
void trimMsgFileName(StringView file)
{
    size_t last = file.find_last_of('/');
    if (last == StringView::npos)
    {
        last = file.find_last_of('\\');
        if (last != StringView::npos)
        {
            file = file.substr(last + 1);
        }
    }
}

#if defined(PHANTOM_STATIC_LIB_HANDLE)
#    define common_output(title, color)                                                                                \
        trimMsgFileName(file);                                                                                         \
        std::ostringstream out;                                                                                        \
        out << console::push << console::fg_##color << title << console::fg_white << "(" << file << "|"                \
            << console::fg_white << line << ") " << expression << (expression[0] ? " " : "");                          \
        out.write(text.data(), text.size());                                                                           \
        out << console::pop << std::endl;                                                                              \
        printf("%s", out.str().c_str());

#else
#    define common_output(title, color)                                                                                \
        trimMsgFileName(file);                                                                                         \
        std::cout << console::push << console::fg_##color << title << console::fg_white << "(" << file << "|"          \
                  << console::fg_white << line << ") " << expression << (expression[0] ? " " : "");                    \
        std::cout.write(text.data(), text.size());                                                                     \
        std::cout << console::pop << std::endl;
#endif

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

void AssertCombineHash(uint64_t& a_rSeed, uint64_t a_Value)
{
    a_rSeed ^= a_Value + 0x9e3779b99e3779b9 + (a_rSeed << 6) + (a_rSeed >> 2); // inspired from boost
}

// on windows asserts are, by default, message box with YES | NO | CANCEL

bool defaultAssert(StringView expression, StringView file, int line, StringView msg)
{
    static SpinMutex          assertIgnoreCacheMtx;
    static SmallSet<uint64_t> assertIgnoreCache;

    uint64_t hash = 0;
    AssertCombineHash(hash, StringHash(expression).get());
    AssertCombineHash(hash, line);
    AssertCombineHash(hash, StringHash(file).get());

    {
        auto lock = assertIgnoreCacheMtx.autoLock();
        if (assertIgnoreCache.find(hash) != assertIgnoreCache.end())
            return false;
    }
    String finalMsg = "ASSERT(" + expression + ")\n\n" + msg + "\n\n" + "File:" + file + "\n\n" +
    "Line:" + std::to_string(line).c_str() + "\n\nPress Yes to break, No to ignore once, Cancel to ignore always.";

    int choice = MessageBoxA(0, finalMsg.c_str(), "ASSERT", MB_YESNOCANCEL | MB_ICONEXCLAMATION);

    switch (choice)
    {
    default:
    case IDYES:
        return true;
    case IDCANCEL:
    {
        auto lock = assertIgnoreCacheMtx.autoLock();
        assertIgnoreCache.insert(hash);
    }
    case IDNO:
        return false;
    }
}

#else

bool defaultAssert(StringView expression, StringView file, int line, StringView text)
{
    common_output("ASSERT", magenta);
    return true;
}

#endif

bool defaultWarning(StringView expression, StringView file, int line, StringView text)
{
    common_output("WARNING", yellow);
    return true;
}

bool defaultError(StringView expression, StringView file, int line, StringView text)
{
    common_output("ERROR", red);
    return true;
}

void defaultLog(MessageType, StringView file, int line, StringView text)
{
    StringView expression = "";
    common_output("LOG", green);
}

} // namespace

namespace detail
{
RawPlacement<std::mutex> g_PHNTM_slot_pool_mutex;

template<class T>
void staticAllocatedDeleteEx(RawPlacement<T>& a_Placement)
{
}

static lang::Main* g_instance;
static StringView  g_pSource;
static StringView  g_pPackage;
#if PHANTOM_REFLECTION_DEBUG_ENABLED
static lang::LanguageElements* g_elements;
#endif
static MessageReportFunc g_assert_func{defaultAssert};
static MessageReportFunc g_warning_func{defaultWarning};
static MessageReportFunc g_error_func{defaultError};
static LogFunc           g_LogFunc{defaultLog};
lang::ClassHookFunc      g_InstanceHook_func;

typedef SmallMap<String, lang::Package*> PackageMap;

typedef SmallVector<void*, 4096> ToFree;

static RawPlacement<Strings> g_pMetaDataNames;

static std::thread::id g_MainThreadId = std::this_thread::get_id();

static void*         g_typeOf_cycling_address_workaround_ptr;
static lang::Module* g_module;

RawPlacement<lang::Application> g_pApplication;

PHANTOM_EXPORT_PHANTOM lang::Symbol* symbolRegisteredAt(size_t a_ModuleHandle, StringView a_File, int a_Line, int a_Tag)
{
    _PHNTM_StaticGlobalRegistrer* pReg =
    dynamic_initializer_()->moduleRegistrationInfo(a_ModuleHandle)->findRegistrer(a_File, a_Line, a_Tag);
    return pReg ? static_cast<lang::_PHNTM_GlobalRegistrer*>(pReg)->_PHNTM_getLastSymbol() : nullptr;
}

void DynamicCppInitializerH::StaticGlobalsInit()
{
    g_pApplication.construct();

    g_pApplication->rtti.instance = &*g_pApplication;
    detail::deferInstallation("phantom::lang::Application", &g_pApplication->rtti);
    g_pApplication->initialize();

    lang::initializeSystem();
}

void DynamicCppInitializerH::StaticGlobalsRelease()
{
    lang::releaseSystem();

    PHANTOM_CLASSOF(lang::Application)->unregisterInstance(g_pApplication);

    g_pApplication->_terminate();
    g_pApplication.destroy();
}

DynamicCppInitializerH::DynamicCppInitializerH()
    : m_iActive(0), m_bAutoRegistrationLocked(false), m_bPhantomInstalled(false)
{
}

void DynamicCppInitializerH::init()
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_MACOSX
    _dyld_register_func_for_add_image(macosx_moduleAdded);
#endif

    g_instance = nullptr;

    g_module = nullptr;

    g_pSource.clear();
    g_pPackage.clear();

    g_PHNTM_slot_pool_mutex.construct();
    g_pMetaDataNames.construct();

    g_MainThreadId = std::this_thread::get_id();

    g_typeOf_cycling_address_workaround_ptr = &g_typeOf_cycling_address_workaround_ptr;

    lang::detail::InitGlobals();
}

void DynamicCppInitializerH::release()
{
    lang::detail::ReleaseGlobals();
    g_PHNTM_slot_pool_mutex.destroy();
    g_pMetaDataNames.destroy();
}

void DynamicCppInitializerH::registerModule(size_t a_ModuleHandle, StringView a_strName, StringView a_strBinaryFileName,
                                            StringView a_strSource, uint a_uiFlags,
                                            std::initializer_list<StringView> a_Dependencies, void (*onLoad)(),
                                            void (*onUnload)())
{
    pushInstallation();
    moduleRegistrationInfo(a_ModuleHandle)->m_Name = a_strName;
    moduleRegistrationInfo(a_ModuleHandle)->m_BinaryFileName = a_strBinaryFileName;
    moduleRegistrationInfo(a_ModuleHandle)->m_Source = a_strSource;
    moduleRegistrationInfo(a_ModuleHandle)->m_uiFlags = a_uiFlags;
    moduleRegistrationInfo(a_ModuleHandle)->m_OnLoad = onLoad;
    moduleRegistrationInfo(a_ModuleHandle)->m_OnUnload = onUnload;
    moduleRegistrationInfo(a_ModuleHandle)->m_Dependencies = SmallVector<StringView, 64>(a_Dependencies);
    popInstallation();
}

void DynamicCppInitializerH::registerType(size_t a_ModuleHandle, hash64 a_Hash, phantom::lang::Type* a_pType)
{
    _PHNTM_R_MTX_GUARD();
    registerTypeHash(a_ModuleHandle, a_Hash, a_pType);
    lang::Namespace::Global()->addType(a_pType);
}

void DynamicCppInitializerH::registerTypeHash(size_t a_ModuleHandle, hash64 a_Hash, phantom::lang::Type* a_pType)
{
    _PHNTM_R_MTX_GUARD();
    PHANTOM_ASSERT(moduleRegistrationInfo(a_ModuleHandle)->registeredTypeByHash(a_Hash) == nullptr,
                   "type already registered in same module, shouldn't happen, test is in TypeOf to avoid that");
    PHANTOM_ASSERT(a_pType->getOwner() && a_pType->getVisibility() == lang::Visibility::Public);
    moduleRegistrationInfo(a_ModuleHandle)->registerTypeByHash(a_Hash, a_pType);
}

void DynamicCppInitializerH::registerType(size_t a_ModuleHandle, hash64 a_Hash, StringView a_ScopeName,
                                          phantom::lang::Type* a_pType)
{
    _PHNTM_R_MTX_GUARD();
    bool isTemplateInstance = a_pType->testFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM);
    registerTypeHash(a_ModuleHandle, a_Hash, a_pType);
    if (!isTemplateInstance)
    {
        phantom::lang::Symbol* pNamingScope =
        a_ScopeName.empty() ? lang::Namespace::Global() : lang::Application::Get()->findCppSymbol(a_ScopeName);
        PHANTOM_ASSERT(pNamingScope, "scope '%.*s' has not been registered => check your type's nesting class",
                       PHANTOM_STRING_AS_PRINTF_ARG(a_ScopeName));
        PHANTOM_ASSERT(pNamingScope->asScope(), "'%.*s' is not a valid C++ scope (class, namespace, etc...)",
                       PHANTOM_STRING_AS_PRINTF_ARG(a_ScopeName));
        /// Only add type if it's not a template instance
        pNamingScope->asScope()->addType(a_pType);
    }
}

void DynamicCppInitializerH::stepTypeInstallation(lang::Type* a_pType)
{
    lang::Module* pModule = a_pType->getModule();
    if (pModule == nullptr)
        return;
    moduleRegistrationInfo(pModule->getHandle())->stepTypeInstallation(a_pType);
}

void DynamicCppInitializerH::stepTemplateInstanceInstallation(size_t a_ModuleHandle, lang::Type* a_pType)
{
    moduleRegistrationInfo(a_ModuleHandle)->stepTypeInstallation(a_pType);
}

lang::Namespace* DynamicCppInitializerH::parseNamespace(StringView a_strNamespace) const
{
    return lang::Namespace::Global()->getOrCreateNamespace(a_strNamespace);
}

void DynamicCppInitializerH::registerTemplateInstance(size_t a_ModuleHandle, lang::TypeInstallationInfo* a_pTii)
{
    _PHNTM_R_MTX_GUARD();
    if (lang::detail::currentModule())
    {
        /// auto registration by template argument use or explicit instanciation => fallback to
        /// classical registerTypeInstallationInfo
        registerTypeInstallationInfo(a_pTii);
    }
    else
    {
        /// auto registration by TypeOf<>::object() in user defined functions
        lang::Module* pModule = lang::Application::Get()->getModuleByHandle(a_ModuleHandle);
        PHANTOM_ASSERT(pModule,
                       "No module installed which match the given module handle, ensure "
                       "'PHANTOM_PLUGIN(...)' or 'PHANTOM_MAIN(...)' has been used correctly");
        lang::detail::pushModule(pModule);
        lang::detail::pushSource(pModule->getAnonymousSource());
        registerTypeInstallationInfo(a_pTii);
        lang::detail::popSource();
        lang::detail::popModule();
    }
}

void DynamicCppInitializerH::registerTypeInstallationInfo(lang::TypeInstallationInfo* a_pTypeInstallInfo)
{
    if (a_pTypeInstallInfo->installFunc == nullptr)
        return;

    _PHNTM_R_MTX_GUARD();

    PHANTOM_ASSERT(lang::detail::currentModule());

    if (a_pTypeInstallInfo->type->getOwner() == nullptr &&
        !(a_pTypeInstallInfo->type->testFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM)))
    {
        /// not a template instance and not a nested type => we add it to the current source
        /// or the current module anonymous source if no current source is defined
        PHANTOM_ASSERT(a_pTypeInstallInfo->m_pSource);
        a_pTypeInstallInfo->m_pSource->addType(a_pTypeInstallInfo->type);
    }

    lang::ModuleRegistrationInfo* info = moduleRegistrationInfo(lang::detail::currentModule()->getHandle());

    PHANTOM_ASSERT(info);

    bool alreadyRegistered = false;
#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
    for (auto pTii : info->m_TypeInstallationInfos)
    {
        if (a_pTypeInstallInfo->installFunc == pTii->installFunc || pTii->type == a_pTypeInstallInfo->type)
        {
            PHANTOM_ASSERT(a_pTypeInstallInfo->type->testFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM),
                           "ensure your %.*s.hxx is not included at multiple places without "
                           "<haunt-include-optimization> include guards",
                           int(a_pTypeInstallInfo->type->getName().size()), a_pTypeInstallInfo->type->getName().data());
            alreadyRegistered = true;
            break;
        }
    }
#else
    if (a_pTypeInstallInfo->type->testFlags(PHANTOM_R_FLAG_TEMPLATE_ELEM))
    {
        for (auto pTii : info->m_TypeInstallationInfos)
        {
            if (a_pTypeInstallInfo->installFunc == pTii->installFunc || pTii->type == a_pTypeInstallInfo->type)
            {
                alreadyRegistered = true;
                break;
            }
        }
    }
#endif
    if (!(alreadyRegistered))
    {
        info->addTypeInstallationInfos(a_pTypeInstallInfo);
    }
}

void DynamicCppInitializerH::unregisterModule(size_t a_ModuleHandle)
{
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        if (it->m_ModuleHandle == a_ModuleHandle)
        {
            m_ModuleRegistrationInfos.erase(it);
            if (lang::Application::Get() && lang::Application::Get()->getMainModule())
            {
                if (lang::Plugin::HasUnloadingInProgress())
                {
                    lang::Application::Get()->_uninstallNativeModule(
                    lang::Application::Get()->getModuleByHandle(a_ModuleHandle));
                }
            }
            return;
        }
    }
    PHANTOM_UNREACHABLE();
}

void DynamicCppInitializerH::stepRegistration(RegistrationStep step)
{
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        if (it->m_bInstalled)
            continue;
        lang::detail::pushModule(it->m_pModule);
        it->stepRegistration(step);
        lang::detail::popModule();
    }
}

void DynamicCppInitializerH::stepTypeInstallation(TypeInstallationStep step)
{
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        if (it->m_bInstalled)
            continue;
        lang::detail::pushModule(it->m_pModule);
        it->installTypes(step);
        lang::detail::popModule();
    }
}

size_t DynamicCppInitializerH::computeModuleRegistrationInfoLevel(lang::ModuleRegistrationInfo const& m)
{
    if (m.m_Dependencies.empty())
        return 0;
    size_t maxLevel = 0;
    for (auto dep : m.m_Dependencies)
    {
        for (auto& mri : m_ModuleRegistrationInfos)
        {
            if (mri.m_Name == dep)
            {
                size_t lev = computeModuleRegistrationInfoLevel(mri);
                if (lev > maxLevel)
                {
                    maxLevel = lev;
                }
                break;
            }
        }
    }
    return maxLevel + 1;
}

void DynamicCppInitializerH::installModules()
{
    PHANTOM_LOG_NATIVE_REFLECTION("installing modules...");
    PHANTOM_LOG_NATIVE_REFLECTION("initialize fundamental types...");
    PHANTOM_LOG_NATIVE_REFLECTION("fundamental types initialized...");
    bool autoRegistrationLocked = m_bAutoRegistrationLocked;
    if (autoRegistrationLocked)
        setAutoRegistrationLocked(false);
    size_t modulesToInstallCount = 0;

    SmallVector<lang::ModuleRegistrationInfo*> infos;
    for (auto& info : m_ModuleRegistrationInfos)
    {
        infos.push_back(&info);
    }

    std::sort(infos.begin(), infos.end(),
              [this](lang::ModuleRegistrationInfo* m0, lang::ModuleRegistrationInfo* m1) -> bool {
                  return computeModuleRegistrationInfoLevel(*m0) < computeModuleRegistrationInfoLevel(*m1);
              });

    if (!m_bPhantomInstalled)
    {
        auto phantomModuleIt = std::find_if(infos.begin(), infos.end(), [](lang::ModuleRegistrationInfo* info) -> bool {
            return info->m_Name == "Phantom";
        });
        PHANTOM_ASSERT(phantomModuleIt != infos.end());
        if (phantomModuleIt != infos.begin())
        {
            std::swap(*infos.begin(), *phantomModuleIt);
        }
    }

    for (auto it : infos)
    {
        if (it->m_bInstalled)
            continue;
        PHANTOM_LOG_NATIVE_REFLECTION("create module '%s'...", it->m_Name.c_str());
        if (it->m_Name.empty())
        {
#define PHANTOM_PLUGIN_exception_message_base "missing 'PHANTOM_PLUGIN(\"plugin_name\")' declaration in your plugin"

#if defined(PHANTOM_STATIC_LIB_HANDLE)
#    if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#        define PHANTOM_PLUGIN_exception_message                                                                       \
            PHANTOM_PLUGIN_exception_message_base                                                                      \
            ", or you forgot to enable LinkLibraryDependencies + UseLibraryDependencyInputs in "                       \
            "Visual Studio"
#    else
#        define PHANTOM_PLUGIN_exception_message                                                                       \
            PHANTOM_PLUGIN_exception_message_base ", or you forgot to add '-fall_load' to your clang compiler options"
#    endif
            PHANTOM_ERROR(false, PHANTOM_PLUGIN_exception_message);
#else

#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
            char buffer[256];
            GetModuleBaseNameA(GetCurrentProcess(), (HMODULE)it->m_ModuleHandle, buffer, 256);
            PHANTOM_ERROR(false, PHANTOM_PLUGIN_exception_message_base " '%s'", buffer);
#    else
            PHANTOM_ERROR(false, PHANTOM_PLUGIN_exception_message_base);
#    endif
#endif
        }
        lang::Application::Get()->_createNativeModule(&*it);
        modulesToInstallCount++;
    }

    if (!m_bPhantomInstalled)
        lang::Application::Get()->_registerBuiltInTypes();

#if !defined(PHANTOM_STATIC_LIB_HANDLE)
    PHANTOM_ASSERT(lang::Plugin::HasLoadingInProgress() || lang::Application::Get()->getMainModule() == nullptr ||
                   modulesToInstallCount == 1);

    // no module to install
    if (modulesToInstallCount == 0)
        return;
#endif

    PHANTOM_LOG_NATIVE_REFLECTION("modules installation...");

    /// BEGIN MODULES INSTALLATION

    /// Install BuiltInTypes and Templates
    stepRegistration(RegistrationStep::_Reserved);

    stepRegistration(RegistrationStep::Start);

    PHANTOM_LOG_NATIVE_REFLECTION("namespaces installation...");
    stepRegistration(RegistrationStep::Namespaces);

    PHANTOM_LOG_NATIVE_REFLECTION("enums installation...");
    stepRegistration(RegistrationStep::Enums);

    PHANTOM_LOG_NATIVE_REFLECTION("templates installation...");
    stepRegistration(RegistrationStep::Templates);

    PHANTOM_LOG_NATIVE_REFLECTION("class types installation...");
    stepRegistration(RegistrationStep::ClassTypes);

    stepRegistration(RegistrationStep::PostClassTypes);

    /// Install global typedefs

    PHANTOM_LOG_NATIVE_REFLECTION("typedefs installation...");
    stepRegistration(RegistrationStep::Typedefs);

    PHANTOM_LOG_NATIVE_REFLECTION("post typedefs installation...");
    stepRegistration(RegistrationStep::PostTypedefs);

    /// Install then template signatures before any other elements

    PHANTOM_LOG_NATIVE_REFLECTION("template signatures installation...");
    stepTypeInstallation(TypeInstallationStep::TemplateSignature);

    PHANTOM_LOG_NATIVE_REFLECTION("post template signatures installation...");
    stepRegistration(RegistrationStep::TemplateSignatures);

    /// Install class members, inheritance ...

    /// After inheritance setup we consider phantom installed (every base class is set and
    /// installation/initialization can occur safely)
    PHANTOM_LOG_NATIVE_REFLECTION("template default arguments installation...");
    stepTypeInstallation(TypeInstallationStep::TemplateDefaultArgs);
    PHANTOM_LOG_NATIVE_REFLECTION("inheritance installation...");
    stepTypeInstallation(TypeInstallationStep::Inheritance);
    PHANTOM_LOG_NATIVE_REFLECTION("class members installation...");
    stepTypeInstallation(TypeInstallationStep::Members);
    PHANTOM_LOG_NATIVE_REFLECTION("statecharts installation...");
    stepTypeInstallation(TypeInstallationStep::Installed);

    m_bPhantomInstalled = true; // at this point we are able to use phantom core features

    PHANTOM_LOG_NATIVE_REFLECTION("post type installation...");
    stepRegistration(RegistrationStep::PostTypes);

    /// Install global functions, variables...
    PHANTOM_LOG_NATIVE_REFLECTION("variable installation...");
    stepRegistration(RegistrationStep::Variables);
    stepRegistration(RegistrationStep::PostVariables);
    PHANTOM_LOG_NATIVE_REFLECTION("functions installation...");
    stepRegistration(RegistrationStep::Functions);

    stepRegistration(RegistrationStep::End);

    PHANTOM_LOG_NATIVE_REFLECTION("end modules installation...");

    PHANTOM_LOG_NATIVE_REFLECTION("meta instances installation...");

    m_DeferredInstallationsMutex.lock();

    for (auto it = m_DeferredInstallations.begin(); it != m_DeferredInstallations.end(); ++it)
    {
        if (it->rtti) // if not rtti, means cancelled
        {
            // TODO : remove ?
            lang::Class* pClass = lang::Application::Get()->findCppClass(it->typeName);
            PHANTOM_ASSERT(pClass);
            // Force dynamic delete function to the proxy one if required
            it->rtti->metaClass = pClass;
            pClass->registerInstance(it->rtti->instance);
        }
    }
    m_DeferredInstallations.clear();

    m_DeferredInstallationsMutex.unlock();

    if (autoRegistrationLocked)
        setAutoRegistrationLocked(autoRegistrationLocked);

    PHANTOM_LOG_NATIVE_REFLECTION("check modules completeness and auto load meta data...");
    for (lang::ModuleRegistrationInfo* it : infos)
    {
        if (it->m_bInstalled)
            continue;
        /// END OF INSTALLATION
        it->m_pModule->checkCompleteness();

        for (auto dep : it->m_Dependencies)
        {
            lang::Module* pDep = lang::Application::Get()->getModule(dep);
            PHANTOM_ASSERT(pDep, "module '%s' is a dependency of module '%s' and has not been loaded", dep, it->m_Name);
            it->m_pModule->addDependency(pDep);
        }

        it->m_bInstalled = true;
        if (it->m_pModule->getOnLoadFunc())
            it->m_pModule->getOnLoadFunc()();
        lang::Application::Get()->_moduleAdded(it->m_pModule);
    }
}

lang::ModuleRegistrationInfo* DynamicCppInitializerH::getModuleRegistrationInfo(StringView name)
{
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        if (it->m_Name == name)
            return &*it;
    }
    return nullptr;
}

lang::ModuleRegistrationInfo* DynamicCppInitializerH::getModuleRegistrationInfo(size_t a_ModuleHandle)
{
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        if (it->m_ModuleHandle == a_ModuleHandle)
            return &*it;
    }
    return nullptr;
}

lang::ModuleRegistrationInfo* DynamicCppInitializerH::moduleRegistrationInfo(size_t a_ModuleHandle)
{
    lang::ModuleRegistrationInfo* info = nullptr;
    size_t                        uninstalledModuleCount = 0;
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        if (it->m_ModuleHandle == a_ModuleHandle)
        {
            info = &*it;
            break;
        }
        uninstalledModuleCount += !it->m_bInstalled;
    }
    if (info == nullptr)
    {
        m_ModuleRegistrationInfos.emplace_back(a_ModuleHandle);
        info = &m_ModuleRegistrationInfos.back();
    }
    return info;
}

lang::Module* DynamicCppInitializerH::findSourceInModules(StringView a_strFilePath, Strings& a_Words)
{
    lang::Module* pBestModule = nullptr;
    size_t        bestScore = 0;
    for (auto it = m_ModuleRegistrationInfos.begin(); it != m_ModuleRegistrationInfos.end(); ++it)
    {
        Strings words;
        size_t  score;
        if ((score = findSourceInModule(a_strFilePath, words, it->m_pModule)))
        {
            if (score > bestScore)
            {
                a_Words = words;
                bestScore = score;
                pBestModule = it->m_pModule;
            }
            else
            {
                PHANTOM_ASSERT(score != bestScore, "two modules are candidate to same source in static linking mode");
            }
        }
    }
    return pBestModule;
}

size_t DynamicCppInitializerH::findSourceInModule(StringView a_strFilePath, Strings& words, lang::Module* a_pModule)
{
    PHANTOM_ASSERT(a_pModule);
    PHANTOM_ASSERT(a_strFilePath.size());

    Path sourceFilePath = a_strFilePath;
    sourceFilePath.removeExtension();
    Path moduleFilePath = a_pModule->getDeclarationCppFullName();
    moduleFilePath = moduleFilePath.parentPath(); // parent path of .cpp file containing the PHANTOM_PLUGIN decl
    Path moduleNamePath(a_pModule->getName(), '.');

    if (moduleFilePath.hasChildPath(sourceFilePath))
    {
        Path sourceRelativePath = sourceFilePath.relative(moduleFilePath);

        if (moduleNamePath.hasChildPath(sourceRelativePath))
        {
            /// the source path contains in its path the module path name (ex:
            /// phantom/lang/jit/jit contains phantom/lang/jit =>
            /// phantom.lang.jit)
            words.insert(words.end(), sourceRelativePath.begin(), sourceRelativePath.end());
            return 2;
        }
        else
        {
#if PHANTOM_REFLECTION_DEBUG_ENABLED
            PHANTOM_WARNING(false,
                            "source relative path '%s' to module '%s' does not contain equivalent "
                            "module name path '%s'",
                            sourceRelativePath.genericString().c_str(), a_pModule->getName().c_str(),
                            moduleNamePath.genericString().c_str());
#endif
            words.insert(words.end(), sourceRelativePath.begin(), sourceRelativePath.end());
            return 1;
        }
    }
    else
    {
        size_t i = sourceFilePath.rfind(a_pModule->getName());
        if (i == Path::npos)
        {
            return 0;
        }
        auto sourceRelativePath = sourceFilePath.subPath(i + 1);
        words.insert(words.end(), sourceRelativePath.begin(), sourceRelativePath.end());
        return 1;
    }
}

phantom::lang::Source* DynamicCppInitializerH::nativeSource(StringView a_strFile, StringView a_strPackage,
                                                            StringView a_strSource)
{
    lang::Module* pModule = lang::detail::currentModule();
    PHANTOM_ASSERT(pModule);

    lang::ModuleRegistrationInfo* info = getModuleRegistrationInfo(pModule->getHandle());
    PHANTOM_ASSERT(info);

    Strings words;

    if (a_strSource.size())
    {
        if (a_strPackage.size())
        {
            words.clear();
            StringUtil::Split(words, a_strPackage, ".");
            StringViews sourceWords;
            StringUtil::Split(sourceWords, a_strSource, ".");
            words.insert(words.end(), sourceWords.begin(), sourceWords.end());
        }
        else
        {
            StringUtil::Split(words, a_strSource, ".");
        }
    }
    else
    {
        PHANTOM_ASSERT(a_strPackage.empty(),
                       "missing PHANTOM_SOURCE(\"...\") inside PHANTOM_PACKAGE(\"%.*s\") scope in file '%.*s'",
                       PHANTOM_STRING_AS_PRINTF_ARG(a_strPackage), PHANTOM_STRING_AS_PRINTF_ARG(a_strFile));
    }

    lang::Package* pPackage = pModule->getDefaultPackage();
    lang::Source*  pSource = pModule->getAnonymousSource();
    if (words.size())
    {
        String sourceName = words.back();
        String packageName;

        if (words.size() > 1) // package present in the name
        {
            words.pop_back();
            for (auto it = words.begin(); it != words.end(); ++it)
            {
                if (it != words.begin())
                    packageName += '.';
                packageName += *it;
            }
            pPackage = pModule->getOrCreatePackage(packageName);
        }

        PHANTOM_ASSERT(pPackage);

        pSource = pPackage->getOrCreateSource(sourceName);
        if (pSource->getSourceStream() == nullptr)
            pSource->setSourceStream(pSource->new_<lang::SourceFile>(a_strFile));
        pSource->addFlags(PHANTOM_R_FLAG_NATIVE);
    }
    return pSource;
}

PHANTOM_EXPORT_PHANTOM void deferInstallation(StringView a_strTypeName, RTTI* a_pRtti)
{
    dynamic_initializer_()->deferInstallation(a_strTypeName, a_pRtti);
}

PHANTOM_EXPORT_PHANTOM bool installed()
{
    return dynamic_initializer_()->installed();
}

} // namespace detail

void CppTypeIdToPhantomQualifiedDecoratedName(char* a_Name, size_t& a_Length)
{
    // TODO optimize
    String a_typeid_name(a_Name, a_Length);
    if (a_typeid_name.find("class ") == 0)
        a_typeid_name = a_typeid_name.substr(6);
    else if (a_typeid_name.find("enum ") == 0)
        a_typeid_name = a_typeid_name.substr(5);
    else if (a_typeid_name.find("struct ") == 0)
        a_typeid_name = a_typeid_name.substr(7);
    else if (a_typeid_name.find("union ") == 0)
        a_typeid_name = a_typeid_name.substr(6);

    StringUtil::ReplaceAll(a_typeid_name, "(class ", "(");
    StringUtil::ReplaceAll(a_typeid_name, "(enum ", "(");
    StringUtil::ReplaceAll(a_typeid_name, "(struct ", "(");
    StringUtil::ReplaceAll(a_typeid_name, "(union ", "(");
    StringUtil::ReplaceAll(a_typeid_name, "<class ", "<");
    StringUtil::ReplaceAll(a_typeid_name, "<enum ", "<");
    StringUtil::ReplaceAll(a_typeid_name, "<struct ", "<");
    StringUtil::ReplaceAll(a_typeid_name, "<union ", "<");
    StringUtil::ReplaceAll(a_typeid_name, ",class ", ",");
    StringUtil::ReplaceAll(a_typeid_name, ",enum ", ",");
    StringUtil::ReplaceAll(a_typeid_name, ",struct ", ",");
    StringUtil::ReplaceAll(a_typeid_name, ",union ", ",");
#if (PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO)
#    if (PHANTOM_ARCHITECTURE == PHANTOM_ARCHITECTURE_X86_64)
    StringUtil::ReplaceAll(a_typeid_name, " __ptr32", "");
    StringUtil::ReplaceAll(a_typeid_name, " __ptr64", "");
#    endif
    StringUtil::ReplaceAll(a_typeid_name, " __cdecl", "");
    StringUtil::ReplaceAll(a_typeid_name, " __stdcall", "");
    StringUtil::ReplaceAll(a_typeid_name, " __fastcall", "");
#endif
    PHANTOM_ASSERT(a_typeid_name.size() <= a_Length);
    a_Length = a_typeid_name.size();
    memcpy(a_Name, a_typeid_name.c_str(), a_Length + 1);
}

void QualifiedDecoratedNameToTypeInfos(char* a_Name, size_t a_Length, lang::TypeInfos& a_type_infos)
{
    a_type_infos.buffer = a_Name;
    char* end = a_Name + a_Length;
    int   templateLevel = 0;
    char* decoration = end;
    char* name = a_Name;
    for (char* it = a_Name; it != end; ++it)
    {
        switch (*it)
        {
        case '<':
        {
            if (templateLevel == 0)
                decoration = it;
            templateLevel++;
            break;
        }
        case '>':
        {
            templateLevel--;
            break;
        }
        case ':':
        {
            // cancel decoration pos if any
            if (templateLevel == 0)
            {
                name = it + 1; // define name beginning at ':'+1
                decoration = end;
            }
            break;
        }
        default:
            break;
        }
    }
    // decoration found
    a_type_infos.decoration_length = uint16(end - decoration);
    a_type_infos.name_length = uint16(decoration - name);
    a_type_infos.scope_length = uint16(name - a_Name);
    a_type_infos.hash =
    phantom::crc64(0, (unsigned char*)a_type_infos.buffer,
                   a_type_infos.scope_length + a_type_infos.name_length + a_type_infos.decoration_length);
}

void CppTypeIdToPhantomQualifiedDecoratedNamePtr(char* a_Name, size_t& a_Length)
{
    CppTypeIdToPhantomQualifiedDecoratedName(a_Name, a_Length);
    a_Length--;
    if (a_Name[a_Length] == '4') // means __ptr64
        a_Length -= sizeof(" __ptr64") - 1;
    PHANTOM_ASSERT(a_Name[a_Length] == '*');
    a_Name[a_Length] = '\0'; // remove '*'
    while (a_Name[a_Length - 1] == ' ')
    {
        a_Length--;
        a_Name[a_Length] = '\0'; // remove '*'
    }
}

PHANTOM_EXPORT_PHANTOM StringView lang::detail::PrettyFunctionToTypeName(StringView a_TypeName)
{
    size_t start = a_TypeName.find_first_of('<');
    if (start != StringView::npos)
    {
        PHANTOM_ASSERT(start != StringView::npos);
        ++start;
        a_TypeName = a_TypeName.substr(start);
        size_t end = a_TypeName.find_last_of('>');
        PHANTOM_ASSERT(end != StringView::npos);
        a_TypeName = a_TypeName.substr(0, end);
    }
    return a_TypeName;
}

PHANTOM_EXPORT_PHANTOM void lang::detail::BuildTypeInfos(StringView a_TypeName, TypeInfos& a_TI)
{
    a_TypeName = PrettyFunctionToTypeName(a_TypeName);
    char* demangled = (char*)PHANTOM_MALLOC(a_TypeName.size() + 1);
    demangled[a_TypeName.size()] = '\0';
    memcpy(demangled, a_TypeName.data(), a_TypeName.size());
    size_t len = a_TypeName.size();
    CppTypeIdToPhantomQualifiedDecoratedName(demangled, len);
    QualifiedDecoratedNameToTypeInfos(demangled, len, a_TI);
    phantom::detail::registerOrphanMemory(demangled);
}

PHANTOM_EXPORT_PHANTOM size_t _dllModuleHandleFromAddress(void const* address)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    if defined(PHANTOM_STATIC_LIB_HANDLE)
    MEMORY_BASIC_INFORMATION mbi;
    ;
    ::VirtualQuery(address, &mbi, sizeof(mbi));
    return (size_t)(mbi.AllocationBase);
#    else
    HMODULE module = 0;
    PHANTOM_VERIFY(GetModuleHandleExA(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)address, &module));
    return (size_t)module;
#    endif
#elif PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
    Dl_info info;
    dladdr((const void*)address, &info);
    PHANTOM_ASSERT(info.dli_fbase);
    return (size_t)info.dli_fbase;
#else
    return 0;
#endif
}

namespace detail
{
PHANTOM_EXPORT_PHANTOM lang::Module* mainModule()
{
    if (lang::Application::Get())
        return lang::Application::Get()->getMainModule();
    return nullptr;
}

PHANTOM_EXPORT_PHANTOM void registerOrphanMemory(void* a_pMem)
{
    lang::Module* pModule = phantom::lang::detail::currentModule();
    if (!pModule)
        pModule = phantom::detail::mainModule();
    if (pModule)
    {
        pModule->getMemoryContext().registerMemory(a_pMem);
        return;
    }
    PHANTOM_ASSERT(lang::Application::Get(), "no memory context is ready yet to receive this orphan memory");
    lang::Application::Get()->getMemoryContext().registerMemory(a_pMem);
}

} // namespace detail

detail::DynamicCppInitializerH* dynamic_initializer_()
{
    static RawPlacement<detail::DynamicCppInitializerH> s_Singleton;
    if (!s_Singleton)
    {
        s_Singleton.construct();
        s_Singleton->init();
    }
    return &*s_Singleton;
}

#define common_valist_decode(str, sz, format, args)                                                                    \
    trimMsgFileName(file);                                                                                             \
    char str[sz];                                                                                                      \
    str[sz] = '\0';                                                                                                    \
    vsnprintf(str, sz - 1, format, args);

PHANTOM_EXPORT_PHANTOM bool assertion PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* file, int line,
                                                                         const char* format, ...)
{
    va_list args;
    va_start(args, format);
    common_valist_decode(buffer, 512, format, args);
    bool r = detail::g_assert_func && detail::g_assert_func(e, file, line, buffer);
    va_end(args);
    return r;
}

PHANTOM_EXPORT_PHANTOM bool warning PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* file, int line,
                                                                       const char* format, ...)
{
    va_list args;
    va_start(args, format);
    common_valist_decode(buffer, 512, format, args);
    bool r = detail::g_warning_func && detail::g_warning_func(e, file, line, buffer);
    va_end(args);
    return r;
}

PHANTOM_EXPORT_PHANTOM bool error PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* file, int line,
                                                                     const char* format, ...)
{
    va_list args;
    va_start(args, format);
    common_valist_decode(buffer, 512, format, args);
    bool r = detail::g_error_func && detail::g_error_func(e, file, line, buffer);
    va_end(args);
    return r;
}

PHANTOM_EXPORT_PHANTOM void log PHANTOM_PREVENT_MACRO_SUBSTITUTION(MessageType msgType, const char* file, int line,
                                                                   const char* format, ...)
{
    va_list args;
    va_start(args, format);
    String str;
    common_valist_decode(buffer, 512, format, args);
    if (detail::g_LogFunc)
        detail::g_LogFunc(msgType, file, line, buffer);
    va_end(args);
}

PHANTOM_EXPORT_PHANTOM void logv PHANTOM_PREVENT_MACRO_SUBSTITUTION(MessageType msgType, const char* file, int line,
                                                                    const char* format, va_list args)
{
    String str;
    common_valist_decode(buffer, 512, format, args);
    if (detail::g_LogFunc)
        detail::g_LogFunc(msgType, file, line, buffer);
}

namespace
{
phantom::lang::Main* g_PHNTM_Main;
}

phantom::lang::Main* phantom::lang::Main::Get()
{
    return g_PHNTM_Main;
}

void lang::Main::setAssertFunc(MessageReportFunc a_func)
{
    phantom::detail::g_assert_func = a_func;
}

void lang::Main::setErrorFunc(MessageReportFunc a_func)
{
    phantom::detail::g_error_func = a_func;
}

void lang::Main::setLogFunc(LogFunc const& a_func)
{
    phantom::detail::g_LogFunc = a_func;
}

phantom::MessageReportFunc const& phantom::lang::Main::getAssertFunc(MessageReportFunc a_func) const
{
    return phantom::detail::g_assert_func;
}

phantom::MessageReportFunc const& phantom::lang::Main::getErrorFunc(MessageReportFunc a_func) const
{
    return phantom::detail::g_error_func;
}

phantom::LogFunc const& phantom::lang::Main::getLogFunc() const
{
    return phantom::detail::g_LogFunc;
}

phantom::MessageReportFunc const& phantom::lang::Main::getWarningFunc() const
{
    return phantom::detail::g_warning_func;
}

void lang::Main::setWarningFunc(MessageReportFunc a_func)
{
    phantom::detail::g_warning_func = a_func;
}

lang::Main::Main(size_t a_ModuleHandle, StringView a_strMainModuleName, int argc, char** argv,
                 CustomAllocator _allocator, ClassHookFunc a_ClassHookFunc, StringView a_strFile, uint a_uiFlags)
{
    g_PHNTM_Main = this;
    // PHANTOM_ASSERT_ON_MAIN_THREAD();

    phantom::detail::g_InstanceHook_func = a_ClassHookFunc;

    CustomAllocator::Push(_allocator);

    phantom::detail::DynamicCppInitializerH::StaticGlobalsInit();

    PHANTOM_LOG_NATIVE_REFLECTION("entering PHANTOM_MAIN...");
    PHANTOM_ASSERT(phantom::detail::g_instance == nullptr, "only one Main instance allowed");
    phantom::detail::g_instance = this;

    PHANTOM_LOG_NATIVE_REFLECTION("setting default paths...");
    for (int i = 1; i < argc; ++i)
    {
        if (strncmp(argv[i], "-M", 2) == 0)
        {
            // module path
            Application::Get()->addPluginPath(argv[i] + 2);
        }
        else if (strncmp(argv[i], "-E", 2) == 0)
        {
            // export path
            Application::Get()->setDefaultExportPath(argv[i] + 2);
        }
        else if (strncmp(argv[i], "-I", 2) == 0)
        {
            // source path
            Application::Get()->addSourcePath(argv[i] + 2);
        }
    }

    Path base_exe_path;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    char exe_path[MAX_PATH];
    GetModuleFileNameA(0, exe_path, MAX_PATH);
    base_exe_path = Path(exe_path).parentPath();
#else
    base_exe_path = Path(argv[0]).parentPath();
#endif
    auto pApp = lang::Application::Get();
    if (pApp->getDefaultBinaryPath().empty())
        pApp->setDefaultBinaryPath(base_exe_path.genericString());

    PHANTOM_LOG_NATIVE_REFLECTION("loading main lang...");
    if (argv)
    {
        Application::Get()->_loadMain(a_ModuleHandle, a_strMainModuleName, argv[0], a_strFile, a_uiFlags);
    }
    else
    {
        char buffer[1024] = "";
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
        GetModuleFileNameA(0, buffer, 1024);
#endif
        lang::Application::Get()->_loadMain(a_ModuleHandle, a_strMainModuleName, buffer, a_strFile, a_uiFlags);
    }
}

lang::Main::~Main()
{
    PHANTOM_ASSERT(g_PHNTM_Main == this);
    // PHANTOM_ASSERT_ON_MAIN_THREAD();
    Application::Get()->_unloadMain();
    Application::Get()->m_OperationCounter++; /// to allow auto dll unloading after main(...) ends
    phantom::detail::DynamicCppInitializerH::StaticGlobalsRelease();
    dynamic_initializer_()->release(); // ensure every allocation made by the dynamic initializer are cleaned up here

    CustomAllocator::Pop();
    g_PHNTM_Main = nullptr;
}

PHANTOM_EXPORT_PHANTOM uint64_t makeStringHash(StringView a_Str)
{
    uint64_t result(crc64(0, (const unsigned char*)a_Str.data(), a_Str.size()));
    return result;
}

PHANTOM_EXPORT_PHANTOM bool isMainThread()
{
    return detail::g_MainThreadId == std::this_thread::get_id();
}
} // namespace phantom

PHANTOM_PLUGIN("Phantom", PHANTOM_PLUGIN_REGISTER_CLASS_MEMBERS_ON_ACCESS)
