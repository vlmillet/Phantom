// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <inttypes.h>
#include <phantom/detail/phantom.h>
#include <phantom/utils/String.h>
#include <string>

/// @cond ADVANCED

#pragma warning(disable : 4996)

namespace phantom
{
namespace detail
{
template<typename t_Target, typename t_Source>
struct LexicalCastH;
/*{
        static t_Target apply(t_Source const& src)
        {

        }
};*/

template<>
struct LexicalCastH<int32_t, StringView>
{
    static int apply(StringView src)
    {
        int32_t val;
        PHANTOM_VERIFY(sscanf(src.data(), "%d", &val) != EOF);
        return val;
    }
};

template<>
struct LexicalCastH<uint32_t, StringView>
{
    static uint32_t apply(StringView src)
    {
        uint32_t val;
        PHANTOM_VERIFY(sscanf(src.data(), "%u", &val) != EOF);
        return val;
    }
};

template<>
struct LexicalCastH<int64_t, StringView>
{
    static int64_t apply(StringView src)
    {
        int64_t val;
        PHANTOM_VERIFY(sscanf(src.data(), "%" PRIi64, &val) != EOF);
        return val;
    }
};

template<>
struct LexicalCastH<uint64_t, StringView>
{
    static uint64_t apply(StringView src)
    {
        uint64_t val;
        PHANTOM_VERIFY(sscanf(src.data(), "%" PRIu64, &val) != EOF);
        return val;
    }
};

#if PHANTOM_INT64_IS_LONGLONG
template<>
struct LexicalCastH<long, StringView>
{
    static long apply(StringView src)
    {
        long val;
        PHANTOM_VERIFY(sscanf(src.data(), "%ld", &val) != EOF);
        return val;
    }
};
#else
template<>
struct LexicalCastH<long long, StringView>
{
    static long apply(StringView src)
    {
        long val;
        PHANTOM_VERIFY(sscanf(src.data(), "%" PRIi64, &val) != EOF);
        return val;
    }
};

#endif

#if PHANTOM_INT64_IS_LONGLONG
template<>
struct LexicalCastH<unsigned long, StringView>
{
    static unsigned long apply(StringView src)
    {
        unsigned long val;
        PHANTOM_VERIFY(sscanf(src.data(), "%ud", &val) != EOF);
        return val;
    }
};
#else
template<>
struct LexicalCastH<unsigned long long, StringView>
{
    static long apply(StringView src)
    {
        long val;
        PHANTOM_VERIFY(sscanf(src.data(), "%" PRIu64, &val) != EOF);
        return val;
    }
};

#endif

template<class t_Target>
struct LexicalCastH<t_Target*, StringView>
{
    static t_Target* apply(StringView src)
    {
        long long val;
        PHANTOM_VERIFY(sscanf(src.data(), "0x%p", &val) != EOF);
        return val;
    }
};

template<class t_Target>
struct LexicalCastH<t_Target const*, StringView>
{
    static t_Target* apply(StringView src)
    {
        long long val;
        PHANTOM_VERIFY(sscanf(src.data(), "0x%p", &val) != EOF);
        return val;
    }
};

template<class t_Source>
struct LexicalCastH<String, t_Source*>
{
    static String apply(t_Source* src)
    {
        char buf[19];
        PHANTOM_VERIFY(sprintf(buf, "0x%p", src) != EOF);
        return buf;
    }
};

template<class t_Source>
struct LexicalCastH<String, t_Source const*>
{
    static String apply(t_Source const* src)
    {
        char buf[19];
        PHANTOM_VERIFY(sprintf(buf, "0x%p", src) != EOF);
        return buf;
    }
};

template<>
struct LexicalCastH<float, StringView>
{
    static float apply(StringView src)
    {
        static float zero = 0.f;
        if (src == "-1.#IND")
            return -zero / zero;
        else if (src == "1.#IND")
            return zero / zero;
        else if (src == "-1.#INF")
            return -1.f / zero;
        else if (src == "1.#INF")
            return 1.f / zero;
        else
        {
            float val;
            PHANTOM_VERIFY(sscanf(src.data(), "%f", &val) != EOF);
            return val;
        }
    }
};

template<>
struct LexicalCastH<double, StringView>
{
    static double apply(StringView src)
    {
        static double zero = 0;
        if (src == "-1.#IND")
            return -zero / zero;
        else if (src == "1.#IND")
            return zero / zero;
        else if (src == "-1.#INF")
            return -1. / zero;
        else if (src == "1.#INF")
            return 1. / zero;
        else
        {
            double val;
            PHANTOM_VERIFY(sscanf(src.data(), "%lf", &val) != EOF);
            return val;
        }
    }
};

template<>
struct LexicalCastH<long double, StringView>
{
    static long double apply(StringView src)
    {
        static long double zero = 0;
        if (src == "-1.#IND")
            return -zero / zero;
        else if (src == "1.#IND")
            return zero / zero;
        else if (src == "-1.#INF")
            return -1. / zero;
        else if (src == "1.#INF")
            return 1. / zero;
        else
        {
            bool _assert_no_warn = sizeof(long double) == sizeof(double);
            (void)_assert_no_warn;
            PHANTOM_ASSERT(_assert_no_warn, "long double not handled by sscanf, too big on this platform");
            double val;
            PHANTOM_VERIFY(sscanf(src.data(), "%lf", &val) != EOF);
            return (long double)val;
        }
    }
};

template<class t_Target>
struct LexicalCastH<t_Target, String> : LexicalCastH<t_Target, StringView>
{
};

template<class t_Source>
struct LexicalCastH<String, t_Source>
{
    static String apply(t_Source const& src)
    {
        return String(std::to_string(src));
    }
};
} // namespace detail

template<typename t_Target, typename t_Source>
inline t_Target lexical_cast(t_Source arg)
{
    return detail::LexicalCastH<t_Target, t_Source>::apply(arg);
}

// long long
template<>
inline long long lexical_cast<long long, StringView>(StringView arg)
{
    return (long long)lexical_cast<long double>(arg);
}

template<>
inline unsigned long long lexical_cast<unsigned long long, StringView>(StringView arg)
{
    return (unsigned long long)lexical_cast<long double>(arg);
}

// long
template<>
inline long lexical_cast<long, StringView>(StringView arg)
{
    return (long)lexical_cast<float>(arg);
}

template<>
inline unsigned long lexical_cast<unsigned long, StringView>(StringView arg)
{
    return (unsigned long)lexical_cast<double>(arg);
}

// int
template<>
inline int lexical_cast<int, StringView>(StringView arg)
{
    return (int)lexical_cast<double>(arg);
}

template<>
inline unsigned int lexical_cast<unsigned int, StringView>(StringView arg)
{
    return (unsigned int)lexical_cast<double>(arg);
}

// short
template<>
inline short lexical_cast<short, StringView>(StringView arg)
{
    return (short)lexical_cast<float>(arg);
}

template<>
inline unsigned short lexical_cast<unsigned short, StringView>(StringView arg)
{
    return (unsigned short)lexical_cast<float>(arg);
}

// char
template<>
inline char lexical_cast<char, StringView>(StringView arg)
{
    return (char)lexical_cast<int>(arg);
}

template<>
inline String lexical_cast<String, char>(char arg)
{
    return lexical_cast<String>((int)arg);
}

// char
template<>
inline bool lexical_cast<bool, StringView>(StringView arg)
{
    return lexical_cast<int>(arg) != 0;
}

template<>
inline String lexical_cast<String, bool>(bool arg)
{
    return lexical_cast<String>((int)arg);
}

template<>
inline unsigned char lexical_cast<unsigned char, StringView>(StringView arg)
{
    return (unsigned char)lexical_cast<unsigned int>(arg);
}

template<>
inline String lexical_cast<String, unsigned char>(unsigned char arg)
{
    return lexical_cast<String>((unsigned int)arg);
}

template<>
inline std::nullptr_t lexical_cast<std::nullptr_t, StringView>(StringView arg)
{
    if (arg != "nullptr")
    {
        PHANTOM_LOG(Error, "parsing nullptr value failed");
    }
    return nullptr;
}

template<>
inline String lexical_cast<String, std::nullptr_t>(std::nullptr_t arg)
{
    if (arg != nullptr)
    {
        PHANTOM_LOG(Error, "parsing nullptr value failed");
    }
    return "nullptr";
}

template<>
inline void* lexical_cast<void*, StringView>(StringView arg)
{
    void* ptr;
    PHANTOM_VERIFY(1 == sscanf(arg.data(), "0x%p", &ptr));
    return ptr;
}

typedef void* msvc_bug_lexical_cast_void_ptr; // WTF ??? why do i need this typedef to make the
                                              // specialization work ? o‘
template<>
inline String lexical_cast<String, msvc_bug_lexical_cast_void_ptr>(msvc_bug_lexical_cast_void_ptr arg)
{
    char buffer[19] = "";
    sprintf(buffer, "0x%p", (void*)arg);
    return buffer;
}
} // namespace phantom

/// @endcond

#pragma warning(default : 4996)
