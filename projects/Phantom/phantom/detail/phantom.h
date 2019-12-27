// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
#    include <phantom/detail/fwd.h>
#    include <phantom/detail/macros.h>
#    include <phantom/detail/typedefs.h>
#    include <phantom/memory/memory.h>
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

PHANTOM_EXPORT_PHANTOM void conversionOperatorNameNormalizer(StringView a_strName, StringBuffer& a_Buf,
                                                             reflection::LanguageElement* a_pScope);

namespace reflection
{
}

} // namespace phantom

#else // DOXYGEN

#    include <phantom/customize.h>
#    include <phantom/detail/macros.h>
#    include <phantom/plugin.h>

#endif // DOXYGEN

namespace phantom
{
#ifndef __DOXYGEN__

namespace detail
{
template<class t_Ty>
struct TypeRegistrer;
template<class t_Target, class t_Source, bool t_SourceHasEmbeddedRtti>
struct asH;
} // namespace detail

#endif // DOXYGEN

} // namespace phantom

#ifndef __DOXYGEN__
#    include "phantom/memory/memory.inl"
#endif // DOXYGEN

namespace phantom
{
enum class RegistrationStep
{
    _None,
    _Reserved,
    Start,
    Namespaces,
    Enums,
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

typedef bool (*MessageReportFunc)(StringView expression, StringView file, int line, const char* format,
                                  va_list arglist);
typedef void (*LogFunc)(MessageType msgType, StringView file, int line, const char* format, va_list arglist);

typedef Delegate<void(StringView, int)> PrintDelegate;

PHANTOM_EXPORT_PHANTOM void setAssertFunc(MessageReportFunc a_func);
PHANTOM_EXPORT_PHANTOM void setErrorFunc(MessageReportFunc a_func);
PHANTOM_EXPORT_PHANTOM void setLogFunc(LogFunc a_func);
PHANTOM_EXPORT_PHANTOM void setWarningFunc(MessageReportFunc a_func);
PHANTOM_EXPORT_PHANTOM void setPrintDelegate(int a_channel, const PrintDelegate& a_func);

PHANTOM_EXPORT_PHANTOM bool assertion PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* f, int l,
                                                                         const char* format, ...);
PHANTOM_EXPORT_PHANTOM bool warning   PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* f, int l,
                                                                         const char* format, ...);
PHANTOM_EXPORT_PHANTOM bool error     PHANTOM_PREVENT_MACRO_SUBSTITUTION(const char* e, const char* f, int l,
                                                                         const char* format, ...);
PHANTOM_EXPORT_PHANTOM void log       PHANTOM_PREVENT_MACRO_SUBSTITUTION(MessageType level, const char* file, int line,
                                                                         const char* format, ...);

PHANTOM_EXPORT_PHANTOM bool isMainThread();

} // namespace phantom

namespace phantom
{
} // namespace phantom
