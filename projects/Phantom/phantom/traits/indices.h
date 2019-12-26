// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond ADVANCED

namespace phantom
{
template<std::size_t... Is>
struct Indices
{
};

template<std::size_t N, std::size_t... Is>
struct BuildIndices : BuildIndices<N - 1, N - 1, Is...>
{
};

template<std::size_t... Is>
struct BuildIndices<0, Is...> : Indices<Is...>
{
};

} // namespace phantom

/// @endcond
