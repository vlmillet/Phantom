// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
