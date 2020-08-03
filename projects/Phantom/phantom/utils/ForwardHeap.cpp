#include "ForwardHeap.h"

#include <windows.h>

namespace phantom
{
static constexpr unsigned int HoDodoCaca = 0xCACAD0D0; // All the range committed
static constexpr unsigned int HoCafeDede = 0xDEDEFECA; // Data available flag
static constexpr unsigned int HoDeadBeef = 0xAFBEADDE; // Deallocated memory

ForwardHeap::ForwardHeap(size_t _heapSize) : m_heapSize(_heapSize)
{
    m_current = (unsigned char*)PHANTOM_MALLOC(m_heapSize);
    if (m_current == nullptr)
    {
        PHANTOM_LOG(Error, "We run out of Memory! Wait, who are you?");
        m_heapSize = 0;
        return;
    }
    m_begin = m_current;
    m_end = m_current + m_heapSize;
}

ForwardHeap::~ForwardHeap()
{
    PHANTOM_FREE(m_begin);
}

inline size_t GetAlignmentMask(size_t p_alignment)
{
    return p_alignment - 1;
}

template<typename T>
T AlignUp(T p_value, size_t p_alignment)
{
    size_t mask = GetAlignmentMask(p_alignment);
    return T((size_t(p_value) + mask) & ~mask);
}

void* ForwardHeap::allocate(size_t _size, size_t _align)
{
    void* ptr = m_current;
    ptr = AlignUp(ptr, _align);

    // Make sure we have enough space for this allocation
    if (((unsigned char*)ptr + _size + sizeof(void*)) > m_end)
    {
        return nullptr;
    }

    unsigned char* whereToStorePointer = reinterpret_cast<unsigned char*>(ptr) + _size;
    // push current alloc position at the end of allocation (kind of callstack way)
    m_current = whereToStorePointer + sizeof(void*);

    *reinterpret_cast<void**>(whereToStorePointer) = ptr;

    return ptr;
}

void ForwardHeap::deallocate(void* _ptr)
{
    if (m_current == m_begin)
        return;
    // we step back in the heap if we are deallocating the last allocation done
    void* last = *(reinterpret_cast<void**>(m_current) - 1);
    if (last == _ptr)
        m_current = reinterpret_cast<unsigned char*>(_ptr);
    /* Actually do nothing more */
}

void ForwardHeap::reset()
{
    m_current = m_begin;
}

ForwardHeapSequence::ForwardHeapSequence(size_t a_HeapSize) : m_HeapSize(a_HeapSize) {}

void ForwardHeapSequence::swap(ForwardHeapSequence& a_Other)
{
    size_t size = m_HeapSize;
    m_HeapSize = a_Other.m_HeapSize;
    a_Other.m_HeapSize = size;
    m_Heaps.swap(a_Other.m_Heaps);
}

void* ForwardHeapSequence::allocate(size_t _s, size_t _a)
{
    PHANTOM_ASSERT((_s % _a) == 0);
    void* ptr;
    for (auto& heap : m_Heaps)
    {
        if ((ptr = heap.allocate(_s, _a)))
            return ptr;
    }
    m_Heaps.emplace_back(std::max(_s + _a + sizeof(void*), m_HeapSize));
    return m_Heaps.back().allocate(_s, _a);
}

struct FriendHeap
{
    size_t         m_heapSize;  /// Reserved heap size
    unsigned char* m_current{}; /// First free (usable but no necessary committed) bit  // TODO: Thread safety Atomic
    unsigned char* m_begin{};   /// First reserved byte
    unsigned char* m_end{};     /// Last (not included) byte
};

void ForwardHeapSequence::deallocate(void* _ptr)
{
    bool inOneOfHeap = false;
    for (auto& heap : m_Heaps)
    {
        FriendHeap& fh = (FriendHeap&)heap;
        if (fh.m_begin <= _ptr && _ptr < fh.m_end)
        {
            heap.deallocate(_ptr);
            return;
        }
    }
    PHANTOM_ASSERT(inOneOfHeap);
    FriendHeap& fh = (FriendHeap&)m_Heaps.back();
    if (fh.m_begin <= _ptr && _ptr < fh.m_end)
        m_Heaps.back().deallocate(_ptr);
}

void ForwardHeapSequence::reset()
{
    m_Heaps.clear();
}

} // namespace phantom
