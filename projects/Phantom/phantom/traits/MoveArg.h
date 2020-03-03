// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

namespace phantom
{
struct MoveArg
{
    explicit MoveArg(void* valueptr) : valueptr(valueptr){};
    operator void*() const { return valueptr; }

private:
    void* valueptr;
};
} // namespace phantom
