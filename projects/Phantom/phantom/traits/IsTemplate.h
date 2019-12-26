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
struct IsTemplate : std::false_type
{
};

template<template<typename...> class t_Ty, typename... t_Params>
struct IsTemplate<t_Ty<t_Params...> > : std::true_type
{
};

} // namespace phantom

/// @endcond
