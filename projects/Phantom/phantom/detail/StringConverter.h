// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once


#include <phantom/detail/core.h>
#include <phantom/lang/Array.h>
#include <phantom/lang/Class.h>
#include <phantom/lang/Enum.h>
#include <phantom/traits/IsFunctionPointer.h>
#include <phantom/traits/IsNullptrT.h>
#include <phantom/traits/IsStructure.h>
#include <phantom/utils/LexicalCast.h>

/// @cond ADVANCED

namespace phantom
{
namespace lang
{
namespace detail
{
template<typename t_Ty>
struct MetaTypeIdOf;
}
} // namespace lang

namespace detail 
{
enum 
{
	string_converter_default,
	string_converter_array,
	string_converter_enum,
	string_converter_class,
	string_converter_classtype,
};
} // namespace detail

template<typename t_Ty>
struct literal_suffix
{
    static void suffix(StringBuffer&)
    {
    }
};

template<>
struct literal_suffix<unsigned long>
{
    static void suffix(StringBuffer& a_Buf)
    {
        a_Buf += "ul";
    }
};

template<>
struct literal_suffix<unsigned long long>
{
    static void suffix(StringBuffer& a_Buf)
    {
        a_Buf += "ull";
    }
};

template<>
struct literal_suffix<long>
{
    static void suffix(StringBuffer& a_Buf)
    {
        a_Buf += 'l';
    }
};

template<>
struct literal_suffix<long long>
{
    static void suffix(StringBuffer& a_Buf)
    {
        a_Buf += "ll";
    }
};

template<>
struct literal_suffix<float>
{
    static void suffix(StringBuffer& a_Buf)
    {
        if (std::find(a_Buf.begin(), a_Buf.end(), '.') == a_Buf.end()) // didn't find '.' in String
            a_Buf += '.';
        a_Buf += 'f';
    }
};

template<>
struct literal_suffix<wchar_t>
{
    static void suffix(StringBuffer& a_Buf)
    {
        a_Buf += 'L';
    }
};

template<typename t_Ty, int t_MetaClassId>
struct StringConverterH // detail::string_converter_default
{
    static void to(const lang::Type*, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        String s = phantom::lexical_cast<String>(*a_pSrc);
        a_Buf += s;
    }
    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        String s = phantom::lexical_cast<String>(*a_pSrc);
        a_Buf += s;
        literal_suffix<t_Ty>::suffix(a_Buf);
    }
    static void from(const lang::Type*, StringView input, t_Ty* a_pDest)
    {
        *a_pDest = phantom::lexical_cast<t_Ty>(input);
    }
};

template<typename t_Ty>
struct StringConverterH<t_Ty, detail::string_converter_classtype>
{
    static void to(const lang::ClassType* a_pClass, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pClass->lang::ClassType::valueToString(a_Buf, a_pSrc);
    }
    static void toLiteral(const lang::ClassType* a_pClass, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pClass->lang::ClassType::valueToString(a_Buf, a_pSrc);
    }
    static void from(const lang::ClassType* a_pClass, StringView a_strIn, t_Ty* a_pDest)
    {
        a_pClass->lang::ClassType::valueFromString(a_strIn, a_pDest);
    }
};

template<typename t_Ty>
struct StringConverterH<t_Ty, detail::string_converter_class>
{
    static void to(const lang::Class* a_pClass, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pClass->lang::Class::valueToString(a_Buf, a_pSrc);
    }
    static void toLiteral(const lang::Class* a_pClass, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pClass->lang::Class::valueToString(a_Buf, a_pSrc);
    }
    static void from(const lang::Class* a_pClass, StringView a_strIn, t_Ty* a_pDest)
    {
        a_pClass->lang::Class::valueFromString(a_strIn, a_pDest);
    }
};

template<typename t_Ty>
struct StringConverterH<t_Ty, detail::string_converter_enum>
{
    static void toLiteral(const lang::Enum* a_pEnum, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pEnum->lang::Enum::valueToLiteral(a_Buf, a_pSrc);
    }
    static void to(const lang::Enum* a_pEnum, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pEnum->lang::Enum::valueToString(a_Buf, a_pSrc);
    }
    static void from(const lang::Enum* a_pEnum, StringView a_strIn, t_Ty* a_pDest)
    {
        a_pEnum->lang::Enum::valueFromString(a_strIn, a_pDest);
    }
};

template<typename t_Ty>
struct StringConverterH<t_Ty, detail::string_converter_array>
{
    static void toLiteral(const lang::Array* a_pArray, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pArray->lang::Array::valueToLiteral(a_Buf, a_pSrc);
    }
    static void to(const lang::Array* a_pArray, StringBuffer& a_Buf, const t_Ty* a_pSrc)
    {
        return a_pArray->lang::Array::valueToString(a_Buf, a_pSrc);
    }
    static void from(const lang::Array* a_pArray, StringView a_strIn, t_Ty* a_pDest)
    {
        a_pArray->lang::Array::valueFromString(a_strIn, a_pDest);
    }
};

template<typename t_Ty>
struct StringConverter
    : public StringConverterH<t_Ty,
                              std::is_array<t_Ty>::value ? detail::string_converter_array
                                                         : std::is_enum<t_Ty>::value ? detail::string_converter_enum
                                                                                     : (std::is_void<t_Ty>::value)
                              ? detail::string_converter_default
                              : phantom::IsDataPointer<t_Ty>::value ? detail::string_converter_default
                                                                    : std::is_pointer<t_Ty>::value
                              ? detail::string_converter_default
                              : ::std::is_floating_point<t_Ty>::value ? detail::string_converter_default
                                                                      : ::phantom::IsNullptrT<t_Ty>::value
                              ? detail::string_converter_default
                              : ::std::is_integral<t_Ty>::value ? detail::string_converter_default
                                                                : ::std::is_member_function_pointer<t_Ty>::value
                              ? detail::string_converter_default
                              : ::std::is_member_object_pointer<t_Ty>::value ? detail::string_converter_default
                                                                             : std::is_union<t_Ty>::value
                              ? detail::string_converter_classtype
                              : std::is_class<t_Ty>::value ? IsStructure<t_Ty>::value ? detail::string_converter_classtype
                                                                                      : detail::string_converter_class
                                                           : detail::string_converter_default>
{
};

template<>
struct StringConverter<void>
{
    static void to(const lang::Type*, StringBuffer&, const void*)
    {
        PHANTOM_ASSERT_NO_IMPL();
    }
    static void toLiteral(const lang::Type*, StringBuffer&, const void*)
    {
        PHANTOM_ASSERT_NO_IMPL();
    }
    static void from(const lang::Type*, StringView, void*)
    {
        PHANTOM_ASSERT_NO_IMPL();
    }
};

template<>
struct StringConverter<std::nullptr_t>
{
    static void toLiteral(const lang::Type* a_pType, StringBuffer& a_Buf, const std::nullptr_t* a_pSrc)
    {
        return to(a_pType, a_Buf, a_pSrc);
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const std::nullptr_t* a_pSrc)
    {
        if (*a_pSrc != nullptr)
        {
            PHANTOM_LOG(Error, "parsing nullptr value failed");
        }
        a_Buf += "nullptr";
    }
    static void from(const lang::Type*, StringView a_strIn, std::nullptr_t* a_pDest)
    {
        if (a_strIn != "nullptr")
        {
            PHANTOM_LOG(Error, "parsing nullptr value failed");
        }
        *a_pDest = nullptr;
    }
};

template<>
struct StringConverter<char>
{
    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const char* a_pSrc)
    {
        a_Buf += '\'';
        switch (*a_pSrc)
        {
        case '\'':
            a_Buf += '\\';
            a_Buf += '\'';
            break;
        case '\\':
            a_Buf += '\\';
            a_Buf += '\\';
            break;
        case '\n':
            a_Buf += '\\';
            a_Buf += 'n';
            break;
        case '\r':
            a_Buf += '\\';
            a_Buf += 'r';
            break;
        case '\t':
            a_Buf += '\\';
            a_Buf += 't';
            break;
        case '\a':
            a_Buf += '\\';
            a_Buf += 'a';
            break;
        case '\f':
            a_Buf += '\\';
            a_Buf += 'f';
            break;
        case '\v':
            a_Buf += '\\';
            a_Buf += 'v';
            break;
        case '\0':
            a_Buf += '\\';
            a_Buf += '0';
            break;
        default:
            a_Buf += *a_pSrc;
        }
        a_Buf += '\'';
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const char* a_pSrc)
    {
        a_Buf += *a_pSrc;
    }
    static void from(const lang::Type*, StringView a_strIn, char* a_pDest)
    {
        *a_pDest = a_strIn[0];
    }
};

template<>
struct StringConverter<bool>
{
    static void toLiteral(const lang::Type* a_pType, StringBuffer& a_Buf, const bool* a_pSrc)
    {
        return to(a_pType, a_Buf, a_pSrc);
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const bool* a_pSrc)
    {
        if (*a_pSrc)
            a_Buf += "true";
        else
            a_Buf += "false";
    }
    static void from(const lang::Type*, StringView a_strIn, bool* a_pDest)
    {
        if (a_strIn == "true")
        {
            *a_pDest = true;
        }
        else
        {
            *a_pDest = false;
        }
    }
};
template<>
struct StringConverter<uchar>
{
    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const uchar* a_pSrc)
    {
        char buf[8];
        snprintf(buf, 8, "%u", (unsigned)*a_pSrc);
        a_Buf += buf;
    }
    static void to(const lang::Type* a_pType, StringBuffer& a_Buf, const uchar* a_pSrc)
    {
        return StringConverter<char>::to(a_pType, a_Buf, (const char*)a_pSrc);
    }
    static void from(const lang::Type* a_pType, StringView a_strIn, uchar* a_pDest)
    {
        StringConverter<char>::from(a_pType, a_strIn, (char*)a_pDest);
    }
};
template<>
struct StringConverter<schar>
{
    static void toLiteral(const lang::Type* a_pType, StringBuffer& a_Buf, const schar* a_pSrc)
    {
        return StringConverter<char>::toLiteral(a_pType, a_Buf, (const char*)a_pSrc);
    }
    static void to(const lang::Type* a_pType, StringBuffer& a_Buf, const schar* a_pSrc)
    {
        return StringConverter<char>::to(a_pType, a_Buf, (const char*)a_pSrc);
    }
    static void from(const lang::Type* a_pType, StringView a_strIn, schar* a_pDest)
    {
        StringConverter<char>::from(a_pType, a_strIn, (char*)a_pDest);
    }
};

#if PHANTOM_HAS_BUILT_IN_WCHAR_T
template<>
struct StringConverter<wchar_t>
{
    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const wchar_t* a_pSrc)
    {
        char buf[8];
        snprintf(buf, 8, "'\\u%04X'", (unsigned)*a_pSrc);
        a_Buf += buf;
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const wchar_t* a_pSrc)
    {
        a_Buf += *((const char*)a_pSrc);
        a_Buf += *(((const char*)a_pSrc) + 1);
    }
    static void from(const lang::Type*, StringView a_strIn, wchar_t* a_pDest)
    {
        *((char*)a_pDest) = a_strIn[0];
        *(((char*)a_pDest) + 1) = a_strIn[1];
    }
};
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR16_T
template<>
struct StringConverter<char16_t>
{
    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const char16_t* a_pSrc)
    {
        char buf[8];
        snprintf(buf, 8, "'\\u%04X'", (unsigned)*a_pSrc);
        a_Buf += buf;
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const char16_t* a_pSrc)
    {
        a_Buf += *((const char*)a_pSrc);
        a_Buf += *(((const char*)a_pSrc) + 1);
    }
    static void from(const lang::Type*, StringView a_strIn, char16_t* a_pDest)
    {
        *((char*)a_pDest) = a_strIn[0];
        *(((char*)a_pDest) + 1) = a_strIn[1];
    }
};
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR32_T
template<>
struct StringConverter<char32_t>
{
    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const char32_t* a_pSrc)
    {
        char buf[8];
        snprintf(buf, 8, "'\\u%08X'", (unsigned int)*a_pSrc);
        a_Buf += buf;
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const char32_t* a_pSrc)
    {
        a_Buf += *((const char*)a_pSrc);
        a_Buf += *(((const char*)a_pSrc) + 1);
        a_Buf += *(((const char*)a_pSrc) + 2);
        a_Buf += *(((const char*)a_pSrc) + 3);
    }
    static void from(const lang::Type*, StringView a_strIn, char32_t* a_pDest)
    {
        *((char*)a_pDest) = a_strIn[0];
        *(((char*)a_pDest) + 1) = a_strIn[1];
        *(((char*)a_pDest) + 2) = a_strIn[2];
        *(((char*)a_pDest) + 3) = a_strIn[3];
    }
};
#endif

template<typename t_Alloc>
struct StringConverter<std::basic_string<char, std::char_traits<char>, t_Alloc> >
{
    typedef std::basic_string<char, std::char_traits<char>, t_Alloc> self_type;

    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const self_type* a_pSrc)
    {
        a_Buf += '\"';
        for (auto it = a_pSrc->begin(); it != a_pSrc->end(); ++it)
        {
            switch (*it)
            {
            case '"':
                a_Buf += "\\\"";
                break;
            case '\\':
                a_Buf += "\\\\";
                break;
            case '\n':
                a_Buf += "\\n";
                break;
            case '\r':
                a_Buf += "\\r";
                break;
            case '\t':
                a_Buf += "\\t";
                break;
            case '\0':
                a_Buf += "\\0";
                break;
            default:
                a_Buf += *it;
            }
        }
        a_Buf += '\"';
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const self_type* a_pSrc)
    {
        a_Buf += a_pSrc->c_str();
    }
    static void from(const lang::Type*, StringView a_strIn, self_type* a_pDest)
    {
        a_pDest->insert(a_pDest->end(), a_strIn.begin(), a_strIn.end());
    }
};

template<size_t S, size_t D>
struct StringConverter<SmallString<char, S, D> >
{
    typedef SmallString<char, S, D> InputType;

    static void toLiteral(const lang::Type*, StringBuffer& a_Buf, const InputType* a_pSrc)
    {
        a_Buf += '\"';
        for (auto it = a_pSrc->begin(); it != a_pSrc->end(); ++it)
        {
            switch (*it)
            {
            case '"':
                a_Buf += "\\\"";
                break;
            case '\\':
                a_Buf += "\\\\";
                break;
            case '\n':
                a_Buf += "\\n";
                break;
            case '\r':
                a_Buf += "\\r";
                break;
            case '\t':
                a_Buf += "\\t";
                break;
            case '\0':
                a_Buf += "\\0";
                break;
            default:
                a_Buf += *it;
            }
        }
        a_Buf += '\"';
    }
    static void to(const lang::Type*, StringBuffer& a_Buf, const InputType* a_pSrc)
    {
        a_Buf += *a_pSrc;
    }
    static void from(const lang::Type*, StringView a_In, InputType* a_pDest)
    {
        (*a_pDest) += a_In;
    }
};

} // namespace phantom

/// @endcond
