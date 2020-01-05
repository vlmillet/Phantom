// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <phantom/detail/core.h>
#include <phantom/traits/CopyTraits.h>

namespace phantom
{
/// @cond INTERNAL

// Mover

template<typename t_Ty>
struct Mover;

namespace detail
{
template<typename t_Ty, bool t_move_assignable>
struct MoverH
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type t_Ty_no_const;

    static void move(t_Ty* a_pDest, t_Ty* a_pSrc)
    {
        *((t_Ty_no_const*)a_pDest) = std::move(*((t_Ty_no_const*)a_pSrc));
    }
};

template<typename t_Ty>
struct MoverH<t_Ty, false>
{
    static void move(t_Ty*, t_Ty*)
    {
        PHANTOM_ASSERT(false, "not move assignable");
    }
};
} // namespace detail

template<typename t_Ty>
struct Mover : public detail::MoverH<t_Ty, IsMoveAssignableAndNotDisabled<t_Ty>::value>
{
    PHANTOM_REBIND(Mover)
};

template<typename t_Ty, size_t t_Size>
struct Mover<t_Ty[t_Size]>
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type t_Ty_no_const;
    static void                                            move(t_Ty* a_pDest, t_Ty const* a_pSrc)
    {
        for (size_t i = 0; i < t_Size; ++i)
        {
            Mover<t_Ty>::move(a_pDest + i, a_pSrc + i);
        }
    }
};

/// @endcond

} // namespace phantom
