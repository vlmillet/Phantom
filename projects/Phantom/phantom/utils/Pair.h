// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/traits/CopyTraits.h>
#include <phantom/traits/MoveTraits.h>
#include <phantom/utils/SmallVector.h>
/* *********************************************** */

namespace phantom
{
template<class t_First, class t_Second>
struct Pair
{
    Pair() : first(), second()
    {
    }
    Pair(t_First a_First, t_Second a_Second) : first(a_First), second(a_Second)
    {
    }
    template<class t_OtherFirst, class t_OtherSecond>
    Pair(Pair<t_OtherFirst, t_OtherSecond> const& a_Other) : first(a_Other.first), second(a_Other.second)
    {
    }
    template<class t_OtherFirst, class t_OtherSecond>
    Pair(Pair<t_OtherFirst, t_OtherSecond>&& a_Temp) : first(std::move(a_Temp.first)), second(std::move(a_Temp.second))
    {
    }
    t_First  first;
    t_Second second;
};

template<class t_First, class t_Second>
Pair<typename std::remove_reference<t_First>::type, typename std::remove_reference<t_Second>::type>
makePair(t_First&& a_First, t_Second&& a_Second)
{
    return Pair<typename std::remove_reference<t_First>::type, typename std::remove_reference<t_Second>::type>(
    std::forward<t_First>(a_First), std::forward<t_Second>(a_Second));
}
} // namespace phantom

PHANTOM_DISABLE_TRAIT_IF_T(IsCopyAssignable, (class, class), (K, V), Pair,
                           !IsCopyAssignable<K>::value OR !IsCopyAssignable<V>::value);
PHANTOM_DISABLE_TRAIT_IF_T(IsMoveAssignable, (class, class), (K, V), Pair,
                           !IsMoveAssignable<K>::value OR !IsMoveAssignable<V>::value);
PHANTOM_DISABLE_TRAIT_IF_T(IsCopyConstructible, (class, class), (K, V), Pair,
                           !IsCopyConstructible<K>::value OR !IsCopyConstructible<V>::value);
PHANTOM_DISABLE_TRAIT_IF_T(IsMoveConstructible, (class, class), (K, V), Pair,
                           !IsMoveConstructible<K>::value OR !IsMoveConstructible<V>::value);
