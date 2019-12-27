#pragma once

#include <haunt>

HAUNT_STOP;

#include <type_traits>

template<typename T>
T _PHNTM_IDTTY(T&& x)
{
    return std::forward<T>(x);
}

namespace phantom
{
namespace static_if_detail
{
struct identity
{
    template<typename T>
    auto&& operator()(T&& x) const
    {
        return std::forward<T>(x);
    }
};

template<bool Cond>
struct statement
{
    template<typename F>
    void then(const F& f)
    {
        f(identity());
    }

    template<typename F>
    void else_(const F&)
    {
    }
};

template<>
struct statement<false>
{
    template<typename F>
    void then(const F&)
    {
    }

    template<typename F>
    void else_(const F& f)
    {
        f(identity());
    }
};

} // end of namespace static_if_detail

template<bool Cond, typename F>
static_if_detail::statement<Cond> static_if(F const& f)
{
    static_if_detail::statement<Cond> if_;
    if_.then(f);
    return if_;
}
} // namespace phantom
