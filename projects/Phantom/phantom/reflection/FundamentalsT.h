// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;

#include "Promoter.h"

#include <phantom/reflection/Constant.h>
#include <phantom/reflection/PrimitiveTypeT.h>
#include <phantom/reflection/TypeT.h>

/// @cond INTERNAL

#define PHANTOM_FUNDAMENTAL_SWITCH_CASE(t, tk, op)                                                                     \
    case TypeKind::tk:                                                                                                 \
        op(t, tk)

#define PHANTOM_FUNDAMENTAL_SWITCH_CASE_INTEGRAL_NO_BOOL(op)                                                           \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(char, Char, op);                                                                   \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(schar, SChar, op);                                                                 \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(uchar, UChar, op);                                                                 \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(short, Short, op);                                                                 \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(ushort, UShort, op);                                                               \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(int, Int, op);                                                                     \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(uint, UInt, op);                                                                   \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(long, Long, op);                                                                   \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(ulong, ULong, op);                                                                 \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(longlong, LongLong, op);                                                           \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(ulonglong, ULongLong, op);                                                         \
    PHANTOM_IF_WCHAR_T(PHANTOM_FUNDAMENTAL_SWITCH_CASE(wchar_t, WChar, op);)                                           \
    PHANTOM_IF_CHAR16_T(PHANTOM_FUNDAMENTAL_SWITCH_CASE(char16_t, Char16, op);)                                        \
    PHANTOM_IF_CHAR32_T(PHANTOM_FUNDAMENTAL_SWITCH_CASE(char32_t, Char32, op);)

#define PHANTOM_FUNDAMENTAL_SWITCH_CASE_INTEGRAL(op)                                                                   \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(bool, Bool, op);                                                                   \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE_INTEGRAL_NO_BOOL(op);

#define PHANTOM_FUNDAMENTAL_SWITCH_CASE_FLOATING(op)                                                                   \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(float, Float, op);                                                                 \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(double, Double, op);                                                               \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE(longdouble, LongDouble, op);

#define PHANTOM_FUNDAMENTAL_SWITCH_CASE_ARITHMETIC(op)                                                                 \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE_FLOATING(op);                                                                      \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE_INTEGRAL(op);

#define PHANTOM_FUNDAMENTAL_SWITCH_CASE_ARITHMETIC_NO_BOOl(op)                                                         \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE_FLOATING(op);                                                                      \
    PHANTOM_FUNDAMENTAL_SWITCH_CASE_INTEGRAL_NO_BOOL(op);

namespace phantom
{
namespace reflection
{
template<typename t_Ty>
class FundamentalTypeT : public TypeT<t_Ty, PrimitiveTypeT<t_Ty>>
{
    PHANTOM_DECL_TYPE;

public:
    FundamentalTypeT(StringView a_strName)
        : TypeT<t_Ty, PrimitiveTypeT<t_Ty>>(a_strName, PHANTOM_R_NONE, PHANTOM_R_FLAG_NONE)
    {
    }

    PrimitiveType* asFundamentalType() const override
    {
        return (PrimitiveType*)this;
    }

    Type* promote() const override
    {
        return Promoter<t_Ty>::apply(const_cast<FundamentalTypeT<t_Ty>*>(this));
    }
};

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    define _PHNTM_FORBIDDEN_VOID_METHOD PHANTOM_ASSERT(false, __FUNCTION__ " invalid call on 'void' type")
#else
#    define _PHNTM_FORBIDDEN_VOID_METHOD PHANTOM_ASSERT(false, "invalid call on 'void' type")
#endif

template<typename t_Base>
class TypeT<void, t_Base> : public t_Base
{
public:
    typedef TypeT<void, t_Base> SelfType;
    typedef t_Base              BaseType;

protected:
    TypeT(StringView a_TypeName, Modifiers a_Modifiers, uint a_uiFlags)
        : BaseType(a_TypeName, 0, 0, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

public:
    virtual bool isDefined() const
    {
        return true;
    }
    void valueFromString(StringView, void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }
    void valueToString(StringBuffer&, const void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }
    void valueToLiteral(StringBuffer&, const void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }
    void copyAssign(void*, void const*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
	}
	void copyConstruct(void*, void const*) const override
	{
		_PHNTM_FORBIDDEN_VOID_METHOD;
	}

    void* allocate() const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
        return nullptr;
    }
    void deallocate(void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }
    void* allocate(size_t) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
        return nullptr;
    }
    void deallocate(void*, size_t) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }

    // Construction
    void construct(void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }
    void destroy(void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }

    void* newInstance() const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
        return nullptr;
    }
    void deleteInstance(void*) const override
    {
        _PHNTM_FORBIDDEN_VOID_METHOD;
    }

    bool isCopyable() const override
    {
        return false;
    }
    bool isCopyAssignable() const override
    {
        return false;
    }
    bool isCopyConstructible() const override
    {
        return false;
    }
    bool isMoveable() const override
    {
        return false;
    }
    bool isMoveAssignable() const override
    {
        return false;
    }
    bool isMoveConstructible() const override
    {
        return false;
    }
    bool isTriviallyMoveAssignable() const override
    {
        return false;
    }
    bool isTriviallyMoveConstructible() const override
    {
        return false;
    }
    bool isTriviallyCopyAssignable() const override
    {
        return false;
    }
    bool isTriviallyCopyConstructible() const override
    {
        return false;
    }

    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        a_Buf += "void";
    }
    hash64 computeHash() const override
    {
        return this->ComputeHash("void", 4);
    }
};

template<>
class FundamentalTypeT<void> : public TypeT<void, PrimitiveTypeT<void>>
{
    PHANTOM_DECL_TYPE;

public:
    FundamentalTypeT(StringView a_strName) : TypeT<void, PrimitiveTypeT<void>>(a_strName, 0, 0)
    {
    }

    PrimitiveType* asFundamentalType() const override
    {
        return (PrimitiveType*)this;
    }
    Type* promote() const override
    {
        return const_cast<FundamentalTypeT<void>*>(this);
    }
};

template<typename t_Ty>
class ArithmeticTypeT : public FundamentalTypeT<t_Ty>
{
    PHANTOM_DECL_TYPE;

public:
    ArithmeticTypeT(StringView a_strName) : FundamentalTypeT<t_Ty>(a_strName)
    {
    }

    PrimitiveType* asArithmeticType() const override
    {
        return (PrimitiveType*)this;
    }

    Constant* createConstant(void* a_pSrc, StringView a_strName = "",
                             PrimitiveType* a_pPrimitiveType = nullptr) const override
    {
        return reflection::Constant::Create<t_Ty>(*(t_Ty*)a_pSrc, a_strName, a_pPrimitiveType);
    }

    bool convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const override
    {
        t_Ty const& src = *(t_Ty const*)a_pSrc;

#define _PHNTM_FUNDAMENTAL_CONV(t, tk)                                                                                 \
    {                                                                                                                  \
        *(t*)a_pDst = src;                                                                                             \
        return true;                                                                                                   \
    }

        switch (a_pDstType->getTypeKind())
        {
#pragma warning(disable : 4800)
#pragma warning(disable : 4244)
            PHANTOM_FUNDAMENTAL_SWITCH_CASE_ARITHMETIC(_PHNTM_FUNDAMENTAL_CONV)
#pragma warning(default : 4800)
#pragma warning(default : 4244)

        case TypeKind::Enum:
            return convert(static_cast<Enum*>(a_pDstType)->getUnderlyingType(), a_pDst, a_pSrc);

        default:
            return false;
        }
    }
};

template<typename t_Ty>
class FloatingPointTypeT : public ArithmeticTypeT<t_Ty>
{
    PHANTOM_DECL_TYPE;

public:
    FloatingPointTypeT(StringView a_strName) : ArithmeticTypeT<t_Ty>(a_strName)
    {
    }

    PrimitiveType* asFloatingPointType() const override
    {
        return (PrimitiveType*)this;
    }
};

template<typename t_Ty>
class IntegralTypeT : public ArithmeticTypeT<t_Ty>
{
    PHANTOM_DECL_TYPE;

public:
    IntegralTypeT(StringView a_strName) : ArithmeticTypeT<t_Ty>(a_strName)
    {
    }

    PrimitiveType* asIntegralType() const override
    {
        return (PrimitiveType*)this;
    }
};

} // namespace reflection
} // namespace phantom

/// @endcond
