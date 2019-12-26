// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
template<typename t_Ty>
struct IsDataPointer : public std::false_type
{
};

template<typename t_Ty>
struct IsDataPointer<t_Ty*> : public std::true_type
{
};

template<typename t_Ty>
struct IsDataPointer<t_Ty* const> : public std::true_type
{
};

template<typename t_Ty>
struct IsDataPointer<t_Ty* volatile> : public std::true_type
{
};

template<typename t_Ty>
struct IsDataPointer<t_Ty* const volatile> : public std::true_type
{
};
} // namespace phantom

/// @endcond
