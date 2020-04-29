#pragma once

namespace phantom
{
/// Heap that can only grows on allocation and do nothing on deallocation.
class ForwardHeap
{
public:
    ForwardHeap(size_t _heapSize);
    ForwardHeap& operator=(ForwardHeap const&)
    {
        PHANTOM_ASSERT(false);
        return *this;
    }
    ForwardHeap(ForwardHeap const&) { PHANTOM_ASSERT(false); }
    ForwardHeap& operator=(ForwardHeap&&)
    {
        PHANTOM_ASSERT(false);
        return *this;
    }
    ForwardHeap(ForwardHeap&& _temp)
    {
        memcpy(this, &_temp, sizeof(ForwardHeap));
        memset(&_temp, 0, sizeof(ForwardHeap));
    }

    ~ForwardHeap();

public: // ForwardHeap Specific
    /// Reset the memory and deallocate pages
    void Reset();

public: // MemoryHeap Related
    void* allocate(size_t _size);
    void  deallocate(void* _ptr);

private:
    size_t         m_heapSize; /// Reserved heap size
    unsigned char* m_current;  /// First free (usable but no necessary committed) bit  // TODO: Thread safety Atomic
    unsigned char* m_begin;    /// First reserved byte
    unsigned char* m_end;      /// Last (not included) byte
    unsigned char* m_endOfLastCommittedPage; /// First byte not committed yet
    bool           m_releaseVM;              /// True to release the reserved vm space.
};

class ForwardHeapSequence
{
public:
    ForwardHeapSequence(size_t a_HeapSize) : m_HeapSize(a_HeapSize) { m_Heaps.emplace_back(a_HeapSize); }

    void* allocate(size_t _s)
    {
        if (void* ptr = m_Heaps.back().allocate(_s))
            return ptr;
        m_Heaps.emplace_back(std::max(_s + (_s + sizeof(void*)) % m_HeapSize, m_HeapSize));
        return allocate(_s);
    }
    void deallocate(void* _ptr) { m_Heaps.back().deallocate(_ptr); }

    void Reset() { m_Heaps.clear(); }

private:
    SmallVector<ForwardHeap, 32> m_Heaps;
    size_t                       m_HeapSize;
};

} // namespace phantom
