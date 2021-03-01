#pragma once

#include <phantom/plugin.h>

namespace phantom
{
/// Heap that can only grows on allocation and do nothing on deallocation.
class PHANTOM_EXPORT_PHANTOM ForwardHeap
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
    void reset();

public: // MemoryHeap Related
    void* allocate(size_t _size, size_t _align);
    void  deallocate(void* _ptr);

private:
    size_t         m_heapSize;  /// Reserved heap size
    unsigned char* m_current{}; /// First free (usable but no necessary committed) bit  // TODO: Thread safety Atomic
    unsigned char* m_begin{};   /// First reserved byte
    unsigned char* m_end{};     /// Last (not included) byte
};

class PHANTOM_EXPORT_PHANTOM ForwardHeapSequence
{
public:
    ForwardHeapSequence(size_t a_HeapSize);

    void* allocate(size_t _s, size_t _a);
    void  deallocate(void* _ptr);

    void reset();
    void swap(ForwardHeapSequence& a_Other);

private:
    SmallVector<ForwardHeap, 32> m_Heaps;
    size_t                       m_HeapSize;
};

} // namespace phantom
