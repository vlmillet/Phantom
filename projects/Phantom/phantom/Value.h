// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/SmallMap.h>
#include <phantom/lexical_cast.h>
#include <phantom/new>
#include <phantom/phantom.h>

namespace phantom
{
template<class T0, class T1, bool T0_unsigned, bool T1_unsigned> // both unsigned
struct ValuePrimitiveEqualityPromoterSignDependant
{
    typedef PHANTOM_TYPENAME std::conditional<(sizeof(T0) >= sizeof(T1)), T0, T1>::type type;
};

template<class T0, class T1> // T0 unsigned T1 signed
struct ValuePrimitiveEqualityPromoterSignDependant<T0, T1, true, false>
{
    typedef PHANTOM_TYPENAME std::conditional<(sizeof(T0) >= sizeof(T1)), T0, T1>::type type;
};

template<class T0, class T1> // T0 signed T1 unsigned
struct ValuePrimitiveEqualityPromoterSignDependant<T0, T1, false, true>
{
    typedef PHANTOM_TYPENAME std::conditional<(sizeof(T1) >= sizeof(T0)), T1, T0>::type type;
};

template<class T0, class T1> // both signed
struct ValuePrimitiveEqualityPromoterSignDependant<T0, T1, false, false>
{
    typedef PHANTOM_TYPENAME std::conditional<(sizeof(T0) >= sizeof(T1)), T0, T1>::type type;
};

template<class T0, class T1>
struct ValuePrimitiveEqualityPromoter
    : public ValuePrimitiveEqualityPromoterSignDependant<T0, T1, std::is_unsigned<T0>::value,
                                                         std::is_unsigned<T1>::value>
{
};

template<class T0>
struct ValuePrimitiveEqualityPromoter<T0, T0>
{
    typedef T0 type;
};

template<>
struct ValuePrimitiveEqualityPromoter<float, double>
{
    typedef double type;
};
template<>
struct ValuePrimitiveEqualityPromoter<double, float>
{
    typedef double type;
};
template<>
struct ValuePrimitiveEqualityPromoter<float, long double>
{
    typedef long double type;
};
template<>
struct ValuePrimitiveEqualityPromoter<long double, float>
{
    typedef long double type;
};
template<>
struct ValuePrimitiveEqualityPromoter<double, long double>
{
    typedef long double type;
};
template<>
struct ValuePrimitiveEqualityPromoter<long double, double>
{
    typedef long double type;
};
template<>
struct ValuePrimitiveEqualityPromoter<long long, double>
{
    typedef long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<double, long long>
{
    typedef long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<long long, long double>
{
    typedef long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<long double, long long>
{
    typedef long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<unsigned long long, double>
{
    typedef unsigned long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<double, unsigned long long>
{
    typedef unsigned long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<unsigned long long, long double>
{
    typedef unsigned long long type;
};
template<>
struct ValuePrimitiveEqualityPromoter<long double, unsigned long long>
{
    typedef unsigned long long type;
};

template<>
struct ValuePrimitiveEqualityPromoter<bool, signed char>
{
    typedef signed char type;
};
template<>
struct ValuePrimitiveEqualityPromoter<signed char, bool>
{
    typedef signed char type;
};
template<>
struct ValuePrimitiveEqualityPromoter<bool, unsigned char>
{
    typedef unsigned char type;
};
template<>
struct ValuePrimitiveEqualityPromoter<unsigned char, bool>
{
    typedef unsigned char type;
};
template<>
struct ValuePrimitiveEqualityPromoter<bool, char>
{
    typedef char type;
};
template<>
struct ValuePrimitiveEqualityPromoter<char, bool>
{
    typedef char type;
};

template<class T0, class T1, bool T0_unsigned, bool T1_unsigned>
struct ValuePrimitiveEqualitySignDependant
{
    typedef PHANTOM_TYPENAME ValuePrimitiveEqualityPromoter<T0, T1>::type type;
    static bool                                                           isSame(T0 v0, T1 v1)
    {
        return (type)v0 == (type)v1;
    }
};

template<class T0, class T1>
struct ValuePrimitiveEqualitySignDependant<T0, T1, false, false>
{
    typedef PHANTOM_TYPENAME ValuePrimitiveEqualityPromoter<T0, T1>::type type;
    static bool                                                           isSame(T0 v0, T1 v1)
    {
        return (type)v0 == (type)v1;
    }
};

template<class T0, class T1>
struct ValuePrimitiveEqualitySignDependant<T0, T1, true, false>
{
    typedef PHANTOM_TYPENAME ValuePrimitiveEqualityPromoter<T0, T1>::type promoted_type;
    static bool                                                           isSame(T0 v0, T1 v1)
    {
        return std::is_same<T1, promoted_type>::value // T1 is the promoted => simple cast is ok
        ? (promoted_type(v0) == promoted_type(v1))
        : (v1 >= T1(0) &&
           (promoted_type(v0) == promoted_type(v1))); // T1 is not the promoted => we need to first test the negativity
    }
};

template<class T0, class T1>
struct ValuePrimitiveEqualitySignDependant<T0, T1, false, true>
{
    typedef PHANTOM_TYPENAME ValuePrimitiveEqualityPromoter<T0, T1>::type promoted_type;
    static bool                                                           isSame(T0 v0, T1 v1)
    {
        return std::is_same<T0, promoted_type>::value // T0 is the promoted => simple cast is ok
        ? ((promoted_type)v0 == (promoted_type)v1)
        : (v0 >= T0(0) &&
           ((promoted_type)v0 == (promoted_type)v1)); // T0 is not the promoted => we need to first test the negativity
    }
};

template<class T0, class T1>
struct ValuePrimitiveEquality
    : public ValuePrimitiveEqualitySignDependant<T0, T1, std::is_unsigned<T0>::value, std::is_unsigned<T1>::value>
{
};

template<>
struct ValuePrimitiveEquality<double, float>
{
    static bool isSame(double v0, float v1, double epsilon)
    {
        return std::abs(v0 - double(v1)) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<float, double>
{
    static bool isSame(float v0, double v1, double epsilon)
    {
        return std::abs(v1 - double(v0)) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<long double, double>
{
    static bool isSame(long double v0, double v1, long double epsilon)
    {
        return std::abs(v0 - (long double)(v1)) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<double, long double>
{
    static bool isSame(double v0, long double v1, long double epsilon)
    {
        return std::abs((long double)(v0)-v1) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<long double, float>
{
    static bool isSame(long double v0, float v1, long double epsilon)
    {
        return std::abs(v0 - (long double)(v1)) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<float, long double>
{
    static bool isSame(float v0, long double v1, long double epsilon)
    {
        return std::abs((long double)(v0)-v1) <= epsilon;
    }
};

template<class T0>
struct ValuePrimitiveEquality<T0, T0>
{
    static bool isSame(T0 v0, T0 v1)
    {
        return v0 == v1;
    }
};

template<>
struct ValuePrimitiveEquality<double, double>
{
    static bool isSame(double v0, double v1, double epsilon)
    {
        return std::abs(v0 - v1) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<long double, long double>
{
    static bool isSame(long double v0, long double v1, long double epsilon)
    {
        return std::abs(v0 - v1) <= epsilon;
    }
};

template<>
struct ValuePrimitiveEquality<float, float>
{
    static bool isSame(float v0, float v1, float epsilon)
    {
        return std::abs(v0 - v1) <= epsilon;
    }
};

class PHANTOM_EXPORT_PHANTOM Value
{
public:
    struct Separator
    {
        explicit Separator(char c) : c(c)
        {
        }
        char c;
    };

    enum EType
    {
        e_undefined,
        e_bool,
        e_char,
        e_uint8,
        e_int8,
        e_int16,
        e_uint16,
        e_int32,
        e_uint32,
        e_int64,
        e_uint64,
        e_float,
        e_double,
        e_longdouble,
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
        e_wchar_t,
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
        e_char16_t,
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
        e_char32_t,
#endif
        e_hex64,
        e_cstr,
        e_struct,
        e_array,
    };

    typedef SmallMap<String, Value> Struct;
    typedef SmallVector<Value>      Array;

    static const Value null;

    Value()
    {
    }

    Value(const Value& copied) : m_type(copied.m_type)
    {
        PHANTOM_ASSERT(!copied.isNull());
        if (m_type == e_struct)
        {
            m_struct = PHANTOM_NEW(Struct)(*copied.m_struct);
        }
        else if (m_type == e_array)
        {
            m_array = PHANTOM_NEW(Array)(*copied.m_array);
        }
        else if (m_type == e_cstr)
        {
            m_cstr = (char*)PHANTOM_MALLOC(copied.m_cstr_size + 1);
            memcpy(m_cstr, copied.m_cstr, copied.m_cstr_size + 1);
            m_cstr_size = copied.m_cstr_size;
        }
        else
            memcpy(this, &copied, sizeof(Value)); // pure raw copy
    }

    Value(Value&& temp)
    {
        PHANTOM_ASSERT(!temp.isNull());
        memcpy(this, &temp, sizeof(Value)); // pure raw copy
        temp.m_type = e_undefined;
    }

    explicit Value(StringView cstr) : m_type(e_cstr)
    {
        m_cstr_size = cstr.size();
        m_cstr = (char*)PHANTOM_MALLOC(m_cstr_size + 1);
        memcpy(m_cstr, cstr.data(), m_cstr_size + 1);
    }

    explicit Value(hex64 hex) : m_type(e_hex64)
    {
        m_hex64 = hex;
    }

    ~Value()
    {
        _release();
    }

    // if LINK fails here
    //
    // NOTE: the bodies of these 3 methods are defined in phantom/serialization/ValueSerializer.h
    // which is in the phantom.serialization module
    template<class T>
    explicit inline Value(const T& val);

    template<class T>
    inline Value& operator=(T const& val);

    template<class T>
    inline bool as(T& val) const;
    // END NODE

    Value& operator=(Value&& temp)
    {
        PHANTOM_ASSERT(!temp.isNull());
        PHANTOM_ASSERT(!isNull());
        _release();
        m_type = temp.m_type;
        memcpy(this, &temp, sizeof(Value));
        temp.m_type = e_undefined;
        return *this;
    }

    Value& operator=(const Value& copied)
    {
        PHANTOM_ASSERT(!copied.isNull());
        PHANTOM_ASSERT(!isNull());
        _release();
        m_type = copied.m_type;
        if (m_type == e_struct)
        {
            m_struct = PHANTOM_NEW(Struct)(*copied.m_struct);
        }
        else if (m_type == e_array)
        {
            m_array = PHANTOM_NEW(Array)(*copied.m_array);
        }
        else if (m_type == e_cstr)
        {
            m_cstr_size = copied.m_cstr_size;
            m_cstr = (char*)PHANTOM_MALLOC(m_cstr_size + 1);
            memcpy(m_cstr, copied.m_cstr, m_cstr_size + 1);
        }
        else
            memcpy(this, &copied, sizeof(Value)); // pure raw copy
        return *this;
    }

    Value& operator=(String c)
    {
        return operator=(StringView(c));
    }

    Value& operator=(StringView c)
    {
        PHANTOM_ASSERT(!isNull());
        _release();
        m_type = e_cstr;
        m_cstr_size = c.size();
        m_cstr = (char*)PHANTOM_MALLOC(m_cstr_size + 1);
        memcpy(m_cstr, c.data(), m_cstr_size + 1);
        return *this;
    }

    Value& operator=(hex64 x)
    {
        PHANTOM_ASSERT(!isNull());
        _release();
        m_type = e_hex64;
        m_hex64 = x;
        return *this;
    }

    bool as(hex64& val) const
    {
        PHANTOM_ASSERT(!isNull());
        if (m_type == e_hex64)
        {
            val = m_hex64;
            return true;
        }
        return false;
    }

    StringView str() const
    {
        PHANTOM_ASSERT(!isNull());
        if (m_type == e_cstr)
        {
            return StringView(m_cstr, m_cstr_size);
        }
        return StringView();
    }

#define _apply_for_every_integral_type(macro)                                                                          \
    PHANTOM_IF_WCHAR_T(macro(wchar_t));                                                                                \
    PHANTOM_IF_CHAR16_T(macro(char16_t));                                                                              \
    PHANTOM_IF_CHAR32_T(macro(char32_t));                                                                              \
    _apply_for_every_integral_type_no_wchar(macro)

#define _apply_for_every_integral_type2(macro)                                                                         \
    PHANTOM_IF_WCHAR_T(macro(wchar_t));                                                                                \
    PHANTOM_IF_CHAR16_T(macro(char16_t));                                                                              \
    PHANTOM_IF_CHAR32_T(macro(char32_t));                                                                              \
    _apply_for_every_integral_type_no_wchar2(macro)

#define _apply_for_every_floating_type(macro) macro(float) macro(double) macro(longdouble)

#define _apply_for_every_floating_type2(macro) macro(float) macro(double) macro(longdouble)

#define _apply_for_every_integral_type_no_wchar(macro)                                                                 \
    macro(char) macro(int8) macro(uint8) macro(int16) macro(uint16) macro(int32) macro(uint32) macro(int64)            \
    macro(uint64)

#define _apply_for_every_integral_type_no_wchar2(macro)                                                                \
    macro(char) macro(int8) macro(uint8) macro(int16) macro(uint16) macro(int32) macro(uint32) macro(int64)            \
    macro(uint64)

#define _apply_for_every_fundamental_type(macro)                                                                       \
    PHANTOM_IF_WCHAR_T(macro(wchar_t))                                                                                 \
    PHANTOM_IF_CHAR16_T(macro(char16_t))                                                                               \
    PHANTOM_IF_CHAR32_T(macro(char32_t))                                                                               \
    _apply_for_every_fundamental_type_no_wchar(macro)

#define _apply_for_every_fundamental_type_no_wchar(macro)                                                              \
    macro(bool) _apply_for_every_integral_type_no_wchar(macro) _apply_for_every_floating_type(macro)

#define _make_case_cast(type)                                                                                          \
    case e_##type:                                                                                                     \
        val = (cast_type)m_##type;                                                                                     \
        break;
#define _make_case_cast_bool(type)                                                                                     \
    case e_##type:                                                                                                     \
        val = m_##type != 0;                                                                                           \
        break;
#define _make_case_lexical_cast_string(type)                                                                           \
    case e_##type:                                                                                                     \
        val = lexical_cast<String>(m_##type);                                                                          \
        break;

#define _value_ctor_operator_as(type)                                                                                  \
    Value(type val) : m_type(e_##type), m_##type(val)                                                                  \
    {                                                                                                                  \
    }                                                                                                                  \
    Value& operator=(type val)                                                                                         \
    {                                                                                                                  \
        PHANTOM_ASSERT(!isNull());                                                                                     \
        _release();                                                                                                    \
        m_type = e_##type;                                                                                             \
        m_##type = val;                                                                                                \
        return *this;                                                                                                  \
    }                                                                                                                  \
    bool as(type& val) const                                                                                           \
    {                                                                                                                  \
        PHANTOM_ASSERT(!isNull());                                                                                     \
        typedef type cast_type;                                                                                        \
        switch (m_type)                                                                                                \
        {                                                                                                              \
            _make_case_cast(bool) _apply_for_every_integral_type(_make_case_cast)                                      \
            _apply_for_every_floating_type(_make_case_cast) default : return false;                                    \
        }                                                                                                              \
        return true;                                                                                                   \
    }

    // special bool case
    Value(bool val) : m_type(e_bool), m_bool(val)
    {
    }
    Value& operator=(bool val)
    {
        PHANTOM_ASSERT(!isNull());
        _release();
        m_type = e_bool;
        m_bool = val;
        return *this;
    }
    bool as(bool& val) const
    {
        PHANTOM_ASSERT(!isNull());
        switch (m_type)
        {
        case e_bool:
        {
            val = m_bool;
        }
        break;
            _apply_for_every_integral_type(_make_case_cast_bool)
            _apply_for_every_floating_type(_make_case_cast_bool) default : return false;
        }
        return true;
    }
    _value_ctor_operator_as(char) _value_ctor_operator_as(uint8) _value_ctor_operator_as(int8)
    _value_ctor_operator_as(int16) _value_ctor_operator_as(uint16) _value_ctor_operator_as(int32)
    _value_ctor_operator_as(uint32) _value_ctor_operator_as(int64) _value_ctor_operator_as(uint64)
    _value_ctor_operator_as(float) _value_ctor_operator_as(double) _value_ctor_operator_as(longdouble)
    PHANTOM_IF_WCHAR_T(_value_ctor_operator_as(wchar_t)) PHANTOM_IF_CHAR16_T(_value_ctor_operator_as(char16_t))
    PHANTOM_IF_CHAR32_T(_value_ctor_operator_as(char32_t))

#undef _value_ctor_operator_as

    bool as(String& val) const
    {
        PHANTOM_ASSERT(!isNull());
        if (m_type == e_cstr)
        {
            val.assign(m_cstr, m_cstr_size);
            return true;
        }
        else
        {
            switch (m_type)
            {
                _apply_for_every_fundamental_type_no_wchar(_make_case_lexical_cast_string) default : break;
            }
        }
        return false;
    }

#define _value_make_equals_case_integral(_type_)                                                                       \
    case e_##_type_:                                                                                                   \
        return ValuePrimitiveEquality<std::remove_cv<std::remove_reference<decltype(v)>::type>::type, _type_>::isSame( \
        v, m_##_type_);
#define _value_make_equals_case_floating(_type_)                                                                       \
    case e_##_type_:                                                                                                   \
        return ValuePrimitiveEquality<std::remove_cv<std::remove_reference<decltype(v)>::type>::type, _type_>::isSame( \
        v, m_##_type_, epsilon);

    inline bool isSame(hex64 v) const
    {
        PHANTOM_ASSERT(!isNull());
        return m_type == e_hex64 && m_hex64 == v;
    }

    inline bool isSame(StringView v) const
    {
        PHANTOM_ASSERT(!isNull());
        return m_type == e_cstr && str() == v;
    }

#define _value_make_other_value_case_integral(_type_)                                                                  \
    case e_##_type_:                                                                                                   \
    {                                                                                                                  \
        _type_ v = other.m_##_type_;                                                                                   \
        switch (m_type)                                                                                                \
        {                                                                                                              \
            _apply_for_every_floating_type2(_value_make_equals_case_integral)                                          \
            _apply_for_every_integral_type2(_value_make_equals_case_integral)                                          \
            _value_make_equals_case_integral(bool) default : return false;                                             \
        }                                                                                                              \
    }                                                                                                                  \
    break;

#define _value_make_other_value_case_floating(_type_)                                                                  \
    case e_##_type_:                                                                                                   \
    {                                                                                                                  \
        _type_ v = other.m_##_type_;                                                                                   \
        switch (m_type)                                                                                                \
        {                                                                                                              \
            _apply_for_every_floating_type2(_value_make_equals_case_floating)                                          \
            _apply_for_every_integral_type2(_value_make_equals_case_integral)                                          \
            _value_make_equals_case_integral(bool) default : return false;                                             \
        }                                                                                                              \
    }                                                                                                                  \
    break;

    inline bool isSame(const Value& other) const
    {
        PHANTOM_ASSERT(!isNull());
        float epsilon = 0;
        auto  input_type = other.m_type;
        switch (input_type)
        {
        case e_struct:
        {
            if (m_type != e_struct || m_struct->size() != other.m_struct->size())
                return false;
            for (auto& member : *m_struct)
            {
                const Value& other_member = other.getValue(member.first);
                if (!other_member.isDefined() || !member.second.isSame(other_member))
                    return false;
            }
            return true;
        }
        break;
        case e_array:
        {
            if (m_type != e_array || m_array->size() != other.m_array->size())
                return false;
            for (size_t i = 0; i < m_array->size(); ++i)
            {
                if (!(*m_array)[i].isSame(other.getValue(i)))
                    return false;
            }
            return true;
        }
        break;
        case e_cstr:
        {
            if (m_type != e_cstr)
                return false;
            return str() == other.str();
        }
        break;
        case e_hex64:
        {
            if (m_type != e_hex64)
                return false;
            return m_hex64 == other.m_hex64;
        }
        break;
            _apply_for_every_floating_type(_value_make_other_value_case_floating)
            _apply_for_every_integral_type(_value_make_other_value_case_integral)
            _value_make_other_value_case_integral(bool) default : return false;
        }
    }
    template<class T>
    inline bool isSame(const Value& other, T epsilon) const
    {
        PHANTOM_ASSERT(!isNull());
        auto input_type = other.m_type;
        switch (input_type)
        {
        case e_struct:
        {
            if (m_type != e_struct || m_struct->size() != other.m_struct->size())
                return false;
            for (auto& member : *m_struct)
            {
                const Value& other_member = other.getValue(member.first);
                if (!other_member.isDefined() || !member.second.isSame(other_member, epsilon))
                    return false;
            }
            return true;
        }
        break;
        case e_array:
        {
            if (m_type != e_array || m_array->size() != other.m_array->size())
                return false;
            for (size_t i = 0; i < m_array->size(); ++i)
            {
                if (!(*m_array)[i].isSame(other.getValue(i), epsilon))
                    return false;
            }
            return true;
        }
        break;
        case e_cstr:
        {
            if (m_type != e_cstr || m_cstr_size != other.m_cstr_size)
                return false;
            return str() == other.str();
        }
        break;
        case e_hex64:
        {
            if (m_type != e_hex64)
                return false;
            return m_hex64 == other.m_hex64;
        }
        break;
            _apply_for_every_floating_type(_value_make_other_value_case_floating)
            _apply_for_every_integral_type(_value_make_other_value_case_integral)
            _value_make_other_value_case_integral(bool) default : return false;
        }
    }

#define _value_equals_member_function(type)                                                                            \
    inline bool isSame(type v) const                                                                                   \
    {                                                                                                                  \
        PHANTOM_ASSERT(!isNull());                                                                                     \
        enum                                                                                                           \
        {                                                                                                              \
            input_type = e_##type                                                                                      \
        };                                                                                                             \
        typedef type cast_type;                                                                                        \
        switch (m_type)                                                                                                \
        {                                                                                                              \
            _apply_for_every_floating_type(_value_make_equals_case_integral)                                           \
            _apply_for_every_integral_type(_value_make_equals_case_integral) default : return false;                   \
        }                                                                                                              \
    }
#define _value_equals_member_function_epsilon(type)                                                                    \
    inline bool isSame(type v, type epsilon = std::numeric_limits<type>::epsilon()) const                              \
    {                                                                                                                  \
        PHANTOM_ASSERT(!isNull());                                                                                     \
        enum                                                                                                           \
        {                                                                                                              \
            input_type = e_##type                                                                                      \
        };                                                                                                             \
        switch (m_type)                                                                                                \
        {                                                                                                              \
            _apply_for_every_floating_type(_value_make_equals_case_floating)                                           \
            _apply_for_every_integral_type(_value_make_equals_case_integral) default : return false;                   \
        }                                                                                                              \
    }

#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif

    _value_equals_member_function(bool) _value_equals_member_function(char) _value_equals_member_function(uint8)
    _value_equals_member_function(int8) _value_equals_member_function(int16) _value_equals_member_function(uint16)
    _value_equals_member_function(int32) _value_equals_member_function(uint32) _value_equals_member_function(int64)
    _value_equals_member_function(uint64) _value_equals_member_function_epsilon(float)
    _value_equals_member_function_epsilon(double) _value_equals_member_function_epsilon(longdouble)
    PHANTOM_IF_WCHAR_T(_value_equals_member_function(wchar_t))
    PHANTOM_IF_CHAR16_T(_value_equals_member_function(char16_t))
    PHANTOM_IF_CHAR32_T(_value_equals_member_function(char32_t))

#if defined(__clang__)
#    pragma clang diagnostic pop
#endif

    inline void reset()
    {
        PHANTOM_ASSERT(!isNull());
        _release();
        m_type = e_undefined;
    }

    inline EType getType() const
    {
        PHANTOM_ASSERT(!isNull());
        return m_type;
    }

    inline bool isArray() const
    {
        return m_type == e_array;
    }
    inline bool isStruct() const
    {
        return m_type == e_struct;
    }

    inline bool isDefined() const
    {
        return m_type != e_undefined;
    }

    inline Array& asArray()
    {
        PHANTOM_ASSERT(!isNull());
        static thread_local Array null_array;
        return m_type == e_array ? *m_array : null_array;
    }

    inline Struct& asStruct()
    {
        PHANTOM_ASSERT(!isNull());
        static thread_local Struct null_struct;
        return m_type == e_struct ? *m_struct : null_struct;
    }

    inline const Array& asArray() const
    {
        PHANTOM_ASSERT(!isNull());
        static thread_local Array null_array;
        return m_type == e_array ? *m_array : null_array;
    }

    inline const Struct& asStruct() const
    {
        PHANTOM_ASSERT(!isNull());
        static thread_local Struct null_struct;
        return m_type == e_struct ? *m_struct : null_struct;
    }

    inline void addValue(Value&& val)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_array;
            m_array = PHANTOM_NEW(Array);
        }
        PHANTOM_ASSERT(m_type == e_array);
        m_array->push_back((Value &&) val);
    }

    inline Value& addValue()
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_array;
            m_array = PHANTOM_NEW(Array);
        }
        PHANTOM_ASSERT(m_type == e_array);
        m_array->resize(m_array->size() + 1);
        return m_array->back();
    }

    inline void removeValue(StringView name)
    {
        PHANTOM_ASSERT(!isNull());
        PHANTOM_ASSERT(m_type == e_struct);
        auto found = m_struct->find(name);
        PHANTOM_ASSERT(found != m_struct->end());
        (*m_struct).erase(found);
    }

    inline void removeValue(size_t index)
    {
        PHANTOM_ASSERT(!isNull());
        PHANTOM_ASSERT(m_type == e_array);
        PHANTOM_ASSERT(index < m_array->size());
        m_array->erase(m_array->begin() + index);
    }

    inline Value& addValue(StringView a_strName)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_struct;
            m_struct = PHANTOM_NEW(Struct);
        }
        PHANTOM_ASSERT(m_type == e_struct);
        return (*m_struct)[a_strName];
    }

    inline Value& addStructValue()
    {
        Value& val = addValue();
        val.setStruct();
        return val;
    }

    inline Value& addStructValue(StringView a_Name)
    {
        Value& val = addValue(a_Name);
        val.setStruct();
        return val;
    }

    inline Value& addArrayValue()
    {
        Value& val = addValue();
        val.setArray();
        return val;
    }

    inline Value& addArrayValue(StringView a_pName)
    {
        Value& val = addValue(a_pName);
        val.setArray();
        return val;
    }

    inline void setArray(size_t count = 0)
    {
        PHANTOM_ASSERT(!isNull());
        if (m_type != e_array)
        {
            _release();
            m_type = e_array;
            m_array = PHANTOM_NEW(Array)();
            m_array->resize(count);
        }
    }

    inline void setStruct()
    {
        PHANTOM_ASSERT(!isNull());
        if (m_type != e_struct)
        {
            _release();
            m_type = e_struct;
            m_struct = PHANTOM_NEW(Struct);
        }
    }

    inline void addValue(const Value& val)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_array;
            m_array = PHANTOM_NEW(Array);
        }
        PHANTOM_ASSERT(m_type == e_array);
        m_array->push_back(val);
    }

    PHANTOM_FORCEINLINE bool isNull() const
    {
        return this == &null;
    }
    PHANTOM_FORCEINLINE bool isValid() const
    {
        return this != &null;
    }

    inline Value& addValue(StringView name, Separator s)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_struct;
            m_struct = PHANTOM_NEW(Struct)();
        }
        StringView fwd = name;
        size_t     wordSize = 0;
        while (fwd.size())
        {
            char c = fwd.front();
            if (c == s.c)
            {
                StringView word = name.substr(0, wordSize);
                name = name.substr(wordSize + 1); // skip word + dot
                wordSize = 0;
                auto found = m_struct->find(word);
                if (found == m_struct->end())
                {
                    return addValue(word).addValue(name, s);
                }
                return found->second.addValue(name, s);
            }
            else
                wordSize++;
        }
        return addValue(name.substr(0, wordSize));
    }

    inline const Value& getValue(StringView name, Separator s) const
    {
        PHANTOM_ASSERT(!isNull());
        if (e_struct == m_type)
        {
            StringView fwd = name;
            size_t     wordSize = 0;
            while (fwd.size())
            {
                char c = fwd.front();
                if (c == s.c)
                {
                    StringView word = name.substr(0, wordSize);
                    name = name.substr(wordSize + 1); // skip word + dot
                    wordSize = 0;
                    auto found = m_struct->find(word);
                    if (found == m_struct->end())
                    {
                        return null;
                    }
                    return found->second.getValue(name, s);
                }
            }
            auto found = m_struct->find(name.substr(0, wordSize));
            if (found == m_struct->end())
            {
                return null;
            }
            return found->second;
        }
        return null;
    }

    inline Value& getValue(StringView name, Separator s)
    {
        return (Value&)const_cast<Value const*>(this)->getValue(name, s);
    }

    inline Value& getValue(StringView name)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_struct == m_type)
        {
            auto found = m_struct->find(name);
            if (found != m_struct->end())
                return found->second;
        }
        return (Value&)null;
    }

    inline const Value& getValue(StringView name) const
    {
        PHANTOM_ASSERT(!isNull());
        if (e_struct == m_type)
        {
            auto found = m_struct->find(name);
            if (found != m_struct->end())
                return found->second;
        }
        return null;
    }

    inline const Value& getValue(size_t index) const
    {
        PHANTOM_ASSERT(!isNull());
        if (e_array == m_type)
        {
            return index < m_array->size() ? (*m_array)[index] : null;
        }
        return null;
    }

    inline void addValue(StringView a_strName, const Value& val)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_struct;
            m_struct = PHANTOM_NEW(Struct);
        }
        PHANTOM_ASSERT(m_type == e_struct);
        (*m_struct)[a_strName] = val;
    }

    inline void addValue(StringView a_strName, Value&& val)
    {
        PHANTOM_ASSERT(!isNull());
        if (e_undefined == m_type)
        {
            m_type = e_struct;
            m_struct = PHANTOM_NEW(Struct);
        }
        PHANTOM_ASSERT(m_type == e_struct);
        (*m_struct)[a_strName] = (Value &&) val;
    }

private:
    EType m_type = e_undefined;
    union {
        bool       m_bool;
        char       m_char;
        uint8      m_uint8;
        int8       m_int8;
        int16      m_int16;
        uint16     m_uint16;
        int32      m_int32;
        uint32     m_uint32;
        int64      m_int64;
        uint64     m_uint64;
        float      m_float;
        double     m_double;
        longdouble m_longdouble;
        PHANTOM_IF_WCHAR_T(wchar_t m_wchar_t);
        PHANTOM_IF_CHAR16_T(char16_t m_char16_t);
        PHANTOM_IF_CHAR32_T(char32_t m_char32_t);
        hex64 m_hex64;
        struct
        {
            char*  m_cstr;
            size_t m_cstr_size;
        };
        Struct* m_struct;
        Array*  m_array;
    };

    inline void _release()
    {
        if (m_type == e_struct)
        {
            PHANTOM_DELETE(Struct) m_struct;
        }
        else if (m_type == e_array)
        {
            PHANTOM_DELETE(Array) m_array;
        }
        else if (m_type == e_cstr)
        {
            PHANTOM_FREE(m_cstr);
        }
        m_type = e_undefined;
    }
};
} // namespace phantom
