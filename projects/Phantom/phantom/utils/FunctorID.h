#pragma once

namespace phantom
{
struct FunctorID
{
    FunctorID() = default;
    FunctorID(size_t a_Lo, size_t a_Hi) : lo(a_Lo), hi(a_Hi) {}
    FunctorID(void* a_Lo, void* a_Hi) : lo(size_t(a_Lo)), hi(size_t(a_Hi)) {}

    bool operator==(FunctorID const& a_Other) const { return lo == a_Other.lo && hi == a_Other.hi; }
    bool operator!=(FunctorID const& a_Other) const { return lo != a_Other.lo || hi != a_Other.hi; }

    bool operator<(FunctorID a_Other) const
    {
        if (hi == a_Other.hi)
        {
            return lo < a_Other.lo;
        }
        return hi < a_Other.hi;
    }

    bool isNull() const { return lo == 0 && hi == 0; }

    operator bool() const { return !isNull(); }

private:
    size_t lo = 0;
    size_t hi = 0;
};
} // namespace phantom
