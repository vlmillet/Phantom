// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "reflection.h"

#include "Application.h"
#include "Class.hxx"
#include "PlaceholderType.h"
#include "reflection.hxx"

// Fundamental expressions

#include "FundamentalsT.h"

/// META META TYPES
#include "Pointer.h"

#include <phantom/detail/core_internal.h>
#include <phantom/thread/RecursiveMutex.h>

namespace phantom
{
namespace lang
{
PHANTOM_EXPORT_PHANTOM void conversionOperatorNameNormalizer(StringView a_strName, StringBuffer& a_Buf,
                                                             lang::LanguageElement* a_pScope)
{
    if (a_strName.find("operator ") == 0)
    {
        lang::Type* pType = a_strName.find_first_of('.') == String::npos
        ? lang::Application::Get()->findCppType(a_strName.substr(9),
                                                a_pScope) // no dot => c++ name search
        : lang::Application::Get()->findType(a_strName.substr(9),
                                             a_pScope); // dot => phantom unique name search
        if (pType)
        {
            a_Buf += "operator ";
            pType->getUniqueName(a_Buf);
            return;
        }
    }
    a_Buf += a_strName;
}
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
        Application::Get()->_addBuiltInType(pType);                                                                    \
        detail::registerType(PHANTOM_MODULE_HANDLE(&BuiltInTypes::Register),                                           \
                             makeStringHash(StringView(#type, sizeof(#type) - 1)), pType);                             \
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

	auto pDefaultSource = Application::Get()->getDefaultSource();

#define _PHNTM_BLD_FUND_META(_var_, _type_) _var_ = pDefaultSource->NewMeta<MetaTypeOf<_type_>::type>(#_type_);

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

	Application::Get()->_addBuiltInType(PHANTOM_TYPEOF(std::nullptr_t));
	Application::Get()->_addBuiltInType(
		pDefaultSource->NewDeferred<PlaceholderType>("auto", Modifier::None, PHANTOM_R_FLAG_NATIVE));
    detail::registerType(PHANTOM_MODULE_HANDLE(&BuiltInTypes::Register),
                         makeStringHash(StringView("std::nullptr_t", sizeof("std::nullptr_t") - 1)), "std",
                         PHANTOM_TYPEOF(std::nullptr_t));
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
    write_mutex().m_private = new_<RecursiveMutex>();
}
PHANTOM_EXPORT_PHANTOM void releaseSystem()
{
    delete_<RecursiveMutex>(reinterpret_cast<RecursiveMutex*>(write_mutex().m_private));
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

ReflectionMutex::ReflectionMutex() {}

ReflectionMutex::~ReflectionMutex() {}

void ReflectionMutex::lock()
{
    ((RecursiveMutex*)m_private)->lock();
}

void ReflectionMutex::unlock()
{
    ((RecursiveMutex*)m_private)->unlock();
}

#endif // PHANTOM_CUSTOM_THREAD_SAFE

} // namespace lang
} // namespace phantom
