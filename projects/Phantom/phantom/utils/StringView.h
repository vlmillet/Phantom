// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <algorithm>
#include <phantom/detail/core.h>
#include <string.h>

namespace phantom
{
#define PHANTOM_STRING_AS_PRINTF_ARG(sv) int(sv.size()), sv.data()

template<class CharT>
class BasicStringView;

template<class CharT>
struct NullTerminatedBasicStringView
{
    static const char null = 0;
    NullTerminatedBasicStringView(BasicStringView<CharT> a_Str) : nullTerminated(a_Str)
    {
        if (*a_Str.end())
            notNullTerminated = a_Str;
    }
                           operator const char*() { return notNullTerminated.size() ? notNullTerminated.c_str() : nullTerminated.begin(); }
    BasicStringView<CharT> nullTerminated;
    SmallString<CharT, 32> notNullTerminated;
};

template<class CharT>
class BasicStringView
{
    using _Zero = void****;

public:
    static const size_t npos = size_t(-1);

    typedef BasicStringView<CharT>               SelfType;
    typedef NullTerminatedBasicStringView<CharT> NullTerminatedType;

    BasicStringView(const CharT* a_Str) : m_pBegin(a_Str), m_pEnd(a_Str + strlen(a_Str)) {}
    BasicStringView(const CharT* a_First, const CharT* a_Last) : m_pBegin(a_First), m_pEnd(a_Last) {}
    BasicStringView(const CharT* a_Str, size_t a_Len) : m_pBegin(a_Str), m_pEnd(a_Str + a_Len) {}
    BasicStringView() : m_pBegin(nullptr), m_pEnd(nullptr) {}

    // avoid attempt to initialize/test by error a StringView with a nullptr (when replacing
    // existing const char* with their zero initializations)
    BasicStringView(_Zero) = delete;
    SelfType& operator=(_Zero) = delete;
    bool      operator==(_Zero) = delete;
    bool      operator!=(_Zero) = delete;

    void clear()
    {
        m_pBegin = nullptr;
        m_pEnd = nullptr;
    }

    SelfType substr(size_t a_Start, size_t a_Off = npos)
    {
        PHANTOM_ASSERT(a_Start <= size());
        PHANTOM_ASSERT(a_Off == npos || (a_Start + a_Off) <= size());
        return SelfType(m_pBegin + a_Start, a_Off == npos ? size() - a_Start : a_Off);
    }

    CharT dropBack()
    {
        PHANTOM_ASSERT(size());
        return *--m_pEnd;
    }

    void dropBack(size_t N)
    {
        if (N >= size())
            m_pBegin = m_pEnd;
        else
            m_pEnd -= N;
    }

    CharT dropFront()
    {
        PHANTOM_ASSERT(size());
        return *m_pBegin++;
    }

    void dropFront(size_t N)
    {
        if (N >= size())
            m_pBegin = m_pEnd;
        else
            m_pBegin += N;
    }

    bool startsWith(SelfType a_Str) const
    {
        return (a_Str.size() <= size()) && memcmp(a_Str.begin(), begin(), a_Str.size()) == 0;
    }

    const CharT& operator[](size_t Idx) const { return *(begin() + Idx); }
    const CharT* data() const { return m_pBegin; }
    const CharT* begin() const { return m_pBegin; }
    const CharT* end() const { return m_pEnd; }
    size_t       size() const { return static_cast<size_t>(m_pEnd - m_pBegin); }
    bool         empty() const { return m_pBegin == m_pEnd; }

    NullTerminatedType nullTerminated() const { return *this; }

    CharT front() const
    {
        PHANTOM_ASSERT(!empty());
        return *m_pBegin;
    }
    CharT back() const
    {
        PHANTOM_ASSERT(!empty());
        return *(m_pEnd - 1);
    }

    size_t find(CharT a_Ch) const { return find_first_of(a_Ch); }

    size_t find(CharT const* a_pWhat, size_t a_Off = 0) const
    {
        PHANTOM_ASSERT(a_Off < size());
        CharT const* first = data() + a_Off;
        CharT const* it = strstr(first, a_pWhat);
        return it ? it - begin() : npos;
    }

    size_t find(SelfType a_What, size_t a_Off = 0) const
    {
        if (size() - a_Off < a_What.size())
            return npos;
        SelfType _this(data() + a_Off, size() - a_Off - a_What.size() + 1);
        for (size_t i = 0; i < _this.size(); ++i)
        {
            if (_this.startsWith(a_What))
                return a_Off + i;
            _this.dropFront();
        }
        return npos;
    }

    template<size_t S>
    size_t find(SmallString<CharT, S> const& a_What, size_t a_Off = 0) const
    {
        return find(a_What.data(), a_Off);
    }

    size_t find_first_of(CharT ch, size_t a_Off = 0) const
    {
        size_t count = size();
        for (size_t i = a_Off; i < count; ++i)
        {
            if (m_pBegin[i] == ch)
                return i;
        }
        return npos;
    }
    size_t find_first_of(CharT const* chs, size_t a_Off = 0) const
    {
        size_t count = size();
        size_t len = strlen(chs);
        for (size_t i = a_Off; i < count; ++i)
        {
            for (size_t j = 0; j < len; ++j)
                if (m_pBegin[i] == chs[j])
                    return i;
        }
        return npos;
    }
    size_t find_last_of(CharT ch, size_t a_Off = 0) const
    {
        size_t i = size();
        while (--i != a_Off)
        {
            if (m_pBegin[i] == ch)
                return i;
        }
        return npos;
    }
    size_t find_last_of(CharT const* chs, size_t a_Off = 0) const
    {
        size_t i = size();
        size_t len = strlen(chs);
        while (--i != a_Off)
        {
            for (size_t j = 0; j < len; ++j)
                if (m_pBegin[i] == chs[j])
                    return i;
        }
        return npos;
    }

    size_t find_first_not_of(CharT ch, size_t a_Off = 0) const
    {
        size_t count = size();
        for (size_t i = a_Off; i < count; ++i)
        {
            if (m_pBegin[i] != ch)
                return i;
        }
        return npos;
    }
    size_t find_first_not_of(CharT const* chs, size_t a_Off = 0) const
    {
        size_t count = size();
        size_t len = strlen(chs);
        for (size_t i = a_Off; i < count; ++i)
        {
            bool found = false;
            for (size_t j = 0; j < len; ++j)
            {
                if (m_pBegin[i] == chs[j])
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                return i;
        }
        return npos;
    }
    size_t find_last_not_of(CharT ch, size_t a_Off = npos) const
    {
        size_t i = size();
        while (--i != a_Off)
        {
            if (m_pBegin[i] != ch)
                return i;
        }
        return npos;
    }
    size_t find_last_not_of(CharT const* chs, size_t a_Off = npos) const
    {
        size_t i = size();
        size_t len = strlen(chs);
        while (--i != a_Off)
        {
            bool found = false;
            for (size_t j = 0; j < len; ++j)
            {
                if (m_pBegin[i] == chs[j])
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                return i;
        }
        return npos;
    }

    /// @brief Compare strings
    /// @return
    ///   - 0  : They compare equal
    ///   - <0 : Either the value of the first character that does not match is lower in the
    ///   compared string, or all compared characters match but the compared string is shorter.
    ///   - >0 : Either the value of the first character that does not match is greater in the
    ///   compared string, or all compared characters match but the compared string is longer.
    int compare(SelfType _other) const
    {
#pragma push_macro("min")
#undef min
        int const comp = strncmp(m_pBegin, _other.m_pBegin, std::min(size(), _other.size()));
#pragma pop_macro("min")
        if (comp == 0)
        {
            if (size() == _other.size())
                return 0;
            else
                return size() < _other.size() ? -1 : 1;
        }
        else
            return comp;
    }

private:
    const CharT* m_pBegin;
    const CharT* m_pEnd;
};

inline bool operator==(BasicStringView<char> a_Left, BasicStringView<char> a_Right)
{
    return a_Left.compare(a_Right) == 0;
}

inline bool operator!=(BasicStringView<char> a_Left, BasicStringView<char> a_Right)
{
    return a_Left.compare(a_Right) != 0;
}

inline bool operator<(BasicStringView<char> a_Left, BasicStringView<char> a_Right)
{
    return a_Left.compare(a_Right) < 0;
}

inline bool operator>(BasicStringView<char> a_Left, BasicStringView<char> a_Right)
{
    return a_Left.compare(a_Right) > 0;
}

inline bool operator<=(BasicStringView<char> a_Left, BasicStringView<char> a_Right)
{
    return a_Left.compare(a_Right) <= 0;
}

inline bool operator>=(BasicStringView<char> a_Left, BasicStringView<char> a_Right)
{
    return a_Left.compare(a_Right) >= 0;
}

PHANTOM_FORCEINLINE const char* cstr(StringView str)
{
    return str.data();
}

template<class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& a_Out,
                                                     phantom::BasicStringView<CharT>    a_Str)
{
    return a_Out.write(a_Str.data(), a_Str.size());
}

} // namespace phantom
