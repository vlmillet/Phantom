#pragma once

// haunt {

#include "Value.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/static_field>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "SmallMap.hxx"
#include "SmallVector.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Value")

        PHANTOM_STRUCT_T((class, class, bool, bool), (T0, T1, T0_unsigned, T1_unsigned), ValuePrimitiveEqualityPromoterSignDependant)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_TS((class, class), (T0, T1), (T0, T1, true, false), ValuePrimitiveEqualityPromoterSignDependant)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_TS((class, class), (T0, T1), (T0, T1, false, true), ValuePrimitiveEqualityPromoterSignDependant)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_TS((class, class), (T0, T1), (T0, T1, false, false), ValuePrimitiveEqualityPromoterSignDependant)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_T((class, class), (T0, T1), ValuePrimitiveEqualityPromoter)
        {
            this_()
            .PHANTOM_T inherits<::phantom::ValuePrimitiveEqualityPromoterSignDependant<T0, T1, std::is_unsigned<T0>::value, std::is_unsigned<T1>::value>>()
            ;
        }
        PHANTOM_STRUCT_TS((class), (T0), (T0, T0), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((float, double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((double, float), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((float, long double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((long double, float), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((double, long double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((long double, double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((long long, double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((double, long long), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((long long, long double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((long double, long long), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((unsigned long long, double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((double, unsigned long long), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((unsigned long long, long double), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((long double, unsigned long long), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((bool, signed char), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((signed char, bool), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((bool, unsigned char), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((unsigned char, bool), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((bool, char), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((char, bool), ValuePrimitiveEqualityPromoter)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_T((class, class, bool, bool), (T0, T1, T0_unsigned, T1_unsigned), ValuePrimitiveEqualitySignDependant)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            .PHANTOM_T staticMethod<bool(T0, T1)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_TS((class, class), (T0, T1), (T0, T1, false, false), ValuePrimitiveEqualitySignDependant)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            .PHANTOM_T staticMethod<bool(T0, T1)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_TS((class, class), (T0, T1), (T0, T1, true, false), ValuePrimitiveEqualitySignDependant)
        {
            using promoted_type = typedef_<PHANTOM_TYPENAME _::promoted_type>;
            this_()
            .PHANTOM_T typedef_<promoted_type>("promoted_type")
            .PHANTOM_T staticMethod<bool(T0, T1)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_TS((class, class), (T0, T1), (T0, T1, false, true), ValuePrimitiveEqualitySignDependant)
        {
            using promoted_type = typedef_<PHANTOM_TYPENAME _::promoted_type>;
            this_()
            .PHANTOM_T typedef_<promoted_type>("promoted_type")
            .PHANTOM_T staticMethod<bool(T0, T1)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_T((class, class), (T0, T1), ValuePrimitiveEquality)
        {
            this_()
            .PHANTOM_T inherits<::phantom::ValuePrimitiveEqualitySignDependant<T0, T1, std::is_unsigned<T0>::value, std::is_unsigned<T1>::value>>()
            ;
        }
        PHANTOM_STRUCT_S((double, float), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(double, float, double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((float, double), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(float, double, double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((long double, double), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(long double, double, long double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((double, long double), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(double, long double, long double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((long double, float), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(long double, float, long double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((float, long double), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(float, long double, long double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_TS((class), (T0), (T0, T0), ValuePrimitiveEquality)
        {
            this_()
            .PHANTOM_T staticMethod<bool(T0, T0)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((double, double), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(double, double, double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((long double, long double), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(long double, long double, long double)>("isSame", &_::isSame)
            ;
        }
        PHANTOM_STRUCT_S((float, float), ValuePrimitiveEquality)
        {
            this_()
            .staticMethod<bool(float, float, float)>("isSame", &_::isSame)
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Value)
        {
            using Array = typedef_<_::Array>;
            using EType = typedef_<_::EType>;
            using Separator = typedef_<_::Separator>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            using Struct = typedef_<_::Struct>;
            this_()
        .public_()
            .struct_<Separator>()
                .constructor<void(char), explicit_>()
                .field("c", &_::Separator::c)
            .end()
            .enum_<EType>().values({
                {"e_undefined",_::e_undefined},
                {"e_bool",_::e_bool},
                {"e_char",_::e_char},
                {"e_uint8",_::e_uint8},
                {"e_int8",_::e_int8},
                {"e_int16",_::e_int16},
                {"e_uint16",_::e_uint16},
                {"e_int32",_::e_int32},
                {"e_uint32",_::e_uint32},
                {"e_int64",_::e_int64},
                {"e_uint64",_::e_uint64},
                {"e_float",_::e_float},
                {"e_double",_::e_double},
                {"e_longdouble",_::e_longdouble},
                {"e_wchar_t",_::e_wchar_t},
                {"e_char16_t",_::e_char16_t},
                {"e_char32_t",_::e_char32_t},
                {"e_hex64",_::e_hex64},
                {"e_cstr",_::e_cstr},
                {"e_struct",_::e_struct},
                {"e_array",_::e_array}})
            .end()
            .typedef_<Struct>("Struct")
            .typedef_<Array>("Array")
            .staticField("null", &_::null)
            .constructor<void()>()
            .constructor<void(const Value&)>()
            .constructor<void(Value&&)>()
            .constructor<void(StringView), explicit_>()
            .constructor<void(hex64), explicit_>()
            .method<Value&(Value&&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<Value&(const Value&)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<Value&(String)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<Value&(StringView)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<Value&(hex64)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(hex64&) const>("as", &_::as)
            .method<StringView() const>("str", &_::str)
            .constructor<void(bool)>()
            .method<Value&(bool)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(bool&) const>("as", &_::as)
            .constructor<void(char)>()
            .method<::phantom::Value &(char)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(char&) const>("as", &_::as)
            .constructor<void(::phantom::uint8)>()
            .method<::phantom::Value &(::phantom::uint8)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::uint8 &) const>("as", &_::as)
            .constructor<void(::phantom::int8)>()
            .method<::phantom::Value &(::phantom::int8)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::int8 &) const>("as", &_::as)
            .constructor<void(::phantom::int16)>()
            .method<::phantom::Value &(::phantom::int16)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::int16 &) const>("as", &_::as)
            .constructor<void(::phantom::uint16)>()
            .method<::phantom::Value &(::phantom::uint16)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::uint16 &) const>("as", &_::as)
            .constructor<void(::phantom::int32)>()
            .method<::phantom::Value &(::phantom::int32)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::int32 &) const>("as", &_::as)
            .constructor<void(::phantom::uint32)>()
            .method<::phantom::Value &(::phantom::uint32)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::uint32 &) const>("as", &_::as)
            .constructor<void(::phantom::int64)>()
            .method<::phantom::Value &(::phantom::int64)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::int64 &) const>("as", &_::as)
            .constructor<void(::phantom::uint64)>()
            .method<::phantom::Value &(::phantom::uint64)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::uint64 &) const>("as", &_::as)
            .constructor<void(float)>()
            .method<::phantom::Value &(float)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(float&) const>("as", &_::as)
            .constructor<void(double)>()
            .method<::phantom::Value &(double)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(double&) const>("as", &_::as)
            .constructor<void(::phantom::longdouble)>()
            .method<::phantom::Value &(::phantom::longdouble)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(::phantom::longdouble &) const>("as", &_::as)
            .constructor<void(wchar_t)>()
            .method<::phantom::Value &(wchar_t)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(wchar_t&) const>("as", &_::as)
            .constructor<void(char16_t)>()
            .method<::phantom::Value &(char16_t)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(char16_t&) const>("as", &_::as)
            .constructor<void(char32_t)>()
            .method<::phantom::Value &(char32_t)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)
            .method<bool(char32_t&) const>("as", &_::as)
            .method<bool(String&) const>("as", &_::as)
            .method<bool(hex64) const>("isSame", &_::isSame)
            .method<bool(StringView) const>("isSame", &_::isSame)
            .method<bool(const Value&) const>("isSame", &_::isSame)
            .method<bool(bool) const>("isSame", &_::isSame)
            .method<bool(char) const>("isSame", &_::isSame)
            .method<bool(::phantom::uint8) const>("isSame", &_::isSame)
            .method<bool(::phantom::int8) const>("isSame", &_::isSame)
            .method<bool(::phantom::int16) const>("isSame", &_::isSame)
            .method<bool(::phantom::uint16) const>("isSame", &_::isSame)
            .method<bool(::phantom::int32) const>("isSame", &_::isSame)
            .method<bool(::phantom::uint32) const>("isSame", &_::isSame)
            .method<bool(::phantom::int64) const>("isSame", &_::isSame)
            .method<bool(::phantom::uint64) const>("isSame", &_::isSame)
            .method<bool(float, float) const>("isSame", &_::isSame)
            .method<bool(double, double) const>("isSame", &_::isSame)
            .method<bool(::phantom::longdouble, ::phantom::longdouble) const>("isSame", &_::isSame)
            .method<bool(wchar_t) const>("isSame", &_::isSame)
            .method<bool(char16_t) const>("isSame", &_::isSame)
            .method<bool(char32_t) const>("isSame", &_::isSame)
            .method<void()>("reset", &_::reset)
            .method<EType() const>("getType", &_::getType)
            .method<bool() const>("isArray", &_::isArray)
            .method<bool() const>("isStruct", &_::isStruct)
            .method<bool() const>("isDefined", &_::isDefined)
            .method<Array&()>("asArray", &_::asArray)
            .method<Struct&()>("asStruct", &_::asStruct)
            .method<const Array&() const>("asArray", &_::asArray)
            .method<const Struct&() const>("asStruct", &_::asStruct)
            .method<void(Value&&)>("addValue", &_::addValue)
            .method<Value&()>("addValue", &_::addValue)
            .method<void(StringView)>("removeValue", &_::removeValue)
            .method<void(size_t)>("removeValue", &_::removeValue)
            .method<Value&(StringView)>("addValue", &_::addValue)
            .method<Value&()>("addStructValue", &_::addStructValue)
            .method<Value&(StringView)>("addStructValue", &_::addStructValue)
            .method<Value&()>("addArrayValue", &_::addArrayValue)
            .method<Value&(StringView)>("addArrayValue", &_::addArrayValue)
            .method<void(size_t)>("setArray", &_::setArray)["0"]
            .method<void()>("setStruct", &_::setStruct)
            .method<void(const Value&)>("addValue", &_::addValue)
            .method<bool() const>("isNull", &_::isNull)
            .method<bool() const>("isValid", &_::isValid)
            .method<Value&(StringView, Separator)>("addValue", &_::addValue)
            .method<const Value&(StringView, Separator) const>("getValue", &_::getValue)
            .method<Value&(StringView, Separator)>("getValue", &_::getValue)
            .method<Value&(StringView)>("getValue", &_::getValue)
            .method<const Value&(StringView) const>("getValue", &_::getValue)
            .method<const Value&(size_t) const>("getValue", &_::getValue)
            .method<void(StringView, const Value&)>("addValue", &_::addValue)
            .method<void(StringView, Value&&)>("addValue", &_::addValue)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Value")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
