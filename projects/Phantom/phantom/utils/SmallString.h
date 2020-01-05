// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "SmallVector.h"
#include "StringView.h"

#include <phantom/detail/core.h>

namespace phantom
{
#if defined(_PHANTOM_MEASURE_AVERAGE_STRING_SIZE)
PHANTOM_EXPORT_PHANTOM extern size_t SmallStringTotalSize;
PHANTOM_EXPORT_PHANTOM extern size_t SmallStringTotalCount;
#endif

template<class T, size_t StaticAllocSize, size_t DynamicAllocInc>
class SmallString
{
    template<class, size_t, size_t>
    friend class SmallString;

public:
    const static size_t                                      npos = size_t(-1);
    typedef BasicStringView<T>                               StringViewType;
    typedef size_t                                           size_type;
    typedef SmallString<T, StaticAllocSize, DynamicAllocInc> SelfType;
    typedef SmallVector<T, StaticAllocSize, DynamicAllocInc> ContainerType;
    typedef T                                                value_type;
    typedef T                                                CharT;
    typedef value_type const*                                const_iterator;
    typedef value_type*                                      iterator;
    typedef std::reverse_iterator<const_iterator>            const_reverse_iterator;
    typedef std::reverse_iterator<iterator>                  reverse_iterator;

    SmallString()
    {
        m_Chars.push_back(0);
    }
    SmallString(SelfType const& a_Other) = default;
    SmallString(SelfType&& a_Temp) : m_Chars((ContainerType &&) a_Temp.m_Chars)
    {
        a_Temp._addNull();
    }
    SmallString(std::initializer_list<CharT> a_IL) : m_Chars(a_IL)
    {
#if defined(_PHANTOM_MEASURE_AVERAGE_STRING_SIZE)
        SmallStringTotalSize += a_IL.size();
        SmallStringTotalCount++;
#endif
        _addNull();
    }
    explicit SmallString(CharT c)
    {
        m_Chars.push_back(c);
        _addNull();
    }

    SmallString(CharT const* a_pCstr, size_t a_Len) : m_Chars(a_pCstr, a_pCstr + a_Len)
    {
#if defined(_PHANTOM_MEASURE_AVERAGE_STRING_SIZE)
        SmallStringTotalSize += a_Len;
        SmallStringTotalCount++;
#endif
        _addNull();
    }
    SmallString(StringViewType a_View) : SmallString(a_View.data(), a_View.size())
    {
    }
    SmallString(CharT const* a_pCstr) : SmallString(a_pCstr, strlen(a_pCstr))
    {
    }

    template<size_t S, size_t D>
    explicit SmallString(SmallString<T, S, D> const& a_Other) : SmallString(a_Other.data(), a_Other.size())
    {
    }

    template<class It>
    SmallString(It a_First, It a_Last) : m_Chars(a_First, a_Last)
    {
        _addNull();
    }

    explicit SmallString(std::string const& a_StdString) : SmallString(a_StdString.c_str(), a_StdString.size())
    {
    }

    ~SmallString()
    {
    }

    SelfType& operator=(SelfType const& a_Other) = default;

    template<size_t S, size_t D>
    SelfType& operator=(SmallString<char, S, D> const& a_Other)
    {
        m_Chars = a_Other.m_Chars;
        return *this;
    }

    SelfType& operator=(SelfType&& a_Other)
    {
        m_Chars = std::move(a_Other.m_Chars);
        a_Other._addNull();
        return *this;
    }

    SelfType& operator=(CharT const* a_Other)
    {
        return assign(a_Other);
    }

    SelfType& operator=(CharT a_Ch)
    {
        return assign(a_Ch);
    }

    SelfType& operator=(std::string const& a_Other)
    {
        assign(a_Other.c_str(), a_Other.size());
        return *this;
    }

    explicit operator std::string() const
    {
        return std::string(c_str(), size());
    }

    operator StringViewType() const
    {
        return StringViewType(c_str(), size());
    }

    CharT const* begin() const
    {
        return m_Chars.begin();
    }
    CharT const* end() const
    {
        return m_Chars.end() - 1;
    }
    CharT* begin()
    {
        return m_Chars.begin();
    }
    CharT* end()
    {
        return m_Chars.end() - 1;
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }
    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }
    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    size_t size() const
    {
        return m_Chars.size() - 1;
    }
    size_t length() const
    {
        return size();
    }

    void resize(size_t s)
    {
        _beginChange();
        m_Chars.resize(s);
        _endChange();
    }
    void resize(size_t s, CharT c)
    {
        _beginChange();
        m_Chars.resize(s, c);
        _endChange();
    }

    size_t capacity() const
    {
        return m_Chars.capacity() - 1;
    }

    void reserve(size_t s)
    {
        m_Chars.reserve(s + 1);
    }

    void clear()
    {
        _beginChange();
        m_Chars.clear();
        _endChange();
    }

    bool empty() const
    {
        return size() == 0;
    }

    CharT const& operator[](size_t i) const
    {
        return m_Chars[i];
    }
    CharT& operator[](size_t i)
    {
        return m_Chars[i];
    }

    CharT const& back() const
    {
        return m_Chars[size() - 1];
    }
    CharT& back()
    {
        return m_Chars[size() - 1];
    }

    CharT const& front() const
    {
        return m_Chars.front();
    }
    CharT& front()
    {
        return m_Chars.front();
    }

    SelfType& operator+=(CharT const* a_pCstr)
    {
        return append(a_pCstr);
    }
    SelfType& operator+=(SelfType const& a_Other)
    {
        return append(a_Other);
    }
    SelfType& operator+=(CharT c)
    {
        return append(c);
    }
    SelfType& operator+=(std::string const& c)
    {
        return append(c.data(), c.size());
    }
    template<size_t S, size_t D>
    SelfType& operator+=(SmallString<CharT, S, D> const& c)
    {
        return append(c.data(), c.size());
    }
    SelfType& operator+=(StringViewType v)
    {
        return append(v.data(), v.size());
    }

    SelfType operator+(CharT const* a_pCstr) const
    {
        SelfType s = *this;
        s += a_pCstr;
        return s;
    }
    SelfType operator+(SelfType const& a_Other) const
    {
        SelfType s = *this;
        s += a_Other;
        return s;
    }
    SelfType operator+(CharT c) const
    {
        SelfType s = *this;
        s += c;
        return s;
    }
    SelfType operator+(std::string const& a_StdStr) const
    {
        SelfType s = *this;
        s += a_StdStr;
        return s;
    }
    SelfType operator+(StringViewType v) const
    {
        SelfType s = *this;
        s += v;
        return s;
    }

    SelfType& append(CharT const* a_pCstr)
    {
        _beginChange();
        m_Chars.insert(m_Chars.end(), a_pCstr, a_pCstr + strlen(a_pCstr));
        _endChange();
        return *this;
    }
    SelfType& append(SelfType const& a_Other)
    {
        _beginChange();
        m_Chars.insert(m_Chars.end(), a_Other.begin(), a_Other.end());
        _endChange();
        return *this;
    }
    SelfType& append(CharT c)
    {
        m_Chars.back() = c;
        _addNull();
        return *this;
    }
    void push_back(CharT c)
    {
        m_Chars.back() = c;
        _addNull();
    }

    SelfType& assign(CharT a_Ch)
    {
        m_Chars.resize(2);
        m_Chars[0] = a_Ch;
        m_Chars[1] = 0;
        return *this;
    }

    template<class It>
    SelfType& assign(It a_First, It a_Last)
    {
        m_Chars.clear();
        m_Chars.insert(m_Chars.end(), a_First, a_Last);
        _addNull();
        return *this;
    }

    template<class It>
    SelfType& append(It a_First, It a_Last)
    {
        _beginChange();
        m_Chars.insert(m_Chars.end(), a_First, a_Last);
        _endChange();
        return *this;
    }

    SelfType& assign(CharT const* a_pCstr)
    {
        return assign(a_pCstr, strlen(a_pCstr));
    }

    SelfType& assign(CharT const* a_pCstr, size_t a_Len)
    {
        _beginChange();
        m_Chars.clear();
        m_Chars.insert(m_Chars.end(), a_pCstr, a_pCstr + a_Len);
        _endChange();
        return *this;
    }

    SelfType& append(CharT const* a_pCstr, size_t a_Len)
    {
        _beginChange();
        m_Chars.insert(m_Chars.end(), a_pCstr, a_pCstr + a_Len);
        _endChange();
        return *this;
    }

    SelfType& assign(std::initializer_list<CharT> a_IL)
    {
        return (assign(a_IL.begin(), a_IL.size()));
    }

    SelfType& append(std::initializer_list<CharT> a_IL)
    {
        return (append(a_IL.begin(), a_IL.size()));
    }

    SelfType& append(const SelfType& str, size_t subpos, size_t sublen)
    {
        return append(str.data() + subpos, sublen);
    }

    SelfType& insert(size_t a_Pos, SelfType const& a_Str)
    {
        return insert(a_Pos, a_Str.data(), a_Str.size());
    }

    SelfType& insert(size_t a_Pos, CharT const* a_Cstr, size_t a_Len)
    {
        _beginChange();
        m_Chars.insert(begin() + a_Pos, a_Cstr, a_Cstr + a_Len);
        _endChange();
        return *this;
    }

    iterator insert(const_iterator a_Where, std::initializer_list<CharT> a_IL)
    {
        _beginChange();
        auto it = m_Chars.insert(a_Where, a_IL.begin(), a_IL.end());
        _endChange();
        return it;
    }

    size_t erase(size_t pos)
    {
        iterator it = m_Chars.erase(begin() + pos);
        return it - begin();
    }

    iterator erase(iterator a_Where)
    {
        return m_Chars.erase(a_Where);
    }
    iterator erase(iterator a_First, iterator a_Last)
    {
        return m_Chars.erase(a_First, a_Last);
    }

    void swap(SelfType& a_Other)
    {
        m_Chars.swap(a_Other.m_Chars);
    }

    void pop_back()
    {
        _beginChange();
        m_Chars.pop_back();
        _endChange();
    }

    CharT const* c_str() const
    {
        return m_Chars.data();
    }
    CharT const* data() const
    {
        return m_Chars.data();
    }
    CharT* data()
    {
        return m_Chars.data();
    }

    size_t find(CharT a_Ch) const
    {
        return find_first_of(a_Ch);
    }

    size_t find(CharT const* a_pWhat, size_t a_Off = 0) const
    {
        PHANTOM_ASSERT(a_Off < size());
        CharT const* first = data() + a_Off;
        CharT const* it = strstr(first, a_pWhat);
        return it ? it - begin() : npos;
    }

    size_t find(SelfType const& a_What, size_t a_Off = 0) const
    {
        return find(StringViewType(a_What.data(), a_What.size()), a_Off);
    }

    size_t find(std::string const& a_What, size_t a_Off = 0) const
    {
        return find(StringViewType(a_What.data(), a_What.size()), a_Off);
    }

    size_t find(StringViewType a_What, size_t a_Off = 0) const
    {
        if (size() - a_Off < a_What.size())
            return npos;
        StringViewType _this(data() + a_Off, size() - a_Off);
        size_t         sz = _this.size();
        for (size_t i = 0; i < sz; ++i)
        {
            if (_this.startsWith(a_What))
                return a_Off + i;
            _this.dropFront();
        }
        return npos;
    }

    template<size_t S, size_t D>
    size_t find(SmallString<char, S, D> const& a_What, size_t a_Off = 0) const
    {
        return find(a_What.data(), a_Off);
    }

    size_t find_first_of(CharT ch, size_t a_Off = 0) const
    {
        return StringViewType(*this).find_first_of(ch, a_Off);
    }
    size_t find_first_of(CharT const* chs, size_t a_Off = 0) const
    {
        return StringViewType(*this).find_first_of(chs, a_Off);
    }
    size_t find_last_of(CharT ch, size_t a_Off = 0) const
    {
        return StringViewType(*this).find_last_of(ch, a_Off);
    }
    size_t find_last_of(CharT const* chs, size_t a_Off = 0) const
    {
        return StringViewType(*this).find_last_of(chs, a_Off);
    }

    size_t find_first_not_of(CharT ch, size_t a_Off = 0) const
    {
        return StringViewType(*this).find_first_not_of(ch, a_Off);
    }
    size_t find_first_not_of(CharT const* chs, size_t a_Off = 0) const
    {
        return StringViewType(*this).find_first_not_of(chs, a_Off);
    }
    size_t find_last_not_of(CharT ch, size_t a_Off = npos) const
    {
        return StringViewType(*this).find_last_not_of(ch, a_Off);
    }
    size_t find_last_not_of(CharT const* chs, size_t a_Off = npos) const
    {
        return StringViewType(*this).find_last_not_of(chs, a_Off);
    }

    SelfType substr(size_t a_Start, size_t a_Offset = npos) const
    {
        return SelfType(begin() + a_Start, a_Offset == npos ? size() - a_Start : a_Offset);
    }

    StringViewType view() const
    {
        return StringViewType(begin(), end());
    }

    template<size_t S, size_t D>
    int compare(SmallString<char, S, D> const& a_Other) const
    {
        return strcmp(data(), a_Other.data());
    }

    int compare(SelfType const& a_Other) const
    {
        return strcmp(data(), a_Other.data());
    }

    int compare(std::string const& a_Other) const
    {
        return strcmp(data(), a_Other.data());
    }

    int compare(CharT const* a_Cstr) const
    {
        return strcmp(data(), a_Cstr);
    }

    int compare(size_t start, size_t len, CharT const* a_Cstr) const
    {
        return strncmp(a_Cstr, data() + start, len);
    }

    int compare(size_t start, size_t len, SelfType const& a_Other) const
    {
        return strncmp(a_Other.data(), data() + start, len);
    }

    bool operator<(SelfType const& a_Other) const
    {
        return compare(a_Other) < 0;
    }
    bool operator>(SelfType const& a_Other) const
    {
        return compare(a_Other) > 0;
    }

    bool operator==(SelfType const& a_Other) const
    {
        return compare(a_Other) == 0;
    }
    bool operator!=(SelfType const& a_Other) const
    {
        return compare(a_Other) != 0;
    }

    bool operator==(CharT const* a_Cstr) const
    {
        return compare(a_Cstr) == 0;
    }
    bool operator!=(CharT const* a_Cstr) const
    {
        return compare(a_Cstr) != 0;
    }

    bool operator==(StringViewType a_Other) const
    {
        return compare(a_Other) == 0;
    }
    bool operator!=(StringViewType a_Other) const
    {
        return compare(a_Other) != 0;
    }

    template<size_t S, size_t D>
    bool operator==(SmallString<char, S, D> const& a_Other) const
    {
        return compare(a_Other) == 0;
    }

    template<size_t S, size_t D>
    bool operator!=(SmallString<char, S, D> const& a_Other) const
    {
        return compare(a_Other) != 0;
    }

    bool operator==(std::string const& a_Other) const
    {
        return compare(a_Other) == 0;
    }
    bool operator!=(std::string const& a_Other) const
    {
        return compare(a_Other) != 0;
    }

private:
    PHANTOM_FORCEINLINE void _addNull()
    {
        m_Chars.push_back(0);
    }
    PHANTOM_FORCEINLINE void _beginChange()
    {
        PHANTOM_ASSERT(!m_Chars.empty() && m_Chars.back() == 0);
        m_Chars.pop_back();
    }
    PHANTOM_FORCEINLINE void _endChange()
    {
        _addNull();
    }

private:
    ContainerType m_Chars;
};

template<class CharT, size_t S, size_t D>
SmallString<CharT, S, D> operator+(CharT const* a_pCstr, SmallString<CharT, S, D> const& a_Other)
{
    return SmallString<CharT, S, D>(a_pCstr) + a_Other;
}

template<class CharT, size_t S, size_t D>
SmallString<CharT, S, D> operator+(std::string const& a_StdStr, SmallString<CharT, S, D> const& a_Other)
{
    return SmallString<CharT, S, D>(a_StdStr) + a_Other;
}

template<class CharT, size_t S, size_t D>
SmallString<CharT, S, D> operator+(CharT a_Ch, SmallString<CharT, S, D> const& a_Other)
{
    return SmallString<CharT, S, D>(a_Ch) + a_Other;
}

template<class CharT, size_t S, size_t D>
SmallString<CharT, S, D> operator+(BasicStringView<CharT> a_View, SmallString<CharT, S, D> const& a_Other)
{
    return SmallString<CharT, S, D>(a_View) + a_Other;
}

template<class CharT, size_t S, size_t D>
bool operator==(CharT const* a_pCstr, SmallString<CharT, S, D> const& a_Other)
{
    return a_Other.operator==(a_pCstr);
}

template<class CharT, size_t S, size_t D>
bool operator==(std::string const& a_StdStr, SmallString<CharT, S, D> const& a_Other)
{
    return a_Other.operator==(a_StdStr);
}

template<class CharT, size_t S, size_t D>
bool operator!=(CharT const* a_pCstr, SmallString<CharT, S, D> const& a_Other)
{
    return a_Other.operator!=(a_pCstr);
}

template<class CharT, size_t S, size_t D>
bool operator!=(std::string const& a_StdStr, SmallString<CharT, S, D> const& a_Other)
{
    return a_Other.operator!=(a_StdStr);
}

template<class CharT, size_t S, size_t D>
bool operator!=(BasicStringView<CharT> a_View, SmallString<CharT, S, D> const& a_Other)
{
    return a_Other.operator!=(a_View);
}

template<class CharT>
SmallString<CharT, PHANTOM_STRING_STATIC_SIZE, PHANTOM_STRING_EXPAND_SIZE> operator+(BasicStringView<CharT> v0,
                                                                                     BasicStringView<CharT> v1)
{
    SmallString<CharT, PHANTOM_STRING_STATIC_SIZE, PHANTOM_STRING_EXPAND_SIZE> s;
    s.reserve(v0.size() + v1.size());
    s.append(v0);
    s.append(v1);
    return s;
}

template<class CharT>
SmallString<CharT, PHANTOM_STRING_STATIC_SIZE, PHANTOM_STRING_EXPAND_SIZE> operator+(BasicStringView<CharT> v, CharT c0)
{
    SmallString<CharT, PHANTOM_STRING_STATIC_SIZE, PHANTOM_STRING_EXPAND_SIZE> s;
    s.reserve(v.size() + 1);
    s.append(v);
    s.append(c0);
    return s;
}

template<class CharT>
SmallString<CharT, PHANTOM_STRING_STATIC_SIZE, PHANTOM_STRING_EXPAND_SIZE> operator+(CharT c0, BasicStringView<CharT> v)
{
    SmallString<CharT, PHANTOM_STRING_STATIC_SIZE, PHANTOM_STRING_EXPAND_SIZE> s;
    s.reserve(v.size() + 1);
    s.append(c0);
    s.append(v);
    return s;
}

template<class CharT, class Traits, size_t S, size_t D>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>&       a_Out,
                                                     phantom::SmallString<CharT, S, D> const& a_Str)
{
    return a_Out.write(a_Str.data(), a_Str.size());
}

} // namespace phantom
