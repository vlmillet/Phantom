#pragma once

// haunt {

// clang-format off

#include "SmallMultimap.h"

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
    PHANTOM_SOURCE("SmallMultimap")

        PHANTOM_CLASS_T((class, class, phantom::uint, class), (K, V, S, Pred), SmallMultimap)
        {
            using BaseType = typedef_<PHANTOM_TYPENAME _::BaseType>;
            using SelfType = typedef_<PHANTOM_TYPENAME _::SelfType>;
            using const_iterator = typedef_<PHANTOM_TYPENAME _::const_iterator>;
            using iterator = typedef_<PHANTOM_TYPENAME _::iterator>;
            using value_type = typedef_<PHANTOM_TYPENAME _::value_type>;
            this_()
            .PHANTOM_T inherits<::phantom::SmallVectorBasedContainer<Pair<const K, V>, S>>()
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
            .PHANTOM_T constructor<void(::std::initializer_list<value_type>)>()({"a_InitList"})
            .PHANTOM_T constructor<void(CustomAllocator const*), explicit_>()({"a_pMemTraits"})
            .PHANTOM_T method<V&(K const&)>("operator[]", &_::operator[])({"_key"})
            .PHANTOM_T method<const_iterator(K const&) const>("find", &_::find)({"_key"})
            .PHANTOM_T method<iterator(K const&)>("find", &_::find)({"_key"})
            .PHANTOM_T method<size_t(K const&)>("erase", &_::erase)({"_key"})
            .PHANTOM_T method<iterator(value_type const&)>("insert", &_::insert)({"a_Pair"})
            .PHANTOM_T method<const_iterator(const K&) const>("lower_bound", &_::lower_bound)({"_key"})
            .PHANTOM_T method<iterator(const K&)>("lower_bound", &_::lower_bound)({"_key"})
            .PHANTOM_T method<const_iterator(const K&) const>("upper_bound", &_::upper_bound)({"_key"})
            .PHANTOM_T method<iterator(const K&)>("upper_bound", &_::upper_bound)({"_key"})
            .PHANTOM_T method<void(SelfType&)>("swap", &_::swap)({"a_Other"})
            ;
        }
    PHANTOM_END("SmallMultimap")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
