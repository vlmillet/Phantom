// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <haunt>
#include <phantom/fwd.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
HAUNT_PACKAGE("phantom")
HAUNT_SOURCE("Object")
/**
 * \brief Object stores a memory address with the reflection ClassType associated
 */
class PHANTOM_EXPORT_PHANTOM Object
{
public:
    Object() = default;
    Object(void* a_pAddress, reflection::Class* a_pClass)
        : m_pAddress(a_pAddress), m_pClass(a_pClass)
    {
    }
    template<typename t_Ty>
    inline Object(t_Ty* const a_address);

    template<typename t_Ty>
    inline Object& operator=(t_Ty* const a_address);

    template<typename t_Ty>
    inline t_Ty* as();

    bool isNull() const
    {
        return m_pAddress == nullptr;
    }
    void destroy();

    bool operator==(const Object& a_Other) const
    {
        return a_Other.m_pAddress == m_pAddress AND a_Other.m_pClass == m_pClass;
    }

    bool operator!=(const Object& a_Other) const
    {
        return a_Other.m_pAddress != m_pAddress OR a_Other.m_pClass != m_pClass;
    }

    void* getAddress() const
    {
        return m_pAddress;
    }
    reflection::Class* getClass() const
    {
        return m_pClass;
    }

    Object as(reflection::Class* a_pDest) const;

private:
    void*              m_pAddress = nullptr;
    reflection::Class* m_pClass = nullptr;
};

HAUNT_END("Object")
HAUNT_END("phantom")

} // namespace phantom
