// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/Allocator.h>

namespace phantom
{
#define PHANTOM_ALLOCATE(...) ::phantom::allocate<__VA_ARGS__>(PHANTOM_MEMORY_STAT_INSERT_VALUES)
#define PHANTOM_ALLOCATE_N(n, ...) ::phantom::allocate<__VA_ARGS__>(n PHANTOM_MEMORY_STAT_APPEND_VALUES)
#define PHANTOM_REALLOCATE_N(ptr, n, ...) ::phantom::reallocate<__VA_ARGS__>(ptr, n PHANTOM_MEMORY_STAT_APPEND_VALUES)
#define PHANTOM_DEALLOCATE(ptr, ...) ::phantom::deallocate<__VA_ARGS__>(ptr)
#define PHANTOM_DEALLOCATE_N(ptr, n, ...) ::phantom::deallocate<__VA_ARGS__>(ptr, n)

template<typename t_Ty>
inline t_Ty* allocate(PHANTOM_MEMORY_STAT_INSERT_PARAMS)
{
    return ::phantom::Allocator<t_Ty>::allocate(PHANTOM_MEMORY_STAT_INSERT_ARGS);
}

template<typename t_Ty>
inline void deallocate(t_Ty* ptr)
{
    return ::phantom::Allocator<t_Ty>::deallocate(ptr);
}

template<typename t_Ty>
t_Ty* allocate(size_t n PHANTOM_MEMORY_STAT_APPEND_PARAMS)
{
    return ::phantom::Allocator<t_Ty>::allocate(n PHANTOM_MEMORY_STAT_APPEND_ARGS);
}

template<typename t_Ty>
t_Ty* reallocate(t_Ty* ptr, size_t n PHANTOM_MEMORY_STAT_APPEND_PARAMS)
{
    PHANTOM_ASSERT(ptr);
    return ::phantom::Allocator<t_Ty>::reallocate(ptr, n PHANTOM_MEMORY_STAT_APPEND_ARGS);
}

template<typename t_Ty>
void deallocate(t_Ty* ptr, size_t n)
{
    PHANTOM_ASSERT(ptr);
    return ::phantom::Allocator<t_Ty>::deallocate(ptr, n);
}

} // namespace phantom
