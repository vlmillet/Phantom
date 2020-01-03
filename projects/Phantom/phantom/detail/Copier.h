// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>
HAUNT_STOP;
#include <phantom/detail/phantom.h>
#include <phantom/traits/CopyTraits.h>

namespace phantom
{
/// @cond INTERNAL

namespace detail
{
template<typename t_Ty, bool t_copy_assignable>
struct CopierH
{
    typedef PHANTOM_TYPENAME std::remove_cv<t_Ty>::type t_Ty_no_cv;
    static void                                         copy(t_Ty* a_pDest, t_Ty const* a_pSrc)
    {
        *((t_Ty_no_cv*)a_pDest) = *((t_Ty_no_cv const*)a_pSrc);
    }
};

template<typename t_Ty>
struct CopierH<t_Ty, false>
{
    static void copy(t_Ty*, t_Ty const*)
    {
        PHANTOM_ASSERT(false, "not copy assignable");
    }
};
} // namespace detail

/// @end

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct Copier
///
/// \brief  Copier.
/// 		Provides copy of type instances
///
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename t_Ty>
struct Copier : public detail::CopierH<t_Ty, IsCopyAssignable<t_Ty>::value>
{
    PHANTOM_REBIND(Copier)
};

template<typename t_Ty, size_t t_Size>
struct Copier<t_Ty[t_Size]>
{
    typedef PHANTOM_TYPENAME std::remove_const<t_Ty>::type t_Ty_no_const;
    static void                                            copy(t_Ty* a_pDest, t_Ty const* a_pSrc)
    {
        for (size_t i = 0; i < t_Size; ++i)
        {
            Copier<t_Ty>::copy(a_pDest + i, a_pSrc + i);
        }
    }
};

} // namespace phantom
