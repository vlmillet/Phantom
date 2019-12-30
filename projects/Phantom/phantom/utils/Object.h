#pragma once

#include "MemorySentry.h"
#include "RTTI.h"

#include <haunt>
#include <phantom/detail/TypeOfFwd.h>
#include <phantom/plugin.h>

namespace phantom
{
namespace reflection
{
class Class;
}

class PHANTOM_EXPORT_PHANTOM Object : public MemorySentry
{
public:
    RTTI rtti; ///< @transient

public:
    template<class T>
    static T* Cast(Object* a_pObject);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  most derived address of this Object.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void* getMostDerived() const { return rtti.instance; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  meta class of this Object.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    reflection::Class* getMetaClass() const { return rtti.metaClass; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  dynamic meta cast.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void const* as(reflection::Class* a_pClass) const;
    void*       as(reflection::Class* a_pClass);
};

template<class T>
struct IsObject : public std::is_base_of<Object, T>
{
};

template<class T>
static T* Object::Cast(Object* a_pObject)
{
    PHANTOM_STATIC_ASSERT(IsObject<T>::value);
    if (a_pObject)
        return reinterpret_cast<T*>(a_pObject->as(PHANTOM_CLASSOF(T)));
    return nullptr;
}

} // namespace phantom
