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
