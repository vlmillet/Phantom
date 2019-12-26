// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/reflection/ClassTypeT.h>
#include <phantom/reflection/Union.h>

namespace phantom
{
namespace reflection
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

} // namespace reflection
} // namespace phantom
