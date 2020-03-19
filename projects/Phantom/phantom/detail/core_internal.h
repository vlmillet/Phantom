// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "core.h"

HAUNT_STOP;

#include "ModuleRegistrationInfo.h"
#include "new.h"

#include <phantom/lang/Class.h>
#include <phantom/lang/Package.h>
#include <phantom/lang/TypeInfos.h>
#include <phantom/thread/RecursiveMutex.h>
#include <phantom/typeof>
#include <phantom/utils/StringViews.h>
#include <phantom/utils/crc64.h>

namespace phantom
{
struct RTTI;

HAUNT_PAUSE;

PHANTOM_EXPORT_PHANTOM class detail::DynamicCppInitializerH* dynamic_initializer_();

namespace detail
{
class PHANTOM_EXPORT_PHANTOM DynamicCppInitializerH
{
public:
    static void StaticGlobalsInit();
    static void StaticGlobalsRelease();

    DynamicCppInitializerH();
    void                            init();
    void                            release();
    phantom::lang::Namespace* parseNamespace(StringView a_strNamespace) const;

    struct DeferredInstallationInfo
    {
        StringView typeName;
        RTTI*      rtti;
    };

    void stepRegistration(RegistrationStep step);
    void stepTypeInstallation(TypeInstallationStep step);

    SmallVector<lang::ModuleRegistrationInfo, 64> m_ModuleRegistrationInfos;
    RecursiveMutex                                      m_DeferredInstallationsMutex;
    SmallVector<DeferredInstallationInfo>               m_DeferredInstallations;
    int                                                 m_iActive;
    StringView                                          m_source;
    StringView                                          m_package;
    bool                                                m_bAutoRegistrationLocked;
    bool                                                m_bPhantomInstalled;

    inline lang::Type* registeredTypeByHash(size_t a_ModuleHandle, hash64 a_Hash)
    {
        return moduleRegistrationInfo(a_ModuleHandle)->registeredTypeByHash(a_Hash);
    }
    lang::Package* package(lang::Module* a_pModule, StringView a_strName, bool* a_pNew = nullptr);
    size_t               findSourceInModule(StringView a_strFilePath, Strings& words, lang::Module* a_pModule);
    lang::Module*  findSourceInModules(StringView a_strFilePath, Strings& words);
    lang::Source*  nativeSource(StringView a_strFile, StringView a_strPackage, StringView a_strSource);
    void                 pushSource(StringView a_strSource)
    {
        PHANTOM_ASSERT(m_source.empty());
        m_source = a_strSource;
        PHANTOM_ASSERT_DEBUG(lang::Package::IsValidName(a_strSource),
                             "invalid source name, only [a-z0-9_] characters (lower case) separated with "
                             "'.' are allowed");
        PHANTOM_ASSERT_DEBUG(m_package.size() ||(a_strSource.find_first_of('.') != StringView::npos),
                             "missing package name in source qualified name (or enclose your "
                             "PHANTOM_R_SOURCE/PHANTOM_R_END scope in a PHANTOM_R_PACKAGE/PHANTOM_R_END scope)");
    }
    void       popSource() { m_source.clear(); }
    StringView currentSource() const { return m_source; }
    void       pushPackage(StringView a_strPackage)
    {
        PHANTOM_ASSERT(m_package.empty());
        m_package = a_strPackage;
        PHANTOM_ASSERT(lang::Package::IsValidName(m_package),
                       "invalid package name, only [a-z0-9_] (lower case) characters separated "
                       "with '.' are allowed");
    }
    void       popPackage() { m_package.clear(); }
    StringView currentPackage() const { return m_package; }
    void       registerModule(size_t a_ModuleHandle, StringView a_strName, StringView a_strBinaryFileName,
                              StringView a_strSource, uint a_uiFlags, std::initializer_list<StringView> a_Dependencies,
                              void (*onLoad)(), void (*onUnload)());
    void       registerTypeHash(size_t a_ModuleHandle, hash64 a_Hash, lang::Type* a_pType);
    void       registerType(size_t a_ModuleHandle, hash64 a_Hash, StringView a_ScopeName, lang::Type* a_pType);
    void       registerTypeInstallationInfo(lang::TypeInstallationInfo* a_pTypeInstallInfo);
    void       registerType(size_t a_ModuleHandle, hash64 a_Hash, lang::Type* a_pType);
    void       stepTypeInstallation(lang::Type* a_pType);
    void       stepTemplateInstanceInstallation(size_t a_ModuleHandle, lang::Type* a_pType);
    void       registerTemplateInstance(size_t a_ModuleHandle, lang::TypeInstallationInfo* a_pTii);
    void       unregisterModule(size_t a_ModuleHandle);
    bool       isActive() const { return m_iActive != 0; }
    bool       isAutoRegistrationLocked() const { return m_bAutoRegistrationLocked; }
    void       pushInstallation() { m_iActive++; }
    void       popInstallation()
    {
        PHANTOM_ASSERT(m_iActive);
        m_iActive--;
    }
    void                                installModules();
    lang::ModuleRegistrationInfo* getModuleRegistrationInfo(StringView name);
    lang::ModuleRegistrationInfo* getModuleRegistrationInfo(size_t a_ModuleHandle);
    lang::ModuleRegistrationInfo* moduleRegistrationInfo(size_t a_ModuleHandle);
    void                                setAutoRegistrationLocked(bool a_bLocked)
    {
        PHANTOM_ASSERT(a_bLocked == !m_bAutoRegistrationLocked);
        m_bAutoRegistrationLocked = a_bLocked;
    }

    hash64 makeRegistrerKey(StringView file, int line, int tag,
                            bool
#if TYPE_REGISTRATION_KEY_DEBUG_ENABLED
                            erase
#endif

    )
    {
        PHANTOM_ASSERT(file.size());
        hash64 h64 = crc64((uint64_t(line) << 32) | uint64_t(tag), (const unsigned char*)file.data(), file.size());
#if TYPE_REGISTRATION_KEY_DEBUG_ENABLED
        {
            CustomAllocator::Push(CustomAllocator::Default());
            struct ___TRK
            {
                StringView file;
                int        line;
                int        tag;
            };
            static std::map<hash64, ___TRK>* m_debug_TRK = new std::map<hash64, ___TRK>;
            static bool                      registeredForCleanup = false;

            PHANTOM_ASSERT(erase || m_debug_TRK);
            if (m_debug_TRK == nullptr)
                return h64;

            if (!registeredForCleanup)
            {
                StaticGlobals::RegisterForCleanup(m_debug_TRK, CleanupDelegate([](void* p) {
                                                      CustomAllocator::Push(CustomAllocator::Default());
                                                      delete reinterpret_cast<std::map<hash64, ___TRK>*>(p);
                                                      m_debug_TRK = nullptr;
                                                      CustomAllocator::Pop();
                                                  }));
                registeredForCleanup = true;
            }
            auto it = m_debug_TRK->find(h64);
            if (it == m_debug_TRK->end())
                (*m_debug_TRK)[h64] = ___TRK{file, line, int(tag)};
            else
                PHANTOM_ASSERT(strcmp(it->second.file, file) == 0 && it->second.line == line &&
                               it->second.tag == int(tag));
            if (erase)
                m_debug_TRK->erase(h64);
            CustomAllocator::Pop();
        }
#endif
        return h64;
    }

    bool installed() const { return m_bPhantomInstalled; }
    void deferInstallation(StringView a_strTypeName, RTTI* a_pRtti)
    {
        PHANTOM_ASSERT(a_pRtti->instance);
        m_DeferredInstallationsMutex.lock();
        m_DeferredInstallations.resize(m_DeferredInstallations.size() + 1);
        m_DeferredInstallations.back().typeName = a_strTypeName;
        m_DeferredInstallations.back().rtti = a_pRtti;
        m_DeferredInstallationsMutex.unlock();
    }

    size_t computeModuleRegistrationInfoLevel(lang::ModuleRegistrationInfo const& m);
};

PHANTOM_EXPORT_PHANTOM void deferInstallation(StringView a_strTypeName, RTTI* a_pInstance);
struct DeferredNewH
{
    template<class T>
    inline T* operator*(T* a_pInstance)
    {
        PHANTOM_STATIC_ASSERT(std::is_class<T>::value);
        a_pInstance->rtti.instance = a_pInstance;
        if (dynamic_initializer_()->installed())
        {
            PHANTOM_VERIFY(a_pInstance->rtti.metaClass = static_cast<lang::Class*>(PHANTOM_TYPEOF(T)));
            a_pInstance->rtti.metaClass->registerInstance(a_pInstance);
        }
        else
        {
            deferInstallation(lang::TypeInfosOf<T>::object().qualifiedDecoratedName(), &a_pInstance->rtti);
        }
        return a_pInstance;
    }
};
template<class T>
struct DeferredDeleteH
{
    inline void operator*(T* a_pInstance)
    {
        if (dynamic_initializer_()->installed())
        {
            PHANTOM_DELETE(T) a_pInstance;
        }
        else
        {
            a_pInstance->rtti.instance = nullptr;
            a_pInstance->rtti.metaClass = nullptr;
            a_pInstance->terminate();
            a_pInstance->~T();
        }
    }
};

} // namespace detail

HAUNT_RESUME;
} // namespace phantom

#define TYPE_REGISTRATION_KEY_DEBUG_ENABLED (PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL)

#define PHANTOM_DEFERRED_NEW(...) phantom::detail::DeferredNewH() * new (phantom::allocate(sizeof(__VA_ARGS__), PHANTOM_ALIGNOF(__VA_ARGS__))) __VA_ARGS__
#define PHANTOM_DEFERRED_PLACEMENT_NEW(instance) phantom::detail::DeferredNewH() * instance

#define PHANTOM_DEFERRED_DELETE(...) phantom::detail::DeferredDeleteH<__VA_ARGS__>()*
