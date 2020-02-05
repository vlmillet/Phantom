#pragma once

namespace phantom
{
template<class T, class Mutex>
struct ThreadSafeArrayView
{
    using const_iterator = T const*;

    ThreadSafeArrayView(T const* a_Data, size_t a_Size, Mutex& a_Mutex)
        : m_Data(a_Data), m_Size(a_Size), m_Mutex(a_Mutex)
    {
        a_Mutex.lock();
    }
    ~ThreadSafeArrayView()
    {
        if (!m_bMoved)
            m_Mutex.unlock();
    }
    ThreadSafeArrayView(ThreadSafeArrayView&& a_Tmp) : m_Data(a_Tmp.m_Data), m_Mutex(a_Tmp.m_Mutex), m_bMoved(true) {}
    ThreadSafeArrayView(ThreadSafeArrayView const&) = delete;
    ThreadSafeArrayView& operator=(ThreadSafeArrayView const&) = delete;
    ThreadSafeArrayView& operator=(ThreadSafeArrayView&&) = delete;

    T const* data() const { return m_Data; }
    size_t   size() const { return m_Size; }
    bool     empty() const { return m_Size == 0; }
    T const* begin() const { return m_Data; }
    T const* end() const { return m_Data + m_Size; }

private:
    T const* m_Data;
    size_t   m_Size;
    Mutex&   m_Mutex;
    bool     m_bMoved = false;
};

template<class Container, class Mutex>
ThreadSafeArrayView<Container, Mutex> MakeThreadSafeContainerReadOnlyAccess(Container const& _container, Mutex& _mutex)
{
    return ThreadSafeArrayView<typename Container::value_type, Mutex>(_container.data(), _container.size(), _mutex);
}

template<class T, class Mutex>
ThreadSafeArrayView<T, Mutex> MakeThreadSafeArrayView(T const* a_pData, size_t a_Size, Mutex& _mutex)
{
    return ThreadSafeArrayView<T, Mutex>(a_pData, a_Size, _mutex);
}
} // namespace phantom
