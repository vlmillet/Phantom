// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/detail/StringConverter.h>
#include <phantom/lang/PrimitiveType.h>
#include <phantom/lang/TypeOf.h>
#include <phantom/traits/SafeAlignOf.h>
#include <phantom/traits/SafeSizeOf.h>

namespace phantom
{
namespace lang
{
#define _PHNTM_HAS_LONG                                                                                                \
    (std::is_same<t_Ty, long>::value || std::is_same<t_Ty, double>::value || std::is_same<t_Ty, int>::value ||         \
     std::is_same<t_Ty, unsigned int>::value || std::is_same<t_Ty, unsigned long>::value)

template<typename t_Ty>
struct SafeMakeLong
{
    typedef void type;
};
template<>
struct SafeMakeLong<int>
{
    typedef long type;
};
template<>
struct SafeMakeLong<long>
{
    typedef long long type;
};
template<>
struct SafeMakeLong<unsigned int>
{
    typedef unsigned long type;
};
template<>
struct SafeMakeLong<double>
{
    typedef long double type;
};
template<>
struct SafeMakeLong<unsigned long>
{
    typedef unsigned long long type;
};

#define _PHNTM_HAS_UNSIGNED                                                                                            \
    ((std::is_integral<t_Ty>::value || std::is_enum<t_Ty>::value) && !std::is_same<t_Ty, bool>::value)

template<typename t_Ty, bool is_valid>
struct SafeMakeUnsignedH : std::make_unsigned<t_Ty>
{
};

template<typename t_Ty>
struct SafeMakeUnsignedH<t_Ty, false>
{
    typedef void type;
};

template<typename t_Ty>
struct SafeMakeUnsigned : SafeMakeUnsignedH<t_Ty, _PHNTM_HAS_UNSIGNED>
{
};
template<typename t_Ty, bool is_valid>
struct SafeMakeSignedH : std::make_signed<t_Ty>
{
};

template<typename t_Ty>
struct SafeMakeSignedH<t_Ty, false>
{
    typedef void type;
};

template<typename t_Ty>
struct SafeMakeSigned : SafeMakeSignedH<t_Ty, _PHNTM_HAS_UNSIGNED>
{
};

template<typename t_Ty>
struct UniqueTypeNameOf;

#define _PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(t, n)                                                                          \
    template<>                                                                                                         \
    struct UniqueTypeNameOf<t>                                                                                         \
    {                                                                                                                  \
        static const char* name() { return #n; }                                                                       \
        static void        name(StringBuffer& a_Buf) { a_Buf += #n; }                                                  \
    };
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(void, void);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(char, char);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(unsigned char, unsigned char);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(signed char, signed char);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(short, short);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(unsigned short, unsigned short);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(int, int);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(unsigned int, unsigned int);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(long, long);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(unsigned long, unsigned long);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(long long, long long);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(unsigned long long, unsigned long long);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(float, float);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(double, double);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(long double, long double);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(bool, bool);
_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(std::nullptr_t, std.nullptr_t);
PHANTOM_IF_WCHAR_T(_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(wchar_t, wchar_t));
PHANTOM_IF_CHAR16_T(_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(char16_t, char16_t));
PHANTOM_IF_CHAR32_T(_PHNTM_SPEC_UNIQUE_TYPE_NAME_OF(char32_t, char32_t));

template<typename t_Ty>
class PrimitiveTypeT : public PrimitiveType
{
    PHANTOM_DECL_TYPE;

public:
    PrimitiveTypeT(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags)
        : PrimitiveType(::phantom::lang::detail::PrimitiveTypeKindH<t_Ty>::value, a_strName, a_uiSize, a_uiAlignment,
                        a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        TypeInfosOf<t_Ty>::object().qualifiedDecoratedName(a_Buf);
    }
    void getQualifiedName(StringBuffer& a_Buf) const override { TypeInfosOf<t_Ty>::object().qualifiedName(a_Buf); }
    void getDecoratedName(StringBuffer& a_Buf) const override { TypeInfosOf<t_Ty>::object().decoratedName(a_Buf); }
    void getUniqueName(StringBuffer& a_Buf) const override { UniqueTypeNameOf<t_Ty>::name(a_Buf); }

    Type* asNullptrType() const override
    {
        return phantom::IsNullptrT<t_Ty>::value ? (Type*)this : nullptr;
        ;
    }

    Type* unsignedType() const override
    {
        return _PHNTM_HAS_UNSIGNED ? PHANTOM_TYPEOF(PHANTOM_TYPENAME SafeMakeUnsigned<t_Ty>::type) : nullptr;
    }

    Type* signedType() const override
    {
        return _PHNTM_HAS_UNSIGNED ? PHANTOM_TYPEOF(PHANTOM_TYPENAME SafeMakeSigned<t_Ty>::type) : nullptr;
    }

    Type* longType() const override
    {
        return _PHNTM_HAS_LONG ? PHANTOM_TYPEOF(PHANTOM_TYPENAME SafeMakeLong<t_Ty>::type) : nullptr;
    }

    bool isSignedInteger() const override
    {
        return _PHNTM_HAS_UNSIGNED
        ? (static_cast<Type*>(PHANTOM_TYPEOF(PHANTOM_TYPENAME SafeMakeSigned<t_Ty>::type)) == this)
        : false;
    }

    bool isUnsignedInteger() const override
    {
        return _PHNTM_HAS_UNSIGNED
        ? (static_cast<Type*>(PHANTOM_TYPEOF(PHANTOM_TYPENAME SafeMakeUnsigned<t_Ty>::type)) == this)
        : false;
    }

    void valueFromString(StringView a_strIn, void* a_pDest) const override
    {
        StringConverter<t_Ty>::from(this, a_strIn, (t_Ty*)a_pDest);
    }

    void valueToString(StringBuffer& a_Buf, const void* a_pSrc) const override
    {
        StringConverter<t_Ty>::to(this, a_Buf, (const t_Ty*)a_pSrc);
    }

    void valueToLiteral(StringBuffer& a_Buf, const void* a_pSrc) const override
    {
        StringConverter<t_Ty>::toLiteral(this, a_Buf, (const t_Ty*)a_pSrc);
    }
};
} // namespace lang
} // namespace phantom
