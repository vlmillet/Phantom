// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet. 
// Distributed under the MIT license. Text available here at http://www.wiwila.com/tools/phantom/license/ 
// ]

#include "ClassOf.h"

namespace phantom
{
    template<typename t_Ty>
    inline Object::Object(t_Ty* const a_address)
    {
        // class
        const RttiMapData& rd = Rtti::Find(a_address);
        m_pAddress = rd.getObjectAddress() ? rd.getObjectAddress() : a_address;
        if (rd.getObjectClass())
            m_pClass = rd.getObjectClass();
        else
            m_pClass = PHANTOM_CLASSOF(t_Ty);
    }

    template<typename t_Ty>
    inline Object& Object::operator=(t_Ty* const a_address)
    {
        const RttiMapData& rd = Rtti::Find(a_address);
        m_pAddress = rd.getObjectAddress() ? rd.getObjectAddress() : a_address;
        if (rd.getObjectClass())
            m_pClass = rd.getObjectClass();
        else
            m_pClass = PHANTOM_CLASSOF(t_Ty);
        return *this;
    }

    // Inlining as soon as possible the member_function which use ClassType class
    template<typename t_Ty>
    t_Ty* phantom::Object::as()
    {
        reflection::Class* targetType = PHANTOM_CLASSOF(t_Ty);
        PHANTOM_ASSERT(m_pClass->isA(targetType));
        if (m_pClass == targetType) return static_cast<t_Ty*>(m_pAddress);
        return static_cast<t_Ty*>(m_pClass->upcast(targetType, m_pAddress));
    }
}