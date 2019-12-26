// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "reflection.h"

#include "Application.h"
#include "Class.hxx"
#include "PlaceholderType.h"
#include "reflection.hxx"

// Fundamental expressions

#include "FundamentalsT.h"

/// META META TYPES
#include "..\phantom_priv.h"
#include "..\thread\RecursiveMutex.h"
#include "Pointer.h"

namespace phantom
{
namespace reflection
{
namespace detail
{
PHANTOM_EXPORT_PHANTOM Type* findType(StringView a_strName, Namespace* a_pScope /*= Namespace::Global()*/)
{
    return Application::Get()->findCppType(a_strName, a_pScope);
}
PHANTOM_EXPORT_PHANTOM Type* findType(const char* a_strName, Namespace* a_pScope /*= Namespace::Global()*/)
{
    return Application::Get()->findCppType(a_strName, a_pScope);
}
PHANTOM_EXPORT_PHANTOM Type* findType(StringView a_strName, ClassType* a_pScope)
{
    return Application::Get()->findCppType(a_strName, a_pScope);
}
PHANTOM_EXPORT_PHANTOM Type* findType(StringView a_strName, Class* a_pScope)
{
    return Application::Get()->findCppType(a_strName, a_pScope);
}
PHANTOM_EXPORT_PHANTOM Type* findType(hash64 a_Hash)
{
    return Application::Get()->findType(a_Hash);
}
} // namespace detail

void BuiltInTypes::Register()
{
    /// META-META-... TYPES :) (or type representation of the type representation of the type
    /// representation ... to INFINITY !

#define _PHNTM_REG_FUND(type)                                                                                          \
    {                                                                                                                  \
        auto pType = PHANTOM_TYPEOF(type);                                                                             \
        detail::registerType(PHANTOM_MODULE_HANDLE(&BuiltInTypes::Register),                                           \
                             makeStringHash(StringView(#type, sizeof(#type) - 1)), pType);                             \
        Application::Get()->_addBuiltInType(pType);                                                                    \
    }

    static bool installed = false;
    if (installed)
        return;
    installed = true;

    detail::pushInstallation();
    PHANTOM_ASSERT(Class::metaClass == nullptr);
    Class_PHANTOM_.this_()._PHNTM_getMeta();
    PHANTOM_ASSERT(Class::metaClass);
    detail::popInstallation();

#define _PHNTM_BLD_FUND_META(_var_, _type_) _var_ = PHANTOM_META_NEW(MetaTypeOf<_type_>::type)(#_type_);

    detail::pushInstallation();
    _PHNTM_BLD_FUND_META(TYPE_VOID, void);
    _PHNTM_BLD_FUND_META(TYPE_CHAR, char);
    _PHNTM_BLD_FUND_META(TYPE_UNSIGNED_CHAR, unsigned char);
    _PHNTM_BLD_FUND_META(TYPE_SIGNED_CHAR, signed char);
    _PHNTM_BLD_FUND_META(TYPE_SHORT, short);
    _PHNTM_BLD_FUND_META(TYPE_UNSIGNED_SHORT, unsigned short);
    _PHNTM_BLD_FUND_META(TYPE_INT, int);
    _PHNTM_BLD_FUND_META(TYPE_UNSIGNED_INT, unsigned int);
    _PHNTM_BLD_FUND_META(TYPE_LONG, long);
    _PHNTM_BLD_FUND_META(TYPE_UNSIGNED_LONG, unsigned long);
    _PHNTM_BLD_FUND_META(TYPE_LONG_LONG, long long);
    _PHNTM_BLD_FUND_META(TYPE_UNSIGNED_LONG_LONG, unsigned long long);
    _PHNTM_BLD_FUND_META(TYPE_FLOAT, float);
    _PHNTM_BLD_FUND_META(TYPE_DOUBLE, double);
    _PHNTM_BLD_FUND_META(TYPE_LONG_DOUBLE, long double);
    _PHNTM_BLD_FUND_META(TYPE_BOOL, bool);
    using std::nullptr_t;
    _PHNTM_BLD_FUND_META(TYPE_NULLPTR_T, nullptr_t);
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
    _PHNTM_BLD_FUND_META(TYPE_WCHAR_T, wchar_t);
#else
    TYPE_WCHAR_T = BuiltInTypes::TYPE_UNSIGNED_SHORT;
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
    _PHNTM_BLD_FUND_META(TYPE_CHAR16_T, char16_t);
#else
    TYPE_CHAR16_T = BuiltInTypes::TYPE_UNSIGNED_SHORT;
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
    _PHNTM_BLD_FUND_META(TYPE_CHAR32_T, char32_t);
#else
    TYPE_CHAR32_T = BuiltInTypes::TYPE_UNSIGNED_INT;
#endif

    _PHNTM_REG_FUND(char);
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
    _PHNTM_REG_FUND(wchar_t);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
    _PHNTM_REG_FUND(char16_t);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
    _PHNTM_REG_FUND(char32_t);
#endif
    _PHNTM_REG_FUND(short);
    _PHNTM_REG_FUND(int);
    _PHNTM_REG_FUND(long);
    _PHNTM_REG_FUND(float);
    _PHNTM_REG_FUND(double);
    _PHNTM_REG_FUND(bool);
    _PHNTM_REG_FUND(void);
    _PHNTM_REG_FUND(signed char);
    _PHNTM_REG_FUND(unsigned char);
    _PHNTM_REG_FUND(unsigned short);
    _PHNTM_REG_FUND(unsigned int);
    _PHNTM_REG_FUND(unsigned long);
    _PHNTM_REG_FUND(long double);
    _PHNTM_REG_FUND(long long);
    _PHNTM_REG_FUND(unsigned long long);

    TYPE_VOID_PTR = TYPE_VOID->makePointer();

    detail::registerType(PHANTOM_MODULE_HANDLE(&BuiltInTypes::Register),
                         makeStringHash(StringView("std::nullptr_t", sizeof("std::nullptr_t") - 1)), "std",
                         PHANTOM_TYPEOF(std::nullptr_t));
    Application::Get()->_addBuiltInType(PHANTOM_TYPEOF(std::nullptr_t));
    Application::Get()->_addBuiltInType(
    PHANTOM_DEFERRED_NEW(PlaceholderType)("auto", Modifier::None, PHANTOM_R_FLAG_NATIVE));
    detail::popInstallation();
}

Type* BuiltInTypes::TYPE_VOID = nullptr;
Type* BuiltInTypes::TYPE_CHAR = nullptr;
Type* BuiltInTypes::TYPE_UNSIGNED_CHAR = nullptr;
Type* BuiltInTypes::TYPE_SIGNED_CHAR = nullptr;
Type* BuiltInTypes::TYPE_SHORT = nullptr;
Type* BuiltInTypes::TYPE_UNSIGNED_SHORT = nullptr;
Type* BuiltInTypes::TYPE_INT = nullptr;
Type* BuiltInTypes::TYPE_UNSIGNED_INT = nullptr;
Type* BuiltInTypes::TYPE_LONG = nullptr;
Type* BuiltInTypes::TYPE_UNSIGNED_LONG = nullptr;
Type* BuiltInTypes::TYPE_LONG_LONG = nullptr;
Type* BuiltInTypes::TYPE_UNSIGNED_LONG_LONG = nullptr;
Type* BuiltInTypes::TYPE_FLOAT = nullptr;
Type* BuiltInTypes::TYPE_DOUBLE = nullptr;
Type* BuiltInTypes::TYPE_LONG_DOUBLE = nullptr;
Type* BuiltInTypes::TYPE_BOOL = nullptr;
Type* BuiltInTypes::TYPE_NULLPTR_T = nullptr;

Type* BuiltInTypes::TYPE_WCHAR_T =
#if PHANTOM_HAS_BUILT_IN_WCHAR_T == 1
nullptr;
#else
BuiltInTypes::TYPE_UNSIGNED_SHORT;
#endif

Type* BuiltInTypes::TYPE_CHAR16_T =
#if PHANTOM_HAS_BUILT_IN_CHAR16_T == 1
nullptr;
#else
BuiltInTypes::TYPE_UNSIGNED_SHORT;
#endif

Type* BuiltInTypes::TYPE_CHAR32_T =
#if PHANTOM_HAS_BUILT_IN_CHAR32_T == 1
nullptr;
#else
BuiltInTypes::TYPE_UNSIGNED_SHORT;
#endif

Type* BuiltInTypes::TYPE_VOID_PTR = nullptr;

Class* BuiltInTypes::TYPE_STD_STRING = nullptr;

Class* BuiltInTypes::TYPE_STRING = nullptr;

PHANTOM_EXPORT_PHANTOM void initializeSystem()
{
    write_mutex().m_private = PHANTOM_NEW(RecursiveMutex);
}
PHANTOM_EXPORT_PHANTOM void releaseSystem()
{
    PHANTOM_DELETE(RecursiveMutex) reinterpret_cast<RecursiveMutex*>(write_mutex().m_private);
}

#if PHANTOM_CUSTOM_THREAD_SAFE
PHANTOM_EXPORT_PHANTOM ReflectionMutex& read_mutex()
{
    static ReflectionMutex m;
    return m;
}

PHANTOM_EXPORT_PHANTOM ReflectionMutex& write_mutex()
{
    static ReflectionMutex m;
    return m;
}

ReflectionMutex::ReflectionMutex()
{
}

ReflectionMutex::~ReflectionMutex()
{
}

void ReflectionMutex::lock()
{
    ((RecursiveMutex*)m_private)->lock();
}

void ReflectionMutex::unlock()
{
    ((RecursiveMutex*)m_private)->unlock();
}

#endif // PHANTOM_CUSTOM_THREAD_SAFE

} // namespace reflection
} // namespace phantom
