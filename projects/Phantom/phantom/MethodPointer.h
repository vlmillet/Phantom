#pragma once

namespace phantom
{
template<class T, class Sign>
struct MethodPointer;

template<class T, class R, class... Params>
struct MethodPointer<T, R(Params...)>
{
    using PointerType = R (T::*)(Params...);

    MethodPointer() = default;
    MethodPointer(PointerType a_Pointer) : m_Pointer(a_Pointer)
    {
    }
    MethodPointer(std::nullptr_t) : m_Pointer(nullptr)
    {
    }

    operator PointerType() const
    {
        return m_Pointer;
    }

    R operator()(T* a_pThis, Params... a_Args) const
    {
        return (a_pThis->*m_Pointer)(std::forward<Params>(a_Args)...);
    }

private:
    PointerType m_Pointer = nullptr;
};
} // namespace phantom
