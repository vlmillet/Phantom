#include "Object.h"

#include <phantom/reflection/Class.h>

namespace phantom
{
void* Object::as(reflection::Class* a_pClass)
{
    return rtti.metaClass->cast(a_pClass, rtti.instance);
}
void const* Object::as(reflection::Class* a_pClass) const
{
    return const_cast<Object*>(this)->as(a_pClass);
}
} // namespace phantom
