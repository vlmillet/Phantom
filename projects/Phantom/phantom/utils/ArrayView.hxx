#pragma once

// haunt {

// clang-format off

#include "ArrayView.h"

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

#include <phantom/template-only-push>

#include <std/initializer_list.hxx>

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("ArrayView")

        PHANTOM_CLASS_T((class), (T), ArrayView)
        {
            this_()
        .public_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(const T*, size_t)>()
            .PHANTOM_T constructor<void(const T*, const T*)>()
            .PHANTOM_T constructor<void(::std::initializer_list<T>)>()
            .PHANTOM_T method<const T*() const>("begin", &_::begin)
            .PHANTOM_T method<const T*() const>("end", &_::end)
            .PHANTOM_T method<const T*() const>("data", &_::data)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            .PHANTOM_T method<T const&() const>("front", &_::front)
            .PHANTOM_T method<T const&() const>("back", &_::back)
            .PHANTOM_T method<T const&(size_t) const>("operator[]", &_::operator[])
            ;
        }
    PHANTOM_END("ArrayView")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
