// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/ClassTypeT.h>
#include <phantom/lang/Union.h>

namespace phantom
{
namespace lang
{
template<typename t_Ty, typename t_Base>
class UnionT : public ClassTypeT<t_Ty, t_Base>
{
public:
    UnionT(StringView name) : ClassTypeT<t_Ty, t_Base>(name)
    {
    }
};

template<typename t_Ty, typename t_Base>
struct TypeOf<UnionT<t_Ty, t_Base> >
{
    static Type* object()
    {
        return Class::metaClass;
    }
};

template<typename t_Ty, typename t_Base>
struct MetaTypeOf<UnionT<t_Ty, t_Base> >
{
    typedef MetaTypeOf<Class>::type type;
};

} // namespace lang
} // namespace phantom
