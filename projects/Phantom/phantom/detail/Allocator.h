// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;

#include <phantom/detail/core.h>

namespace phantom
{
/// @cond ADVANCED

/**
 * \struct DefaultAllocator
 *
 * \brief Default allocator implementation
 */

template<typename t_Ty>
struct DefaultAllocator
{
    PHANTOM_REBIND(DefaultAllocator);
    PHANTOM_FORCEINLINE static t_Ty* allocate()
    {
        return reinterpret_cast<t_Ty*>(
        phantom::allocate(sizeof(t_Ty), PHANTOM_ALIGNOF(t_Ty)));
    }
    PHANTOM_FORCEINLINE static void deallocate(void* a_pPtr)
    {
        phantom::deallocate(a_pPtr);
    }
    PHANTOM_FORCEINLINE static t_Ty* allocate(size_t a_Count)
    {
        return reinterpret_cast<t_Ty*>(
        phantom::allocate(sizeof(t_Ty) * a_Count, PHANTOM_ALIGNOF(t_Ty)));
    }
    PHANTOM_FORCEINLINE static void deallocate(void* ptr, size_t)
    {
        return phantom::deallocate(ptr);
    }
};

template<>
struct DefaultAllocator<void>
{
    PHANTOM_FORCEINLINE static void* allocate()
    {
        PHANTOM_ASSERT(false, "cannot allocate void");
        return nullptr;
    }
    PHANTOM_FORCEINLINE static void deallocate(void*)
    {
        PHANTOM_ASSERT(false, "cannot deallocate void");
    }
    PHANTOM_FORCEINLINE static void* allocate(size_t)
    {
        PHANTOM_ASSERT(false, "cannot allocate void");
        return nullptr;
    }
    PHANTOM_FORCEINLINE static void deallocate(void*, size_t)
    {
        PHANTOM_ASSERT(false, "cannot deallocate void");
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct Allocator
///
/// \brief  Allocator used by ClassT::newInstance essentially.
///
////////////////////////////////////////////////////////////////////////////////////////////////////

/// @endcond

template<typename t_Ty>
struct Allocator : public DefaultAllocator<t_Ty>
{
    PHANTOM_REBIND(Allocator);
};

} // namespace phantom
