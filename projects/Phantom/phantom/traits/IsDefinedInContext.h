// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO &&                                          \
PHANTOM_COMPILER_VISUAL_STUDIO_VERSION <= PHANTOM_COMPILER_VISUAL_STUDIO_MAJOR_VERSION_2010
namespace std
{
template<typename T>
typename add_rvalue_reference<T>::type declval();
}
#endif

namespace phantom
{
template<typename T, typename Ctx = T>
struct IsDefinedInContext
{
    /* Has destructor */
    template<typename A>
    static std::true_type test(decltype(std::declval<A>().~A())*); /* {
        return std::true_type();
    }*/

    /* Has no destructor */
    template<typename A>
    static std::false_type test(...); /* {
         return false_type();
     }*/

    /* This will be either `std::true_type` or `std::false_type` */
    typedef decltype(test<T>(0)) type;

    static const bool value = type::value; /* Which is it? */
};
} // namespace phantom

/// @endcond
