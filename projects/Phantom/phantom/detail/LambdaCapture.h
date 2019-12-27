#pragma once

#include <utility>

namespace phantom
{
template<typename T, typename F>
class LambdaCaptureImpl
{
    T x;
    F f;

public:
    LambdaCaptureImpl(T&& x, F&& f) : x{std::forward<T>(x)}, f{std::forward<F>(f)}
    {
    }

    template<typename... Ts>
    auto operator()(Ts&&... args) -> decltype(f(x, std::forward<Ts>(args)...))
    {
        return f(x, std::forward<Ts>(args)...);
    }

    template<typename... Ts>
    auto operator()(Ts&&... args) const -> decltype(f(x, std::forward<Ts>(args)...))
    {
        return f(x, std::forward<Ts>(args)...);
    }
};

template<typename T, typename F>
LambdaCaptureImpl<T, F> LambdaCapture(T&& x, F&& f)
{
    return LambdaCaptureImpl<T, F>(std::forward<T>(x), std::forward<F>(f));
}
} // namespace phantom
