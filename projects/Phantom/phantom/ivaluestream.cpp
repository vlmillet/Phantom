// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "IValueStream.h"
#if IVALUESTREAM_USES_GENERATED_PARSER
#    include "IValueStreamParser.h"
#endif // IVALUESTREAM_USES_GENERATED_PARSER
#include "value.h"

#pragma warning(disable : 4996)

namespace phantom
{
static thread_local String g_IValueStreamPrivate_last_error;
const char*                IValueStream::last_error()
{
    return g_IValueStreamPrivate_last_error.c_str();
}

#define IVS_ON_EOF IVS_ERROR_IF(valueStack.size(), "unterminated value");
#define IVS_IGNORE_WHITE_SPACES                                                                                        \
    while ((c = (char)m_InStream.peek()) == ' ' || c == '\t' || c == '\n' || c == '\r')                                \
        m_InStream.ignore();
#define IVS_DEBUG_ENABLED 1
#if IVS_DEBUG_ENABLED
#    define IVS_ASSERT(cond) PHANTOM_ASSERT(cond)
#else
#    define IVS_ASSERT(cond)
#endif
#define IVS_ERROR(str)                                                                                                 \
    {                                                                                                                  \
        g_IValueStreamPrivate_last_error = str;                                                                        \
        error = true;                                                                                                  \
        return false;                                                                                                  \
    }
#define IVS_ERROR_IF(cond, str)                                                                                        \
    if (cond)                                                                                                          \
    IVS_ERROR(str)
#define IVS_READ_AND_EXPECTS(c)                                                                                        \
    {                                                                                                                  \
        IVS_ERROR_IF(m_InStream.peek() != c, (String("unexpected token ") + (char)m_InStream.peek()));                 \
        m_InStream.get();                                                                                              \
    }
#define IVS_BACK_AFTER_VALUE                                                                                           \
    valueStack.pop_back();                                                                                             \
    if (valueStack.empty())                                                                                            \
        return true;                                                                                                   \
    if (valueStack.back()->isArray())                                                                                  \
        goto state_array_next_value;                                                                                   \
    else                                                                                                               \
        goto state_struct;

#define IVS_CASE_WHITE_SPACE                                                                                           \
    case ' ':                                                                                                          \
    case '\t':                                                                                                         \
    case '\r':                                                                                                         \
    case '\n'
#define IVS_CASE_VALUE_SEPARATOR                                                                                       \
    IVS_CASE_WHITE_SPACE:                                                                                              \
    case '"':                                                                                                          \
    case ',':                                                                                                          \
    case ']':                                                                                                          \
    case '}'

#define IVS_STACK_SIZE 128
bool IValueStream::operator>>(Value& value)
{
    char                       c = 0;
    SmallVector<char, 50, 100> token;
    SmallVector<Value*, 10>    valueStack;
    Value                      valuePool[IVS_STACK_SIZE];
    int                        valueCount = 0;
    valueStack.push_back(&value);
    hex64 hex;
    char  escapedChar[4];
    bool  escaped_from_string = false;
    int   escapedCount = 0;

    /// VALUE
state_value:
    IVS_IGNORE_WHITE_SPACES;
    while ((c = (char)m_InStream.peek()) != EOF)
    {
        switch (c)
        {
        case '{': // struct
            m_InStream.ignore();
            IVS_ERROR_IF(valueCount == IVS_STACK_SIZE, "struct/array stack overflow");
            valueStack.back()->setStruct();
            goto state_struct;

        case '[': // array
            m_InStream.ignore();
            IVS_ERROR_IF(valueCount == IVS_STACK_SIZE, "struct/array stack overflow");
            valueStack.back()->setArray();
            goto state_array_first_value;

        case '0': // hex, [u]int8/16/32/64, float, double
            m_InStream.ignore();
            goto state_starts_with_0;
        case '-': // int8/16/32/64, float, double
            goto state_number;
        case '"': // cstr
            m_InStream.ignore();
            goto state_string_literal;
        case '\'': // char
            m_InStream.ignore();
            goto state_char_literal;
        case 't': // bool
            m_InStream.ignore();
            IVS_READ_AND_EXPECTS('r');
            IVS_READ_AND_EXPECTS('u');
            IVS_READ_AND_EXPECTS('e');
            *valueStack.back() = true;
            IVS_BACK_AFTER_VALUE;
        case 'f': // bool
            m_InStream.ignore();
            IVS_READ_AND_EXPECTS('a');
            IVS_READ_AND_EXPECTS('l');
            IVS_READ_AND_EXPECTS('s');
            IVS_READ_AND_EXPECTS('e');
            *valueStack.back() = false;
            IVS_BACK_AFTER_VALUE;
        default: // [u]int8/16/32/64, float, double
            if (c > '0' && c <= '9')
            {
                goto state_number;
            }
            IVS_ERROR("invalid value formatting");
        }
    }

    /// ESCAPED CHAR
state_escaped:
    switch ((c = (char)m_InStream.get()))
    {
    case 'x':
    {
        char hx[3];
        hx[2] = 0;
        m_InStream.read(hx, 2); // read 'FF' for example
        IVS_ERROR_IF(sscanf(hx, "%2x", (unsigned int*)&escapedChar[escapedCount++]) != 1, "bad \\x formatting");
    }
    break;

    case 'u':
    {
        char ux[9];
        ux[8] = 0;
        IVS_ASSERT(escapedCount == 0);
        escapedCount = 0;
        while (((c = (char)m_InStream.peek()) != EOF) &&
               ((c >= '0' && c <= '9') || ((c | 0x20) >= 'a' && (c | 0x20) <= 'f')))
        {
            IVS_ERROR_IF(escapedCount == 8, "bad \\u formatting, expected 4 or 8 hex digits");
            ux[escapedCount++] = c;
            m_InStream.ignore();
        }
        IVS_ERROR_IF(escapedCount != 4 && escapedCount != 8, "bad \\u formatting, expected 4 or 8 hex digits");
        IVS_ERROR_IF(m_InStream.eof(), "unexcepted end of stream");
        if (escapedCount == 8)
        {
            IVS_ERROR_IF(sscanf(ux, "%8x", (unsigned int*)&escapedChar[0]) != 1, "bad \\u formatting");
        }
        else
        {
            IVS_ERROR_IF(sscanf(ux, "%4hx", (unsigned short*)&escapedChar[0]) != 1, "bad \\u formatting");
        }
        escapedCount >>= 1;
    }
    break;

    case '0':
        escapedChar[0] = '\0';
        break;

    case 'n':
        escapedChar[0] = '\n';
        break;

    case 't':
        escapedChar[0] = '\t';
        break;

    case 'r':
        escapedChar[0] = '\r';
        break;

    case 'b':
        escapedChar[0] = '\b';
        break;

    case 'a':
        escapedChar[0] = '\a';
        break;

    case 'v':
        escapedChar[0] = '\v';
        break;

    case 'f':
        escapedChar[0] = '\f';
        break;

    case '\\':
        escapedChar[0] = '\\';
        break;

    default:
        escapedChar[0] = c;
        break;
    }
    if (!escapedCount)
        escapedCount = 1; // default
    if (escaped_from_string)
        goto state_string_literal;
    goto state_char_literal;

    /// CHAR LITERAL
state_char_literal:
    if (escapedCount)
    {
        c = (char)m_InStream.get();
        IVS_ERROR_IF(c != '\'', "expects ' to end char literal");
        if (escapedCount == 1)
        {
            *valueStack.back() = escapedChar[0];
        }
        else if (escapedCount == 2)
        {
            *valueStack.back() = *(char16*)&escapedChar[0];
        }
        else if (escapedCount == 4)
        {
            *valueStack.back() = *(char32*)&escapedChar[0];
        }
        else
        {
            PHANTOM_ASSERT(false);
        }
        escapedCount = 0;
        token.clear();
        IVS_BACK_AFTER_VALUE;
    }
    else
    {
        while (((c = (char)m_InStream.get()) != EOF))
        {
            switch (c)
            {
            case '\\':
            {
                goto state_escaped;
            }
            case '\'':
            {
                IVS_ERROR_IF(token.empty(), "empty char literal");
                *valueStack.back() = token.front();
                token.clear();
                IVS_BACK_AFTER_VALUE;
            }
            default:
                token.push_back(c);
                break;
            }
        }
    }

    /// STRING LITERAL
state_string_literal:
    for (int i = 0; i < escapedCount; ++i)
        token.push_back(escapedChar[i]);
    escapedCount = 0;
    escaped_from_string = false;
    while (((c = (char)m_InStream.get()) != EOF))
    {
        switch (c)
        {
        case '\\':
        {
            escaped_from_string = true;
            goto state_escaped;
        }
        case '"':
        {
            if (valueStack.back()->isStruct()) // member reading
            {
                valueStack.push_back(&valueStack.back()->addValue(StringView(token.data(), token.size())));
                token.clear();
                goto state_struct_member_value;
            }
            else
            {
                *valueStack.back() = String(token.data(), token.size());
                token.clear();
                IVS_BACK_AFTER_VALUE;
            }
        }
        default:
            break;
        }
        token.push_back(c);
    }
    IVS_ON_EOF;

    /// HEXADECIMAL
state_hex:
    while (((c = (char)m_InStream.peek()) != EOF) &&
           ((c >= '0' && c <= '9') || ((c | 0x20) >= 'a' && (c | 0x20) <= 'f')))
    {
        token.push_back(c);
        m_InStream.ignore();
    }
    sscanf(token.data(), "%" PRIx64, (uint64*)&hex);
    *valueStack.back() = hex;
    token.clear();
    IVS_BACK_AFTER_VALUE;

    /// NUMBER
state_number:
    while (((c = (char)m_InStream.peek()) != EOF))
    {
        switch (c)
        {
        case 'i':
            if (token.size() == 1)
            {
                IVS_ERROR_IF(token.front() != '-' && token.front() != '+', "unexpected 'i'");
                IVS_READ_AND_EXPECTS('n');
                IVS_READ_AND_EXPECTS('f');
                if (token.front() == '-')
                {
                    *valueStack.back() = -std::numeric_limits<float>::infinity();
                    IVS_BACK_AFTER_VALUE;
                }
            }
            IVS_ERROR_IF(!token.empty(), "unexpected 'i'");
            IVS_READ_AND_EXPECTS('n');
            IVS_READ_AND_EXPECTS('f');
            *valueStack.back() = -std::numeric_limits<float>::infinity();
            IVS_BACK_AFTER_VALUE;

        case 'n':
            if (token.size() == 1)
            {
                IVS_ERROR_IF(token.front() != '-' && token.front() != '+', "unexpected 'i'");
            }
            else
            {
                IVS_ERROR_IF(!token.empty(), "unexpected 'i'");
            }
            IVS_READ_AND_EXPECTS('a');
            IVS_READ_AND_EXPECTS('n');
            *valueStack.back() = -std::numeric_limits<float>::quiet_NaN();
            IVS_BACK_AFTER_VALUE;

        case '.':
            token.push_back('.');
            m_InStream.ignore();
            goto state_floating;
        case 'u':
        case 'U':
        {
            PHANTOM_ASSERT(!token.empty(), "unexpected unsigned suffix");
            goto state_unsigned_int;
        }
        case 's':
        case 'S':
        {
            PHANTOM_ASSERT(!token.empty(), "unexpected signed suffix");
            goto state_signed_int;
        }
        case '+':
            IVS_ERROR_IF(!token.empty(), "unexpected '+'");
            break;
        case '-':
            IVS_ERROR_IF(!token.empty(), "unexpected '-'");
            break;
        default:
            IVS_ERROR_IF(c < '0' || c > '9', "expected digit");
            break;
        }
        token.push_back(c);
        m_InStream.ignore();
    }

#define IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR(txt)                                                                      \
    switch (m_InStream.peek())                                                                                         \
    {                                                                                                                  \
    IVS_CASE_VALUE_SEPARATOR:                                                                                          \
        break;                                                                                                         \
    default:                                                                                                           \
        IVS_ERROR(txt);                                                                                                \
    }

/// UNSIGNED
state_signed_int:
    IVS_ASSERT(m_InStream.peek() == 's' || m_InStream.peek() == 'S');
    m_InStream.ignore(); // ignore 's' or 'S'
    switch (c = (char)m_InStream.get())
    {
    case '8':
    {
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid signed int 8 formatting");
        int8 s8;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu8, &s8) != 1, "invalid signed int 8 formatting");
        token.clear();
        *valueStack.back() = s8;
        IVS_BACK_AFTER_VALUE;
    }
    case '1':
    {
        IVS_READ_AND_EXPECTS('6');
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid signed int 16 formatting");
        int16 s16;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu16, &s16) != 1, "invalid signed int 16 formatting");
        token.clear();
        *valueStack.back() = s16;
        IVS_BACK_AFTER_VALUE;
    }

    case '3':
    {
        IVS_READ_AND_EXPECTS('2');
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid signed int 32 formatting");
        int32 s32;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu32, &s32) != 1, "invalid signed int 32 formatting");
        token.clear();
        *valueStack.back() = s32;
        IVS_BACK_AFTER_VALUE;
    }
    case '6':
    {
        IVS_READ_AND_EXPECTS('4');
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid signed int 64 formatting");
        int64 s64;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu64, &s64) != 1, "invalid signed int 64 formatting");
        token.clear();
        *valueStack.back() = s64;
        IVS_BACK_AFTER_VALUE;
    }
    default:
        IVS_ERROR("missing size suffix (8|16|32|64) on signed int formatting");
    }

/// UNSIGNED
state_unsigned_int:
    IVS_ASSERT(m_InStream.peek() == 'u' || m_InStream.peek() == 'U');
    m_InStream.ignore(); // ignore 'u' or 'U'
    switch (c = (char)m_InStream.get())
    {
    case '8':
    {
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid unsigned int 8 formatting");
        uint8 u8;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu8, &u8) != 1, "invalid unsigned int 8 formatting");
        token.clear();
        *valueStack.back() = u8;
        IVS_BACK_AFTER_VALUE;
    }
    case '1':
    {
        IVS_READ_AND_EXPECTS('6');
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid unsigned int 16 formatting");
        uint16 u16;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu16, &u16) != 1, "invalid unsigned int 16 formatting");
        token.clear();
        *valueStack.back() = u16;
        IVS_BACK_AFTER_VALUE;
    }

    case '3':
    {
        IVS_READ_AND_EXPECTS('2');
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid unsigned int 32 formatting");
        uint32 u32;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu32, &u32) != 1, "invalid unsigned int 32 formatting");
        token.clear();
        *valueStack.back() = u32;
        IVS_BACK_AFTER_VALUE;
    }
    case '6':
    {
        IVS_READ_AND_EXPECTS('4');
        IVS_PEEK_AND_EXPECTS_VALUE_SEPARATOR("invalid unsigned int 64 formatting");
        uint64 u64;
        IVS_ERROR_IF(sscanf(token.data(), "%" PRIu64, &u64) != 1, "invalid unsigned int 64 formatting");
        token.clear();
        *valueStack.back() = u64;
        IVS_BACK_AFTER_VALUE;
    }
    default:
        IVS_ERROR("missing size suffix (8|16|32|64) on unsigned int formatting");
    }

    /// FLOATING POINT
state_floating:
    while (
    ((c = (char)m_InStream.peek()) != EOF) &&
    ((c >= '0' && c <= '9') || ((c | 0x20) == 'e') | ((c | 0x20) == 'f') | ((c | 0x20) == 'l') || c == '-' || c == '+'))
    {
        token.push_back(c);
        m_InStream.ignore();
    }
    if ((token.back() | 0x20) == 'f')
    {
        float f;
        IVS_ERROR_IF(sscanf(token.data(), "%f", &f) != 1, "invalid float formatting");
        *valueStack.back() = f;
    }
    else if ((token.back() | 0x20) == 'l')
    {
        double f;
        IVS_ERROR_IF(sscanf(token.data(), "%lf", &f) != 1, "invalid long double formatting");
        *valueStack.back() = (longdouble)f;
    }
    else
    {
        double d;
        IVS_ERROR_IF(sscanf(token.data(), "%lf", &d) != 1, "invalid double formatting");
        *valueStack.back() = d;
    }
    token.clear();
    IVS_BACK_AFTER_VALUE;

    /// STARTS WITH A '0'
state_starts_with_0:
    while ((c = (char)m_InStream.peek()) != EOF)
    {
        switch (c)
        {
        case 'x':
            m_InStream.ignore();
            goto state_hex;
        case '.':
            m_InStream.ignore();
            token.push_back('0');
            token.push_back('.');
            goto state_floating;
        default:
            token.push_back('0');
            goto state_number;
        }
    }
    IVS_ON_EOF;

    /// STRUCT
state_struct:
    IVS_IGNORE_WHITE_SPACES;
    if ((c = (char)m_InStream.peek()) == '}')
    {
        m_InStream.ignore();
        IVS_BACK_AFTER_VALUE;
    }
    else
    {
        goto state_struct_member;
    }

    /// STRUCT MEMBER
state_struct_member:
    IVS_IGNORE_WHITE_SPACES;
    IVS_READ_AND_EXPECTS('"');
    goto state_string_literal;

state_struct_member_value:
    IVS_IGNORE_WHITE_SPACES;
    IVS_READ_AND_EXPECTS(':');
    goto state_value;

    /// ARRAY
state_array_first_value:
    IVS_IGNORE_WHITE_SPACES;
    if ((c = (char)m_InStream.peek()) != EOF)
    {
        switch (c)
        {
        case ']':
            m_InStream.ignore();
            IVS_BACK_AFTER_VALUE;

        default:
            valueStack.push_back(&valueStack.back()->addValue());
            goto state_value;
        }
    }
    IVS_ON_EOF;

state_array_next_value:
    IVS_IGNORE_WHITE_SPACES;
    while ((c = (char)m_InStream.peek()) != EOF)
    {
        switch (c)
        {
        case ',':
            m_InStream.ignore();
            IVS_ERROR_IF(valueCount == IVS_STACK_SIZE, "struct/array stack overflow");
            valueStack.push_back(&valueStack.back()->addValue());
            goto state_value;
        case ']':
            m_InStream.ignore();
            IVS_BACK_AFTER_VALUE;

        default:
            IVS_ERROR("invalid array formatting");
        }
    }
    IVS_ON_EOF;
    return true;
}

IValueStream::IValueStream(std::istream& a_InStream) : m_InStream(a_InStream)
{
}

IValueStream::~IValueStream()
{
}

bool IValueStream::fail() const
{
    return m_InStream.fail() || error;
}

} // namespace phantom
