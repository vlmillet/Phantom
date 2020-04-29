#include "ForwardHeap.h"

#include <windows.h>

namespace phantom
{
static constexpr unsigned int HoDodoCaca = 0xCACAD0D0; // All the range committed
static constexpr unsigned int HoCafeDede = 0xDEDEFECA; // Data available flag
static constexpr unsigned int HoDeadBeef = 0xAFBEADDE; // Deallocated memory

ForwardHeap::ForwardHeap(size_t _heapSize)
    : m_heapSize(_heapSize), m_current(0), m_begin(0), m_end(0), m_releaseVM(false)
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

void* ForwardHeap::allocate(size_t _size)
{
    void* ptr = m_current;
    ptr = AlignUp(ptr, 8);

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

void ForwardHeap::Reset()
{
    m_current = m_begin;
}

} // namespace phantom
