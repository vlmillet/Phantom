#pragma once

#include "SmallSet.h"

#include <phantom/lang/SetClassT.h>
#include <phantom/meta_type>

PHANTOM_META_TYPE_T((typename, size_t, size_t, typename), (V, S, D, P), phantom::SmallSet,
                      phantom::lang::SetClassT);

// haunt {

#include "SmallSet.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "Pair.hxx"
#include "SmallVectorBasedContainer.hxx"
#include <std/initializer_list.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("SmallSet")

        PHANTOM_CLASS_T((class, size_t, size_t, class), (T, S, D, Pred), SmallSet)
        {
            using BaseType = typedef_<PHANTOM_TYPENAME _::BaseType>;
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
            using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
            using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;
            this_()
            .PHANTOM_T inherits<::phantom::SmallVectorBasedContainer<T, S, D>>()
        .public_()
            .PHANTOM_T typedef_<SelfType>("SelfType")
            .PHANTOM_T typedef_<BaseType>("BaseType")
            .PHANTOM_T typedef_<value_type>("value_type")
            .PHANTOM_T typedef_<iterator>("iterator")
            .PHANTOM_T typedef_<const_iterator>("const_iterator")
        
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(::std::initializer_list<value_type>)>()
            .PHANTOM_T constructor<void(CustomAllocator const*), explicit_>()
            .PHANTOM_T method<T const&(size_t) const>("operator[]", &_::operator[])
            .PHANTOM_T method<const_iterator(T const&) const>("find", &_::find)
            .PHANTOM_T method<iterator(T const&)>("find", &_::find)
            .PHANTOM_T method<size_t(T const&)>("erase", &_::erase)
            .PHANTOM_T method<Pair<iterator, bool>(T const&)>("insert", &_::insert)
            .PHANTOM_T method<const_iterator(const T&) const>("lower_bound", &_::lower_bound)
            .PHANTOM_T method<iterator(const T&)>("lower_bound", &_::lower_bound)
            .PHANTOM_T method<void(SelfType&)>("swap", &_::swap)
            ;
        }
    PHANTOM_END("SmallSet")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
