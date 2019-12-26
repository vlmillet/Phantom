// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "TypeOf.h"

HAUNT_STOP;

/// @cond INTERNAL

namespace phantom
{
namespace reflection
{
template<typename t_DerivedClass, typename... t_BaseClasses>
struct BaseClassProviderT
{
    struct pair
    {
        inline pair(Class* _aclass, size_t _aoffset) : _class(_aclass), _offset(_aoffset)
        {
        }
        Class* _class;
        size_t _offset;
    };

    inline static void _add(Class* a_pDerivedClass)
    {
    } // recursion end

    template<typename... Args>
    inline static void _add(Class* a_pDerivedClass, const pair& baseClass, Args... vargs)
    {
        a_pDerivedClass->addBaseClass(baseClass._class, baseClass._offset);
        _add(a_pDerivedClass, vargs...);
    }

    template<typename t_BaseClass>
    inline static pair _get(Class* a_pDerivedClass)
    {
        Class* pBaseClass = PHANTOM_CLASSOF(t_BaseClass);
        PHANTOM_ASSERT(pBaseClass);
        return pair(pBaseClass, size_t(static_cast<t_BaseClass*>((t_DerivedClass*)1)) - 1);
    }

    static void add(Class* a_pDerivedClass)
    {
        _add(a_pDerivedClass, _get<t_BaseClasses>(a_pDerivedClass)...);
    }
};

} // namespace reflection
} // namespace phantom

/// @endcond
