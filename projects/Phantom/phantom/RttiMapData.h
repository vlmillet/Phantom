// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/newImpl.h>
#include <phantom/reflection/Class.h>
#include <phantom/typeof>

namespace phantom
{
/**
 * \struct RttiMapData
 *
 * \brief The mapped Rtti data you can register/unregister/find using mapRtti()/unmapRtti()/findRtti
 * functions.
 */

struct PHANTOM_EXPORT_PHANTOM RttiMapData
{
    template<class>
    friend struct phantom::reflection::detail::TypeOfBuilder;
    friend class phantom::detail::DynamicCppInitializerH;
    RttiMapData() = default;
    RttiMapData(phantom::reflection::Class* a_pObjectClass,
                phantom::reflection::Class* a_pAddressClass, void const* a_pObject,
                DynamicDeleteFunc a_DynDelFunc = nullptr)
        : m_pAddressClass(a_pAddressClass),
          m_pObject((void*)a_pObject),
          m_pObjectClass(a_pObjectClass),
          m_DynDeleteFunc(a_DynDelFunc)
    {
        PHANTOM_ASSERT((a_pObjectClass == nullptr AND a_pObject == nullptr) OR m_DynDeleteFunc);
    }
    PHANTOM_FORCEINLINE bool isMostDerived() const
    {
        return m_pObjectClass == m_pAddressClass;
    }
    PHANTOM_FORCEINLINE bool isNull() const
    {
        return m_pObjectClass == nullptr;
    }

    inline void delete_() const;

    PHANTOM_FORCEINLINE void* cast(reflection::Class* a_pTargetClass) const;
    void                      asObject(Object& out) const;

    reflection::Class* getAddressClass() const
    {
        return m_pAddressClass;
    }
    void* getObjectAddress() const
    {
        return m_pObject;
    }
    reflection::Class* getObjectClass() const
    {
        return m_pObjectClass;
    }

private:
    reflection::Class* m_pAddressClass = nullptr;
    void*              m_pObject = nullptr;
    reflection::Class* m_pObjectClass = nullptr;
    DynamicDeleteFunc  m_DynDeleteFunc = nullptr;
};

inline void RttiMapData::delete_() const
{
    if (m_DynDeleteFunc)
        (*m_DynDeleteFunc)(const_cast<void*>(m_pObject));
    else
        m_pObjectClass->deleteInstance(const_cast<void*>(m_pObject));
}

PHANTOM_FORCEINLINE void*
phantom::RttiMapData::cast(phantom::reflection::Class* a_pTargetClass) const
{
    return m_pObjectClass->upcast(a_pTargetClass, m_pObject);
}

} // namespace phantom

#include "RttiMapData.inl"
