// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <phantom/detail/phantom.h>
#include <phantom/detail/typedefs.h>
#include <phantom/reflection/reflection.h>
#include <phantom/utils/SmallString.h>
#include <phantom/utils/StringHash.h>
#include <phantom/utils/StringView.h>

/// @cond ADVANCED

namespace phantom
{
namespace detail
{
PHANTOM_EXPORT_PHANTOM void registerOrphanMemory(void*);
}

namespace reflection
{
struct TypeInfos
{
    TypeInfos() = default;
    TypeInfos(char* buffer) : buffer(buffer)
    {
    }
    TypeInfos(TypeInfos&& temp)
        : hash(temp.hash),
          buffer(temp.buffer),
          decoration_length(temp.decoration_length),
          scope_length(temp.scope_length),
          name_length(temp.name_length)
    {
        temp.buffer = 0;
    }

    TypeInfos(const TypeInfos&) = delete;
    TypeInfos& operator=(const TypeInfos&) = delete;

    bool empty() const
    {
        return buffer == nullptr;
    }

    inline StringView name() const
    {
        return StringView(buffer + scope_length, name_length);
    }
    inline StringView scope() const
    {
        return StringView(buffer, scope_length - ((!!scope_length) << 1)); // 'scope length - 2' if 'scope_length > 0'
                                                                           // ex: "std::" => "std" but "" => ""
    }
    inline StringView decoratedName() const
    {
        return StringView(buffer + scope_length, name_length + decoration_length);
    }
    inline StringView qualifiedName() const
    {
        return StringView(buffer, scope_length + name_length);
    }
    inline StringView qualifiedDecoratedName() const
    {
        return StringView(buffer, scope_length + name_length + decoration_length);
    }

    void decoratedName(StringBuffer& a_Buf) const
    {
        size_t      count = name_length + decoration_length;
        const char* cstr = buffer + scope_length;
        a_Buf.append(cstr, count);
    }
    void qualifiedName(StringBuffer& a_Buf) const
    {
        size_t      count = name_length + scope_length;
        const char* cstr = buffer;
        a_Buf.append(cstr, count);
    }
    void qualifiedDecoratedName(StringBuffer& a_Buf) const
    {
        size_t      count = name_length + scope_length + decoration_length;
        const char* cstr = buffer;
        a_Buf.append(cstr, count);
    }

    uint64 hash = 0;
    char*  buffer = nullptr;
    uint16 decoration_length = 0;
    uint16 scope_length = 0;
    uint16 name_length = 0;
};

namespace detail
{
PHANTOM_EXPORT_PHANTOM void       BuildTypeInfos(StringView a_TypeName, TypeInfos& a_TI);
PHANTOM_EXPORT_PHANTOM StringView PrettyFunctionToTypeName(StringView a_TypeName);
template<class T>
constexpr const char* TypeIdNameBase()
{
    return PHANTOM_PRETTY_FUNCTION_VAR;
}
} // namespace detail

template<typename T>
StringView TypeIdName()
{
    static StringView s_Name = detail::TypeIdNameBase<T>();
    return s_Name;
};

template<typename T>
uint64_t TypeIdHash()
{
    auto            name = TypeIdName<T>();
    static uint64_t s_Hash = makeStringHash(name);
    return s_Hash;
};

template<typename t_Ty>
struct TypeInfosOf
{
    static const TypeInfos& object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        static TypeInfos ti;
        if (ti.empty())
        {
            detail::BuildTypeInfos(PHANTOM_PRETTY_FUNCTION_VAR, ti);
        }
        return ti;
    }
};

template<typename t_Ty>
struct TypeInfosOf<t_Ty*>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

template<typename t_Ty>
struct TypeInfosOf<t_Ty&>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

#if PHANTOM_HAS_RVALUE_REFERENCES
template<typename t_Ty>
struct TypeInfosOf<t_Ty&&>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

#endif

template<typename t_Ty>
struct TypeInfosOf<t_Ty const>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

template<typename t_Ty>
struct TypeInfosOf<t_Ty volatile>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

template<typename t_Ty>
struct TypeInfosOf<t_Ty const volatile>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

template<typename t_Ty, size_t i>
struct TypeInfosOf<t_Ty[i]>
{
    PHANTOM_STATIC_ASSERT((std::is_same<t_Ty, t_Ty*>::value), "TypeInfosOf only usable on non-compound types");
};

#define PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(_type_)                                                                \
    template<>                                                                                                         \
    struct TypeInfosOf<_type_>                                                                                         \
    {                                                                                                                  \
        static const TypeInfos& object()                                                                               \
        {                                                                                                              \
            PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();                                                                    \
            static TypeInfos ti;                                                                                       \
            if (ti.empty())                                                                                            \
            {                                                                                                          \
                detail::BuildTypeInfos(PHANTOM_PP_QUOTE(_type_), ti);                                                  \
            }                                                                                                          \
            return ti;                                                                                                 \
        }                                                                                                              \
    };

PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(char);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(unsigned char);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(signed char);
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(wchar_t);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(char16_t);
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(char32_t);
#endif
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(short);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(unsigned short);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(int);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(unsigned int);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(long);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(unsigned long);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(long long);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(unsigned long long);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(float);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(double);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(long double);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(bool);
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(void);
#if PHANTOM_COMPILER != PHANTOM_COMPILER_VISUAL_STUDIO && PHANTOM_SIZE_OF_VOID_P == 8
PHANTOM_SPEC_FUNDAMENTALS_TYPE_INFOS_OF(__int128);
#endif

template<>
struct TypeInfosOf<std::nullptr_t>
{
    static const TypeInfos& object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        static TypeInfos ti;
        if (ti.empty())
        {
            detail::BuildTypeInfos("std::nullptr_t", ti);
        }
        return ti;
    }
};

} // namespace reflection

template<typename t_Ty>
struct TypeNameOf
{
    static StringView name()
    {
        return reflection::TypeInfosOf<t_Ty>::object().qualifiedDecoratedName();
    }
};

template<typename t_Ty>
struct TypeNameOf<t_Ty*>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + '*';
    }
};

template<typename t_Ty>
struct TypeNameOf<t_Ty&>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + '&';
    }
};

#if PHANTOM_HAS_RVALUE_REFERENCES
template<typename t_Ty>
struct TypeNameOf<t_Ty&&>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + "&&";
    }
};

#endif

template<typename t_Ty>
struct TypeNameOf<t_Ty const>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + " const";
    }
};

template<typename t_Ty>
struct TypeNameOf<t_Ty volatile>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + " volatile";
    }
};

template<typename t_Ty>
struct TypeNameOf<t_Ty const volatile>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + " const volatile";
    }
};

template<typename t_Ty, size_t i>
struct TypeNameOf<t_Ty[i]>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + '[' + std::to_string(i) + ']';
    }
};

template<typename t_Ty>
struct TypeNameOf<t_Ty[]>
{
    static String name()
    {
        return String(TypeNameOf<t_Ty>::name()) + '[' + ']';
    }
};

} // namespace phantom

/// @endcond
