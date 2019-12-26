// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond ADVANCED
namespace phantom
{
namespace detail
{
template<typename t_Ty, size_t t_size_of>
struct EnumIntTypeH;

template<typename t_Ty>
struct EnumIntTypeH<t_Ty, sizeof(char)>
{
    typedef char type;
};
template<typename t_Ty>
struct EnumIntTypeH<t_Ty, sizeof(short)>
{
    typedef short type;
};
template<typename t_Ty>
struct EnumIntTypeH<t_Ty, sizeof(int)>
{
    typedef int type;
};
template<typename t_Ty>
struct EnumIntTypeH<t_Ty, sizeof(long long)>
{
    typedef long long type;
};
} // namespace detail

template<typename t_Ty>
struct EnumIntType : public detail::EnumIntTypeH<t_Ty, sizeof(t_Ty)>
{
};
} // namespace phantom
/// @endcond
