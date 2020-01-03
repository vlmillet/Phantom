// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "ArrayView.h"

#include <algorithm>
#include <phantom/alignof>
#include <phantom/detail/allocate.h>
#include <phantom/detail/phantom.h>
#include <phantom/traits/CopyTraits.h>
#include <phantom/traits/MoveTraits.h>

namespace phantom
{
namespace detail
{
HAUNT_PAUSE;
template<class T, bool is_fundamental>
struct SmallVectorH
{
    typedef T                       value_type;
    PHANTOM_FORCEINLINE static void cdef(value_type* out)
    {
        *out = value_type();
    }
    PHANTOM_FORCEINLINE static void cmove(value_type* out, value_type&& rval)
    {
        *out = rval;
    }
    PHANTOM_FORCEINLINE static void ccopy(value_type* out, const value_type& lval)
    {
        *out = lval;
    }
    PHANTOM_FORCEINLINE static void destroy(value_type*)
    {
    }
    PHANTOM_FORCEINLINE static void destroy(value_type*, value_type*)
    {
    }
};

template<class T>
struct SmallVectorH<T, false>
{
    typedef T          value_type;
    inline static void cdef(value_type* out)
    {
        new (out) value_type();
    }
    inline static void cmove(value_type* out, value_type&& rval)
    {
        new (out) value_type((value_type &&) rval);
    }
    inline static void ccopy(value_type* out, const value_type& lval)
    {
        new (out) value_type(lval);
    }
    inline static void destroy(value_type* v)
    {
        v->~value_type();
    }
    static void destroy(value_type* it, value_type* end)
    {
        for (; it != end; ++it)
            it->~value_type();
    }
};
HAUNT_RESUME;
} // namespace detail

template<class T, size_t StaticAllocSize, size_t DynamicAllocInc>
class SmallVector
{
    PHANTOM_STATIC_ASSERT(DynamicAllocInc, "increment size must be non-zero");
#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
    PHANTOM_STATIC_ASSERT(StaticAllocSize < 65535, "safety check : base capacity must be inferior to 65535");
#endif

    typedef phantom::detail::SmallVectorH<T, std::is_fundamental<T>::value> Helper;
    template<class, size_t, size_t>
    friend class SmallVector;

public:
    typedef SmallVector<T, StaticAllocSize, DynamicAllocInc> SelfType;

    // typedefs for stl vector compliance
    typedef size_t                                size_type;
    typedef T                                     value_type;
    typedef value_type const*                     const_iterator;
    typedef value_type*                           iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef ArrayView<value_type>                 ArrayViewType;

    SmallVector() = default;
    explicit SmallVector(size_t a_Count)
    {
        resize(a_Count);
    };
    explicit SmallVector(CustomAllocator const* a_pMemTraits) : m_pMemTraits(a_pMemTraits)
    {
        PHANTOM_ASSERT(m_pMemTraits);
    };
    SmallVector(std::initializer_list<value_type> const& inits)
    {
        for (auto const& v : inits)
        {
            push_back(v);
        }
    }
    SmallVector(ArrayViewType inits)
    {
        for (auto const& v : inits)
        {
            push_back(v);
        }
    }

    SmallVector(const SelfType& other)
    {
        reserve(other.m_size);
        m_pMemTraits = other.m_pMemTraits;
        m_size = other.m_size;
        auto d = m_data;
        auto od = other.m_data;
        for (size_t i = 0; i < m_size; ++i)
        {
            Helper::ccopy(&d[i], od[i]);
        }
    }

    SmallVector(iterator _it, iterator _end) : SmallVector((const_iterator)_it, (const_iterator)_end)
    {
    }

    SmallVector(const_iterator _it, const_iterator _end)
    {
        size_t capacity = std::distance(_it, _end);
        reserve(capacity);
        auto d = m_data;
        for (; _it != _end; ++_it)
        {
            Helper::ccopy(&d[m_size++], *_it);
        }
    }

    template<typename It>
    SmallVector(It _it, It _end) : m_size(0), m_capacity(StaticAllocSize)
    {
        for (; _it != _end; ++_it)
            push_back(*_it);
    }

    SmallVector(SelfType&& temp)
    {
        // other is dynamic
        if (temp.m_capacity > StaticAllocSize)
        {
            if (m_pMemTraits == temp.m_pMemTraits) // same traits => classical move semantic
            {
                m_size = temp.m_size;
                m_capacity = temp.m_capacity;
                m_data = temp.m_data;
                temp.m_data = (value_type*)&temp.m_staticData;
                temp.m_size = 0;
                temp.m_capacity = StaticAllocSize;
            }
            else // different traits => fallback to simple copy
            {
                *this = temp;
            }
        }
        else // other is static
        {
            m_size = temp.m_size;
            //@ m_capacity = StaticAllocSize;
            auto        thisData = m_data;
            value_type* tempData = temp.m_data;
            size_t      i = temp.m_size;
            while (i--)
            {
                Helper::cmove(thisData, std::move(*tempData));
                Helper::destroy(tempData);
                thisData++;
                tempData++;
            }
            temp.m_size = 0;
        }
    }

    ~SmallVector()
    {
        Helper::destroy(m_data, m_data + m_size);
        if (m_capacity != StaticAllocSize)
        {
            _dealloc(m_data);
        }
    }

    SelfType& operator=(const SelfType& other)
    {
        reserve(other.m_capacity);
        auto d = m_data;
        auto od = other.m_data;
        Helper::destroy(begin(), end());
        m_size = other.m_size;
        for (size_t i = 0; i < m_size; ++i)
        {
            Helper::ccopy(d + i, od[i]); // replace by other content
        }
        return *this;
    }

    SelfType& operator=(SelfType&& temp)
    {
        if (m_capacity > StaticAllocSize) // this dynamic
        {
            if (temp.m_capacity == StaticAllocSize) // other static
            {
                for (size_t i = 0; i < m_size; ++i) // destroy this content
                    Helper::destroy(m_data + i);
                _dealloc(m_data); // deallocate dynamic buffer
                m_data = (value_type*)&m_staticData;
                m_size = temp.m_size;
                m_capacity = StaticAllocSize;
                for (size_t i = 0; i < m_size; ++i)
                {
                    Helper::cmove(m_data + i,
                                  (value_type &&) temp.m_data[i]); // replace by other content
                    Helper::destroy(&(temp.m_data[i]));
                }
            }
            else // other dynamic
            {
                if (m_pMemTraits == temp.m_pMemTraits) // same traits => classical move semantic
                {
                    for (size_t i = 0; i < m_size; ++i) // destroy this content
                        Helper::destroy(&m_data[i]);
                    _dealloc(m_data); // deallocate dynamic buffer
                    m_size = temp.m_size;
                    m_capacity = temp.m_capacity;
                    m_data = temp.m_data;
                    temp.m_data = (value_type*)&temp.m_staticData;
                    temp.m_capacity = StaticAllocSize;
                }
                else // different traits => fallback to simple copy
                {
                    return *this = temp;
                }
            }
        }
        else // this static
        {
            if (temp.m_capacity == StaticAllocSize) // other static
            {
                for (size_t i = 0; i < m_size; ++i)
                    Helper::destroy(&m_data[i]);

                m_size = temp.m_size;
                for (size_t i = 0; i < m_size; ++i)
                {
                    Helper::ccopy(&m_data[i], temp.m_data[i]);
                    Helper::destroy(&(temp.m_data[i]));
                }
            }
            else // other dynamic
            {
                if (m_pMemTraits == temp.m_pMemTraits) // same traits => classical move semantic
                {
                    for (size_t i = 0; i < m_size; ++i)
                        Helper::destroy(&m_data[i]);

                    m_size = temp.m_size;
                    m_capacity = temp.m_capacity;
                    m_data = temp.m_data;
                    temp.m_data = (value_type*)&temp.m_staticData;
                    temp.m_capacity = StaticAllocSize;
                }
                else // different traits => fallback to simple copy
                {
                    return *this = temp;
                }
            }
        }
        temp.m_size = 0;
        return *this;
    }

    template<size_t S, size_t D>
    SelfType& operator=(const SmallVector<T, S, D>& other)
    {
        reserve(other.m_capacity);
        auto d = m_data;
        auto od = other.m_data;
        Helper::destroy(begin(), end());
        m_size = other.m_size;
        for (size_t i = 0; i < m_size; ++i)
        {
            Helper::ccopy(d + i, od[i]); // replace by other content
        }
        return *this;
    }

    // TODO : implement operator=(SmallVector<T, S, D>&& temp)

    value_type* data()
    {
        return m_data;
    }
    value_type const* data() const
    {
        return m_data;
    }

    const_iterator begin() const
    {
        return m_data;
    }
    const_iterator end() const
    {
        return m_data + m_size;
    }

    iterator begin()
    {
        return m_data;
    }
    iterator end()
    {
        return m_data + m_size;
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

    value_type& operator[](size_type i)
    {
        PHANTOM_ASSERT(i < size());
        return m_data[i];
    }

    value_type const& operator[](size_type i) const
    {
        PHANTOM_ASSERT(i < size());
        return m_data[i];
    }

    size_type size() const
    {
        return m_size;
    }
    size_type capacity() const
    {
        return m_capacity;
    }
    bool empty() const
    {
        return m_size == 0;
    }
    value_type const& front() const
    {
        PHANTOM_ASSERT(size());
        return m_data[0];
    }
    value_type const& back() const
    {
        PHANTOM_ASSERT(size());
        return m_data[m_size - 1];
    }
    value_type& front()
    {
        PHANTOM_ASSERT(size());
        return m_data[0];
    }
    value_type& back()
    {
        PHANTOM_ASSERT(size());
        return m_data[m_size - 1];
    }
    void pop_back()
    {
        Helper::destroy(&back());
        m_size--;
    }

    void reserve(size_t newCapacity)
    {
        if (m_capacity < newCapacity)
        {
            if (m_capacity == StaticAllocSize) // we were using static buffer
            {
                value_type* pNewData = _alloc(newCapacity);
                value_type* pCurrNewData = pNewData;
                size_t      i = m_size;
                while (i--) // copy content to dynamic buffer
                {
                    Helper::cmove(pCurrNewData++, std::move(*m_data));
                    Helper::destroy(m_data++);
                }
                m_data = pNewData;
            }
            else
            {
                value_type* pOldData = m_data;
                value_type* pNewData = _alloc(newCapacity);
                value_type* pCurrNewData = pNewData;
                size_t      i = m_size;
                while (i--) // copy content to dynamic buffer
                {
                    Helper::cmove(pCurrNewData++, std::move(*m_data));
                    Helper::destroy(m_data++);
                }
                _dealloc(pOldData);
                m_data = pNewData;
            }
            m_capacity = newCapacity;
        }
    }
    void resize(size_t newSize)
    {
        reserve(newSize);
        auto d = m_data;
        for (; m_size < newSize; ++m_size)
            Helper::cdef(&d[m_size]);
        m_size = newSize;
    }

    void resize(size_t newSize, const value_type& init)
    {
        reserve(newSize);
        auto d = m_data;
        for (; m_size < newSize; ++m_size)
            Helper::ccopy(&d[m_size], init);
        m_size = newSize;
    }

    void resize(size_t newSize, value_type&& init)
    {
        reserve(newSize);
        auto d = m_data;
        for (; m_size < newSize; ++m_size)
            Helper::cmove(&d[m_size], (value_type &&) init);
        m_size = newSize;
    }

    void push_back(const value_type& val)
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity + DynamicAllocInc);
        }
        Helper::ccopy(&m_data[m_size++], val);
    }

    iterator erase(const_iterator _it)
    {
        iterator it = (iterator)_it;
        auto     d = m_data;
        auto     e = end();
        PHANTOM_ASSERT(it >= d && it < e);
        Helper::destroy(it);
        auto _end = it + 1;
        if (_end != e)
        {
            for (auto src = _end; src != e; ++src)
            {
                auto dst = src - 1;
                Helper::cmove(dst, (value_type &&) * src);
                Helper::destroy(src);
            }
        }
        m_size--;
        return it;
    }

    iterator erase_unsorted(const_iterator _it)
    {
        iterator it = (iterator)_it;
        *it = (value_type &&) back();
        pop_back();
        return it + 1;
    }

    iterator erase_unsorted(const_iterator _it, const_iterator _last)
    {
        if (_it == _last)
            return (iterator)_it;
        iterator it = (iterator)_it;
        iterator last = (iterator)_last;
        PHANTOM_ASSERT(it < last);
        PHANTOM_ASSERT(it >= begin() && it < end());
        PHANTOM_ASSERT(last > begin() && last <= end());
        size_t countToErase = last - it;
        auto   e = end();
        for (; last < e; ++last)
        {
            *last = *it++;
        }
        resize(size() - countToErase);
        return (iterator)_last;
    }

    iterator erase(const_iterator _it, const_iterator _last)
    {
        if (_it == _last)
            return (iterator)_it;
        iterator it = (iterator)_it;
        iterator last = (iterator)_last;
        auto     d = m_data;
        auto     e = end();
        PHANTOM_ASSERT(it >= d && last <= e && last > it);
        Helper::destroy(it, last);
        size_t count = std::distance(it, last);
        if (last != e)
        {
            for (auto src = last; src != e; ++src)
            {
                auto dst = src - count;
                Helper::cmove(dst, (value_type &&) * src);
                Helper::destroy(src);
            }
        }
        m_size -= count;
        return (iterator)_it;
    }

    iterator insert(iterator _where, iterator _it, iterator _end)
    {
        return insert((iterator)_where, (const_iterator)_it, (const_iterator)_end);
    }

    iterator insert(const_iterator _where, iterator _it, iterator _end)
    {
        return insert((iterator)_where, (const_iterator)_it, (const_iterator)_end);
    }

    iterator insert(const_iterator _where, const_iterator _it, const_iterator _end)
    {
        return insert((iterator)_where, _it, _end);
    }

    iterator insert(iterator _where, const_iterator _it, const_iterator _end)
    {
        PHANTOM_ASSERT(_where >= m_data && _where <= end());
        if (_it == _end)
            return _where;
        size_t index = _where - begin();
        size_t count = std::distance(_it, _end);
        reserve(m_size + count);
        _where = begin() + index;
        auto d = m_data;
        // we need to move 'count' elements from where to where+count
        size_t countToMove = end() - _where;
        while (countToMove--)
        {
            auto src = _where + countToMove;
            auto dst = src + count;
            Helper::cmove(dst, (value_type &&) * src);
            Helper::destroy(src);
        }
        for (; _it != _end; ++_it)
        {
            Helper::ccopy(&d[index++], *_it);
        }
        m_size += count;
        return begin() + index;
    }

    template<typename It>
    iterator insert(const_iterator _where, It _it, It _end)
    {
        return insert((iterator)_where, _it, _end);
    }

    template<typename It>
    iterator insert(iterator _where, It _it, It _end)
    {
        PHANTOM_ASSERT(_where >= m_data && _where <= end());
        if (_it == _end)
            return _where;
        size_t insertIndex = _where - begin();
        for (; _it != _end; ++_it)
        {
            if (m_size == m_capacity)
            {
                size_t cur = _where - begin();
                reserve(m_capacity + DynamicAllocInc);
                _where = begin() + cur;
            }
            auto lastToMove = end() - 1;
            auto firstToMove = _where;
            for (; lastToMove >= firstToMove; --lastToMove)
            {
                auto src = lastToMove;
                auto dst = lastToMove + 1;
                Helper::cmove(dst, (value_type &&) * src);
                Helper::destroy(src);
            }
            Helper::ccopy(_where++, *_it);
            m_size++;
        }
        return begin() + insertIndex;
    }

    iterator insert(iterator _where, const value_type& val)
    {
        PHANTOM_ASSERT(_where >= m_data && _where <= end());
        size_type index = _where - begin();
        if (_where == end())
        {
            push_back(val);
        }
        else
        {
            if (m_size == m_capacity)
            {
                size_t cur = _where - begin();
                reserve(m_capacity + DynamicAllocInc);
                _where = begin() + cur;
            }
            size_t countToMove = end() - _where;
            while (countToMove--)
            {
                auto src = _where + countToMove;
                auto dst = src + 1;
                Helper::cmove(dst, (value_type &&) * src);
                Helper::destroy(src);
            }
            Helper::ccopy(_where, val);
            m_size++;
        }
        return begin() + index;
    }

    operator ArrayViewType() const
    {
        return ArrayViewType(m_data, m_size);
    }

    iterator insert(iterator _where, value_type&& val)
    {
        PHANTOM_ASSERT(_where >= m_data && _where <= end());
        size_type index = _where - begin();
        if (_where == end())
            push_back((value_type &&) val);
        else
        {
            if (m_size == m_capacity)
            {
                size_t cur = _where - begin();
                reserve(m_capacity + DynamicAllocInc);
                _where = begin() + cur;
            }
            size_t countToMove = end() - _where;
            while (countToMove--)
            {
                auto src = _where + countToMove;
                auto dst = src + 1;
                Helper::cmove(dst, (value_type &&) * src);
                Helper::destroy(src);
            }
            Helper::cmove(_where, (value_type &&) val);
            m_size++;
        }
        return begin() + index;
    }

    void push_back(value_type&& val)
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity + DynamicAllocInc);
        }
        Helper::cmove(&m_data[m_size++], (value_type &&) val);
    }

    template<class... Args>
    void emplace_back(Args&&... args)
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity + DynamicAllocInc);
        }
        new (&m_data[m_size++]) value_type(std::forward<Args>(args)...);
    }

    void clear()
    {
        Helper::destroy(begin(), end());
        m_size = 0;
    }

    void swap(SelfType& other)
    {
        size_t s = m_size;
        size_t os = other.m_size;
        size_t c = m_capacity;
        size_t oc = other.m_capacity;
        auto*  d = m_data;
        auto*  od = other.m_data;
        if (oc == StaticAllocSize && c != StaticAllocSize)
        {
            other.swap(*this);
            return;
        }
        if (c == StaticAllocSize && oc == StaticAllocSize)
        {
            if (s > os)
            {
                other.swap(*this);
                return;
            }
            for (size_t i = 0; i < s; ++i)
            {
                value_type v = d[i];
                value_type ov = od[i];
                d[i] = (value_type &&) ov;
                od[i] = (value_type &&) v;
            }
            for (size_t i = s; i < os; ++i)
            {
                push_back(od[i]);
            }
            for (size_t i = s; i < os; ++i)
            {
                other.pop_back();
            }
            PHANTOM_ASSERT(m_size == os);
            PHANTOM_ASSERT(other.m_size == s);
        }
        else
        {
            if (c != StaticAllocSize && oc != StaticAllocSize)
            {
                m_data = od;
                other.m_data = d;
            }
            else // c == StaticAllocSize && oc != StaticAllocSize
            {
                // destroy this content and transfer it to 'other' static data
                other.m_data = (value_type*)&other.m_staticData;
                auto* osd = other.m_data;
                m_data = od;
                for (size_t i = 0; i < s; ++i)
                {
                    new (&osd[i]) value_type((value_type &&) d[i]);
                    Helper::destroy(&d[i]);
                }
                // transfer other dynamic to this dynamic
            }
            m_size = os;
            other.m_size = s;
            m_capacity = oc;
            other.m_capacity = c;
        }
    }

private:
    value_type* _alloc(size_t s)
    {
        return reinterpret_cast<value_type*>(
        m_pMemTraits->allocFunc(s * sizeof(value_type), PHANTOM_ALIGNOF(value_type), __FILE__, __LINE__));
    }
    void _dealloc(value_type* t)
    {
        m_pMemTraits->deallocFunc(t);
    }

private:
    size_type   m_capacity = StaticAllocSize;
    size_type   m_size = 0;
    value_type* m_data = (value_type*)&m_staticData;
    byte m_staticData[(StaticAllocSize == 0) | (sizeof(value_type) * StaticAllocSize)]; // trick to avoid 0-size array
    CustomAllocator const* m_pMemTraits = &CustomAllocator::CurrentOrDefault();
};
} // namespace phantom

#include <phantom/traits/CopyTraits.h>
#include <phantom/traits/MoveTraits.h>

PHANTOM_DISABLE_TRAIT_IF_T(IsCopyAssignable, (class, size_t, size_t), (T, S, D), SmallVector,
                           !IsCopyAssignable<T>::value);
PHANTOM_DISABLE_TRAIT_IF_T(IsMoveAssignable, (class, size_t, size_t), (T, S, D), SmallVector,
                           !IsMoveAssignable<T>::value);
PHANTOM_DISABLE_TRAIT_IF_T(IsCopyConstructible, (class, size_t, size_t), (T, S, D), SmallVector,
                           !IsCopyConstructible<T>::value);
PHANTOM_DISABLE_TRAIT_IF_T(IsMoveConstructible, (class, size_t, size_t), (T, S, D), SmallVector,
                           !IsMoveConstructible<T>::value);
