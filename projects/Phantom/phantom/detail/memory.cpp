// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* TODO LICENCE HERE */

#include "phantom/detail/core_internal.h"

#include <phantom/class>

namespace phantom
{
namespace
{
bool                              g_default_traits = false;
SmallVector<CustomAllocator, 32>& _CustomAllocatorStack()
{
    static SmallVector<CustomAllocator, 32> g_memory_traits(&CustomAllocator::Default());
    return g_memory_traits;
}
} // namespace

void CustomAllocator::Push(CustomAllocator a_Traits)
{
    PHANTOM_ASSERT(_CustomAllocatorStack().size() < 32, "maximum custom allocator stack reached");
    _CustomAllocatorStack().push_back(a_Traits);
}
void CustomAllocator::Pop()
{
    _CustomAllocatorStack().pop_back();
}

CustomAllocator const& CustomAllocator::CurrentOrDefault()
{
    return _CustomAllocatorStack().size() ? _CustomAllocatorStack().back() : Default();
}

CustomAllocator const& CustomAllocator::Current()
{
    PHANTOM_ASSERT(_CustomAllocatorStack().size(), "no allocator pushed, use 'CustomAllocator::Push'");
    return _CustomAllocatorStack().back();
}

static void* DefaultAllocFunc(size_t s, size_t)
{
    return std::malloc(s);
}

static void* DefaultReallocFunc(void* m, size_t s, size_t)
{
    return std::realloc(m, s);
}

static void DefaultDeallocFunc(void* m)
{
    std::free(m);
}

void CustomAllocator::Init()
{
    if (_CustomAllocatorStack().empty())
    {
        g_default_traits = true;
        Push(Default());
    }
}
void CustomAllocator::Release()
{
    if (g_default_traits)
    {
        PHANTOM_ASSERT(Current() == Default());
        _CustomAllocatorStack().pop_back();
        _CustomAllocatorStack().empty();
    }
    else
    {
        PHANTOM_ASSERT(_CustomAllocatorStack().empty());
    }
}

CustomAllocator const& CustomAllocator::Default()
{
    static CustomAllocator s_Default{DefaultAllocFunc, DefaultDeallocFunc, DefaultReallocFunc};
    return s_Default;
}

PHANTOM_EXPORT_PHANTOM void* allocate(size_t size, size_t align)
{
    PHANTOM_ASSERT(!_CustomAllocatorStack().empty());
    return CustomAllocator::Current().allocFunc(size, align);
}
PHANTOM_EXPORT_PHANTOM void deallocate(void* mem)
{
    PHANTOM_ASSERT(!_CustomAllocatorStack().empty(), "memory deallocated while custom allocator not defined. \n");
    CustomAllocator::Current().deallocFunc(mem);
}
PHANTOM_EXPORT_PHANTOM void* reallocate(void* mem, size_t size, size_t align)
{
    PHANTOM_ASSERT(!_CustomAllocatorStack().empty());
    return CustomAllocator::Current().reallocFunc(mem, size, align);
}

} // namespace phantom

// Memory operators overriding.
// [ALESS] TODO: to be removed as soon as the stl bindings are removed from phantom.

#if !defined(PHANTOM_STATIC_LINK_PHANTOM)

#    if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#        define PHANTOM_NEW_THROW
#    else
#        define PHANTOM_NEW_THROW throw(std::bad_alloc)
#    endif

void* operator new(size_t _size) PHANTOM_NEW_THROW
{
#    if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#        if !TYPE_REGISTRATION_KEY_DEBUG_ENABLED
    PHANTOM_LOG(Warning, "operator new has been called");
#        endif
#    endif
    return std::malloc(_size);
}

void* operator new[](size_t _size) PHANTOM_NEW_THROW
{
#    if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#        if !TYPE_REGISTRATION_KEY_DEBUG_ENABLED
    PHANTOM_LOG(Warning, "operator new has been called");
#        endif
#    endif
    return std::malloc(_size);
}

void operator delete(void* _ptr) throw()
{
    std::free(_ptr);
}

void operator delete[](void* _ptr) throw()
{
    std::free(_ptr);
}

void* operator new(size_t _size, const std::nothrow_t&) noexcept
{
#    if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#        if !TYPE_REGISTRATION_KEY_DEBUG_ENABLED
    PHANTOM_LOG(Warning, "operator new has been called");
#        endif
#    endif
    return std::malloc(_size);
}

void* operator new[](size_t _size, const std::nothrow_t&) noexcept
{
#    if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
#        if !TYPE_REGISTRATION_KEY_DEBUG_ENABLED
    PHANTOM_LOG(Warning, "operator new has been called");
#        endif
#    endif
    return std::malloc(_size);
}

void operator delete(void* _ptr, const std::nothrow_t&)noexcept
{
    std::free(_ptr);
}

void operator delete[](void* _ptr, const std::nothrow_t&) noexcept
{
    std::free(_ptr);
}

#endif
