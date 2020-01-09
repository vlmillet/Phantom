// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/traits/Promote.h>
#include <phantom/typeof>

HAUNT_STOP;

namespace phantom
{
namespace lang
{
namespace detail
{
template<typename t_Ty, bool t_is_fundamental>
struct DefaultPromoterH
{
    static lang::Type* apply(lang::Type*)
    {
        return PHANTOM_TYPEOF(PHANTOM_TYPENAME Promote<t_Ty>::type);
    }
};
template<typename t_Ty>
struct DefaultPromoterH<t_Ty, false>
{
    static lang::Type* apply(lang::Type* a_pType)
    {
        return a_pType;
    }
};
} // namespace detail

template<typename t_Ty>
struct Promoter : public detail::DefaultPromoterH<t_Ty, std::is_fundamental<t_Ty>::value>
{
};
} // namespace lang
} // namespace phantom
