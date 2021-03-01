// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

// ********************************************************************* //
// ******************************************************************* ///
// **************************** PREREQUISITES ************************** //
// ******************************************************************* ///
// ********************************************************************* //

#ifndef __DOXYGEN__

#    define _CRT_SECURE_NO_WARNINGS 1

/* ************** PHANTOM CONFIGURATION *********** */
#    include <phantom/customize.h>
#    include <phantom/detail/config.h>
/* ************* UNIVERSAL PREDEFINITION ********** */
#    ifdef _UNICODE
#        ifndef UNICODE
#            define UNICODE
#        endif
#    elif defined(UNICODE)
#        define _UNICODE
#    endif

/* ********************** STD ********************* */

#    if (PHANTOM_COMPILER == PHANTOM_COMPILER_GCC)
#        include <inttypes.h>
#        include <stdarg.h>
#        include <stdlib.h>
#    endif

/* ********************* BOOST ******************** */
#    define BOOST_SYSTEM_NO_DEPRECATED
#    if !defined(Q_MOC_RUN)
#    endif // Q_MOC_RUN

/* ******************** PHANTOM ******************* */
#    include <phantom/detail/macros.h>
#    include <phantom/detail/memory.h>
#    include <phantom/detail/typedefs.h>
#    include <phantom/fwd.h>
#    include <phantom/plugin.h>

namespace phantom
{
PHANTOM_EXPORT_PHANTOM size_t _dllModuleHandleFromAddress(void const*);

#    if defined(PHANTOM_STATIC_LIB_HANDLE)
#        define PHANTOM_MODULE_HANDLE(...) PHANTOM_STATIC_LIB_HANDLE
#    else
#        define PHANTOM_MODULE_HANDLE(...) ::phantom::_dllModuleHandleFromAddress((void const*)(__VA_ARGS__))
#    endif

namespace
{
/// @off
PHANTOM_FORCEINLINE size_t currentModuleHandle()
{
#    if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
    return PHANTOM_MODULE_HANDLE(currentModuleHandle);
#    else
    return PHANTOM_MODULE_HANDLE((void*)&currentModuleHandle);
#    endif
}
} // namespace

} // namespace phantom

#else // DOXYGEN

#    include <phantom/customize.h>
#    include <phantom/detail/macros.h>
#    include <phantom/plugin.h>

#endif // DOXYGEN

#include <phantom/traits/TypeIdentity.h>

namespace phantom
{
enum class RegistrationStep
{
    _None,
    _Reserved,
    Start,
    Namespaces,
    Enums,
    Templates,
    ClassTypes,
    PostClassTypes,
    Typedefs,
    PostTypedefs,
    TemplateSignatures,
    PostTypes,
    Variables,
    PostVariables,
    Functions,
    End
};

using RegistrationSteps = std::initializer_list<RegistrationStep>;

enum class TypeInstallationStep
{
    Uninstalled,
    TemplateSignature,
    TemplateDefaultArgs,
    Inheritance,
    Members,
    Installed,
};

using TypeInstallationDelegate = Delegate<void(TypeInstallationStep)>;

} // namespace phantom

namespace phantom
{
enum class MessageType
{
    Undefined,
    Information,
    Success,
    Warning,
    Error,
};

PHANTOM_EXPORT_PHANTOM bool assertion PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* f, int l,
                                                                         const char* format, ...);
PHANTOM_EXPORT_PHANTOM bool warning   PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* f, int l,
                                                                         const char* format, ...);
PHANTOM_EXPORT_PHANTOM bool error     PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* f, int l,
                                                                         const char* format, ...);
PHANTOM_EXPORT_PHANTOM void log       PHANTOM_PREVENT_MACRO_SUBSTITUTION(MessageType level, const char* file, int line,
                                                                         const char* format, ...);

PHANTOM_EXPORT_PHANTOM bool isMainThread();

HAUNT_STOP;

template<class T, class... Args>
T* new_(Args&&... a_Args)
{
    PHANTOM_STATIC_ASSERT(!(std::is_same<lang::LanguageElement, T>::value));
    return new (allocate(sizeof(T), PHANTOM_ALIGNOF(T))) T(std::forward<Args>(a_Args)...);
}

template<class T, class... Args>
T* pnew_(T* a_pObj, Args&&... a_Args)
{
    PHANTOM_STATIC_ASSERT(!(std::is_same<lang::LanguageElement, T>::value));
    return new (a_pObj) T(std::forward<Args>(a_Args)...);
}

template<class T>
void delete_(TypeIndentityT<T*> a_pPtr)
{
    PHANTOM_STATIC_ASSERT(!(std::is_same<lang::LanguageElement, T>::value));
    a_pPtr->~T();
    deallocate(a_pPtr);
}

template<class T>
void deleteVirtual(T* a_pPtr)
{
    PHANTOM_STATIC_ASSERT(!(std::is_same<lang::LanguageElement, T>::value));
    PHANTOM_STATIC_ASSERT(std::has_virtual_destructor<T>::value);
    a_pPtr->~T();
    deallocate(a_pPtr);
}

} // namespace phantom
