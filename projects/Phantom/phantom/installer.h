// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;

#if PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_MODE

#    include <phantom/RttiImpl.h>
#    include <phantom/reflection/Class.h>
#    include <phantom/traits/HasEmbeddedRtti.h>

namespace phantom
{
/// @cond INTERNAL

/* installation */

enum DefautInstallerID
{
    DefaultInstallerNone,
    DefaultInstallerEmbeddedRtti,
    DefaultInstallerCustomInstallation,
    DefaultInstallerMappedRtti,
};

namespace detail
{
template<typename t_Ty>
struct DefaultInstallerH<t_Ty, DefaultInstallerMappedRtti>
{
public:
    PHANTOM_FORCEINLINE static void install(reflection::Class* a_pClass, t_Ty* a_pInstance)
    {
        a_pClass->mapRtti(a_pInstance);
    }
    PHANTOM_FORCEINLINE static void uninstall(reflection::Class* a_pClass, t_Ty* a_pInstance)
    {
        a_pClass->unmapRtti(a_pInstance);
    }
};

template<typename t_Ty>
struct DefaultInstallerH<t_Ty, DefaultInstallerNone>
{
public:
    inline static void install(reflection::Type*, t_Ty*)
    {
    }
    inline static void uninstall(reflection::Type*, t_Ty*)
    {
    }
};

template<typename t_Ty>
struct EmbeddedRttiInstaller
{
    static void install(reflection::Class* a_pClass, t_Ty* a_pInstance)
    {
        a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance = a_pInstance;
        a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass = a_pClass;
    }
    static void uninstall(reflection::Class* a_pClass, t_Ty* a_pInstance)
    {
        PHANTOM_ASSERT(a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass,
                       "instance already uninstalled");
        PHANTOM_ASSERT(a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance == a_pInstance,
                       "instance does not match the current uninstalling one");
        PHANTOM_ASSERT(a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass == a_pClass,
                       "instance class does not match the current uninstalling class");
        a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.instance = nullptr;
        a_pInstance->PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD.metaClass = nullptr;
    }
};

template<typename t_Ty>
struct DefaultInstallerH<t_Ty, DefaultInstallerEmbeddedRtti> : detail::EmbeddedRttiInstaller<t_Ty>
{
};

} // namespace detail

template<typename t_Ty>
struct DefaultInstaller
    : public detail::DefaultInstallerH<
      t_Ty,

      (std::is_fundamental<t_Ty>::value OR phantom::IsNullptrT<t_Ty>::value OR(
      std::is_pod<t_Ty>::value AND NOT(std::is_class<t_Ty>::value)) OR IsStructure<t_Ty>::value)
      ? DefaultInstallerNone
      : std::is_class<t_Ty>::value
      ? (HasEmbeddedRtti<t_Ty>::value OR HasEmbeddedProxyRtti<t_Ty>::value)
      ? DefaultInstallerEmbeddedRtti
      : DefaultInstallerMappedRtti
      : DefaultInstallerNone

      >
{
    PHANTOM_REBIND(DefaultInstaller)
};

/// @endcond

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct Installer
///
/// \brief  Installer. (see instance creation process)
/// 		Installation is applied just after construction
/// 		Uninstallation is applied just before destruction
///
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename t_Ty>
struct Installer : public DefaultInstaller<t_Ty>
{
};

} // namespace phantom

#endif // PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_MODE
