// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/ClassTypeT.h>
#include <phantom/lang/Structure.h>

namespace phantom
{
namespace lang
{
template<typename t_Ty, typename t_Base>
class StructureT : public ClassTypeT<t_Ty, t_Base>
{
public:
    StructureT(StringView name, Modifiers a_Modifiers = 0)
        : ClassTypeT<t_Ty, t_Base>(name, a_Modifiers)
    {
        PHANTOM_STATIC_ASSERT((std::is_class<t_Ty>::value),
                              "structures must be 'class' or 'struct' declared class-types");
        PHANTOM_STATIC_ASSERT((!std::is_abstract<t_Ty>::value && !std::is_polymorphic<t_Ty>::value),
                              "structures cannot be abstract or polymorphic");
        PHANTOM_STATIC_ASSERT(phantom::IsCopyable<t_Ty>::value, "structures must be copyable");
    }
};

template<typename t_Ty, typename t_Base>
struct TypeOf<StructureT<t_Ty, t_Base> >
{
    static Type* object()
    {
        return Class::metaClass;
    }
};

template<typename t_Ty, typename t_Base>
struct MetaTypeOf<StructureT<t_Ty, t_Base> >
{
    typedef MetaTypeOf<Class>::type type;
};

} // namespace lang

} // namespace phantom
