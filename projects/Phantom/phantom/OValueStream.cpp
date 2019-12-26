// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "OValueStream.h"

#include "StringConverter.h"

#include <phantom/Value.h>

#pragma warning(disable : 4996)

namespace phantom
{
OValueStream::OValueStream(std::ostream& a_OutStream, int32 tabSize /*= 4*/)
    : m_OutStream(a_OutStream), m_tabSize(tabSize), m_indentation(0)
{
}

bool OValueStream::isIndented(const Value& val) const
{
    switch (val.getType())
    {
    case Value::e_array:
    {
        return !val.asArray().empty() &&
        (val.getValue(size_t(0)).getType() == Value::e_array || val.getValue(size_t(0)).getType() == Value::e_struct);
    }
    break;
    case Value::e_struct:
    {
        return !val.asStruct().empty();
    }
    break;
    default:
        break;
    }
    return false;
}

#define _value_ostream_case_fundamental(type)                                                                          \
    case Value::e_##type:                                                                                              \
    {                                                                                                                  \
        type v = 0;                                                                                                    \
        val.as(v);                                                                                                     \
        StringConverter<type>::to(PHANTOM_TYPEOF(type), buf, &v);                                                      \
        print(buf.data());                                                                                             \
        buf.clear();                                                                                                   \
    }                                                                                                                  \
    break;

#define _value_ostream_case_fundamental_suffix(type, suffix)                                                           \
    case Value::e_##type:                                                                                              \
    {                                                                                                                  \
        type v = 0;                                                                                                    \
        val.as(v);                                                                                                     \
        StringConverter<type>::to(PHANTOM_TYPEOF(type), buf, &v);                                                      \
        print(buf.data());                                                                                             \
        print(suffix);                                                                                                 \
        buf.clear();                                                                                                   \
    }                                                                                                                  \
    break;

#define _value_ostream_case_fundamental_lit(type)                                                                      \
    case Value::e_##type:                                                                                              \
    {                                                                                                                  \
        type v = 0;                                                                                                    \
        val.as(v);                                                                                                     \
        StringConverter<type>::toLiteral(PHANTOM_TYPEOF(type), buf, &v);                                               \
        print(buf.data());                                                                                             \
        buf.clear();                                                                                                   \
    }                                                                                                                  \
    break;

OValueStream& OValueStream::operator<<(const Value& val)
{
    char         buffer[256];
    StringBuffer buf;
    switch (val.getType())
    {
    case Value::e_array:
    {
        if (isIndented(val))
        {
            line("[");
            indent();
            auto& arr = val.asArray();
            for (size_t i = 0; i < arr.size(); ++i)
            {
                tabs();
                operator<<(arr[i]);
                if (i != arr.size() - 1)
                    line(",");
                else
                    line("");
            }
            unindent();
            tabs();
            print("]");
        }
        else
        {
            print("[");
            auto& arr = val.asArray();
            for (size_t i = 0; i < arr.size(); ++i)
            {
                operator<<(arr[i]);
                if (i != arr.size() - 1)
                    print(", ");
            }
            print("]");
        }
    }
    break;
    case Value::e_struct:
    {
        line("{");
        auto& strct = val.asStruct();
        auto  it = strct.begin();
        if (it != strct.end())
        {
            indent();
            auto last = strct.end();
            last--;
            for (; it != strct.end(); ++it)
            {
                tabs();
                auto& pair = *it;
                print("\"");
                print(pair.first.c_str(), pair.first.size());
                print("\":");
                operator<<(pair.second);
                line("");
            }
            unindent();
        }
        tabs();
        print("}");
    }
    break;
    case Value::e_hex64:
    {
        const int size = sizeof(uint64) * 2;
        hex64     x;
        val.as(x);
        print("0x");
        char format[9] = "00000000";
        sprintf(format, "%%0%d%sX", size, size > 8 ? "ll" : "");
        char buf2[size + 1];
        sprintf(buf2, format, uint64(x));
        print(buf2, size);
    }
    break;
    case Value::e_bool:
    {
        bool x;
        val.as(x);
        print(x ? "true" : "false");
    }
    break;
        _value_ostream_case_fundamental_lit(char);

    case Value::e_int8:
    {
        int8 x;
        val.as(x);
        sprintf(buffer, "%" PRIi8 "s8", x);
        print(buffer);
    }
    break;
    case Value::e_int16:
    {
        int16 x;
        val.as(x);
        sprintf(buffer, "%" PRIi16 "s16", x);
        print(buffer);
    }
    break;
    case Value::e_int32:
    {
        int32 x;
        val.as(x);
        sprintf(buffer, "%" PRIi32 "s32", x);
        print(buffer);
    }
    break;
    case Value::e_int64:
    {
        int64 x;
        val.as(x);
        sprintf(buffer, "%" PRIi64 "s64", x);
        print(buffer);
    }
    break;
    case Value::e_uint8:
    {
        uint8 x;
        val.as(x);
        sprintf(buffer, "%" PRIu8 "u8", x);
        print(buffer);
    }
    break;
    case Value::e_uint16:
    {
        uint16 x;
        val.as(x);
        sprintf(buffer, "%" PRIu16 "u16", x);
        print(buffer);
    }
    break;
    case Value::e_uint32:
    {
        uint32 x;
        val.as(x);
        sprintf(buffer, "%" PRIu32 "u32", x);
        print(buffer);
    }
    break;
    case Value::e_uint64:
    {
        uint64 x;
        val.as(x);
        sprintf(buffer, "%" PRIu64 "u64", x);
        print(buffer);
    }
    break;
    case Value::e_float:
    {
        float x;
        val.as(x);
        sprintf(buffer, "%ff", x);
        print(buffer);
    }
    break;
    case Value::e_double:
    {
        double x;
        val.as(x);
        sprintf(buffer, "%lf", x);
        print(buffer);
    }
    break;
    case Value::e_longdouble:
    {
        longdouble x;
        val.as(x);
        sprintf(buffer, "%lfl", (double)x);
        print(buffer);
    }
    break;
        PHANTOM_IF_WCHAR_T(_value_ostream_case_fundamental_lit(wchar_t))
        PHANTOM_IF_CHAR16_T(_value_ostream_case_fundamental_lit(char16_t));
        PHANTOM_IF_CHAR32_T(_value_ostream_case_fundamental_lit(char32_t));
    case Value::e_cstr:
    {
        StringView str = val.str();
        String     v(str);
        StringConverter<String>::toLiteral(nullptr, buf, &v);
        print(buf);
    }
    break;
    default:
        PHANTOM_ASSERT(false);
    }
    return *this;
}

void OValueStream::indent()
{
    ++m_indentation;
}

void OValueStream::unindent()
{
    --m_indentation;
}

void OValueStream::line(StringView a_Text)
{
    m_OutStream.write(a_Text.data(), a_Text.size());
    if (m_tabSize >= 0)
        m_OutStream << std::endl;
}

void OValueStream::tabs()
{
    if (m_tabSize > 0)
        for (int32 i = 0; i < m_tabSize * m_indentation; ++i)
            m_OutStream << ' ';
    else if (m_tabSize == 0)
        for (int32 i = 0; i < m_indentation; ++i)
            m_OutStream << '\t';
    else
        m_OutStream << ' ';
}

} // namespace phantom
