// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Owner, typename t_ValueType>
struct MemberPointerMaker
{
    typedef t_ValueType(t_Owner::*type);
};

template<typename t_Owner, typename t_ValueType, size_t t_size0>
struct MemberPointerMaker<t_ValueType[t_size0], t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1>
struct MemberPointerMaker<t_ValueType[t_size0][t_size1], t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2>
struct MemberPointerMaker<t_ValueType[t_size0][t_size1][t_size2], t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1][t_size2];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3>
struct MemberPointerMaker<t_ValueType[t_size0][t_size1][t_size2][t_size3], t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1][t_size2][t_size3];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3, size_t t_size4>
struct MemberPointerMaker<t_ValueType[t_size0][t_size1][t_size2][t_size3][t_size4], t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1][t_size2][t_size3][t_size4];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3, size_t t_size4, size_t t_size5>
struct MemberPointerMaker<t_ValueType[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5],
                          t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3, size_t t_size4, size_t t_size5, size_t t_size6>
struct MemberPointerMaker<
t_ValueType[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5][t_size6], t_Owner>
{
    typedef t_ValueType (
    t_Owner::*type)[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5][t_size6];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3, size_t t_size4, size_t t_size5, size_t t_size6, size_t t_size7>
struct MemberPointerMaker<
t_ValueType[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5][t_size6][t_size7], t_Owner>
{
    typedef t_ValueType (
    t_Owner::*type)[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5][t_size6][t_size7];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3, size_t t_size4, size_t t_size5, size_t t_size6, size_t t_size7,
         size_t t_size8>
struct MemberPointerMaker<
t_ValueType[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5][t_size6][t_size7][t_size8],
t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5]
                                        [t_size6][t_size7][t_size8];
};

template<typename t_Owner, typename t_ValueType, size_t t_size0, size_t t_size1, size_t t_size2,
         size_t t_size3, size_t t_size4, size_t t_size5, size_t t_size6, size_t t_size7,
         size_t t_size8, size_t t_size9>
struct MemberPointerMaker<t_ValueType[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5][t_size6]
                                     [t_size7][t_size8][t_size9],
                          t_Owner>
{
    typedef t_ValueType (t_Owner::*type)[t_size0][t_size1][t_size2][t_size3][t_size4][t_size5]
                                        [t_size6][t_size7][t_size8][t_size9];
};
} // namespace phantom

/// @endcond
