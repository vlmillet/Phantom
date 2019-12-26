#pragma once

namespace phantom
{
template<class T>
struct ImplicitConv
{
    ImplicitConv(T&& d);
};
} // namespace phantom
