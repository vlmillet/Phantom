// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

//	Delegate class highly inpired by "FastDelegate" using modern C++11 variadic templates

#pragma once

#include "MethodClosure.h"

#include <haunt>
#include <phantom/config.h>
#include <phantom/typedefs.h>
#include <string.h>

namespace phantom
{
namespace detail
{
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
class __single_inheritance DelegateGenericClass;
class DelegateGenericClass
{
}; ///< @off
#else
class DelegateGenericClass
{
}; ///< @off
#endif
} // namespace detail
HAUNT_ON class OpaqueDelegate
{
    HAUNT_OFF protected : HAUNT_OFF typedef void (detail::DelegateGenericClass::*MFPtr)();
    HAUNT_OFF detail::DelegateGenericClass* m_pThis;
    HAUNT_OFF MFPtr m_pFunction;

public:
    OpaqueDelegate() : m_pThis(0), m_pFunction(0){};
    void clear()
    {
        m_pThis = 0;
        m_pFunction = 0;
    }

public:
    inline bool operator==(const OpaqueDelegate& other) const
    {
        return m_pThis == other.m_pThis && m_pFunction == other.m_pFunction;
    }
    inline bool operator!=(const OpaqueDelegate& other) const
    {
        return !operator==(other);
    }

    inline bool operator!() const
    {
        return m_pThis == 0 && m_pFunction == 0;
    }
    inline bool empty() const
    {
        return m_pThis == 0 && m_pFunction == 0;
    }

    int getThisOffset() const
    {
        return Closure(MethodClosure(m_pFunction)).offset;
    }

    MethodClosure getMethod() const
    {
        return MethodClosure(m_pFunction);
    }
    void* getThis() const
    {
        return m_pFunction ? m_pThis : nullptr;
    }
    void* getFunction() const
    {
        return m_pFunction ? nullptr : m_pThis;
    }
    FunctorID getID() const
    {
        return FunctorID(m_pThis, MethodClosure(m_pFunction).getAddress());
    }

public:
    OpaqueDelegate& operator=(const OpaqueDelegate& right)
    {
        setOpaqueFrom(right);
        return *this;
    }
    inline bool operator<(const OpaqueDelegate& right) const
    {
        if (m_pThis != right.m_pThis)
            return m_pThis < right.m_pThis;
        return memcmp(&m_pFunction, &right.m_pFunction, sizeof(m_pFunction)) < 0;
    }
    inline bool operator>(const OpaqueDelegate& right) const
    {
        return right.operator<(*this);
    }
    OpaqueDelegate(const OpaqueDelegate& right)
        : m_pThis(right.m_pThis), m_pFunction(right.m_pFunction)
    {
    }

protected:
    void setOpaqueFrom(const OpaqueDelegate& right)
    {
        m_pFunction = right.m_pFunction;
        m_pThis = right.m_pThis;
    }
};

} // namespace phantom
