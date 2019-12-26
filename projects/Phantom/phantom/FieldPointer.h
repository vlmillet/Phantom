#pragma once

namespace phantom
{
template<class T, class V>
struct FieldPointer
{
    using PointerType = V(T::*);

    FieldPointer() = default;
    FieldPointer(PointerType a_Pointer) : m_Pointer(a_Pointer)
    {
    }

    operator PointerType() const
    {
        return m_Pointer;
    }

    V& operator()(T* a_pThis, V&& a_Value) const
    {
        return (a_pThis->*m_Pointer) = std::move(a_Value);
    }

    V& operator()(T* a_pThis, V const& a_Value) const
    {
        return (a_pThis->*m_Pointer) = a_Value;
    }

private:
    PointerType m_Pointer = nullptr;
};
} // namespace phantom
