// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/traits/CopyTraits.h>

namespace phantom
{
namespace reflection
{
namespace detail
{
template<typename t_Ty, bool t_is_copyable, bool t_is_movable>
struct FieldHH
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type NoConst;
    static void                                            GetValue(void* dest, t_Ty& src)
    {
        *reinterpret_cast<NoConst*>(dest) = src;
    }
    static void SetValue(void const* src, t_Ty& dest)
    {
        const_cast<NoConst&>(dest) = *const_cast<NoConst*>(reinterpret_cast<t_Ty const*>(src));
    }
    static void SetValueMoved(void* src, t_Ty& dest)
    {
        const_cast<NoConst&>(dest) = std::move(*const_cast<NoConst*>(reinterpret_cast<t_Ty*>(src)));
    }
};

template<typename t_Ty>
struct FieldHH<t_Ty, false, false>
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type NoConst;
    static void                                            GetValue(void* dest, t_Ty& src)
    {
        PHANTOM_UNREACHABLE();
    }
    static void SetValue(void const* src, t_Ty& dest)
    {
        PHANTOM_UNREACHABLE();
    }
    static void SetValueMoved(void* src, t_Ty& dest)
    {
        PHANTOM_UNREACHABLE();
    }
};

template<typename t_Ty>
struct FieldHH<t_Ty, true, false>
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type NoConst;
    static void                                            GetValue(void* dest, t_Ty& src)
    {
        *reinterpret_cast<NoConst*>(dest) = src;
    }
    static void SetValue(void const* src, t_Ty& dest)
    {
        const_cast<NoConst&>(dest) = *const_cast<NoConst*>(reinterpret_cast<t_Ty const*>(src));
    }
    static void SetValueMoved(void* src, t_Ty& dest)
    {
        const_cast<NoConst&>(dest) = *const_cast<NoConst*>(reinterpret_cast<t_Ty*>(src));
    }
};

template<typename t_Ty>
struct FieldHH<t_Ty, false, true>
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type NoConst;
    static void                                            GetValue(void* dest, t_Ty& src)
    {
        PHANTOM_UNREACHABLE();
    }
    static void SetValue(void const* src, t_Ty& dest)
    {
        PHANTOM_UNREACHABLE();
    }
    static void SetValueMoved(void* src, t_Ty& dest)
    {
        const_cast<NoConst&>(dest) = std::move(*const_cast<NoConst*>(reinterpret_cast<t_Ty*>(src)));
    }
};

} // namespace detail

template<typename t_Ty>
struct FieldH : public detail::FieldHH<t_Ty, std::is_copy_assignable<t_Ty>::value,
                                       std::is_move_assignable<t_Ty>::value>
{
};

template<typename t_Ty, size_t t_size>
struct FieldH<t_Ty[t_size]>
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type no_const;
    static void                                            GetValue(void* dest, t_Ty src[t_size])
    {
        size_t i = 0;
        for (; i < t_size; ++i)
        {
            FieldH<t_Ty>::GetValue(&reinterpret_cast<no_const*>(dest)[i], src[i]);
        }
    }
    static void SetValue(void const* src, t_Ty dest[t_size])
    {
        size_t i = 0;
        for (; i < t_size; ++i)
        {
            FieldH<t_Ty>::SetValue(&(const_cast<no_const*>(reinterpret_cast<t_Ty const*>(src))[i]),
                                   dest[i]);
        }
    }
    static void SetValueMoved(void* src, t_Ty dest[t_size])
    {
        size_t i = 0;
        for (; i < t_size; ++i)
        {
            FieldH<t_Ty>::SetValueMoved(&(const_cast<no_const*>(reinterpret_cast<t_Ty*>(src))[i]),
                                        dest[i]);
        }
    }
};

} // namespace reflection
} // namespace phantom
