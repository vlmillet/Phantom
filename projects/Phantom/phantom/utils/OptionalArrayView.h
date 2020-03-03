// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "ArrayView.h"
#include "Optional.h"

namespace phantom
{
template<class T>
using OptionalArrayView = Optional<ArrayView<T>>;

template<class Container>
auto MakeOptionalArrayView(Container const* c)
{
    return c ? OptionalArrayView<typename Container::value_type>(MakeArrayView(*c)) : NullOpt;
}
} // namespace phantom
