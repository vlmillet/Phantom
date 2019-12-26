#pragma once

namespace phantom
{
template<class Sign>
struct FunctionPointer;

template<class R, class... Params>
struct FunctionPointer<R(Params...)>
{
    using PointerType = R (*)(Params...);

    FunctionPointer() = default;
    FunctionPointer(PointerType a_Pointer) : m_Pointer(a_Pointer)
    {
    }
    FunctionPointer(std::nullptr_t) : m_Pointer(nullptr)
    {
    }

    operator PointerType() const
    {
        return m_Pointer;
    }

    R operator()(Params... a_Args) const
    {
        return (m_Pointer)(std::forward<Params>(a_Args)...);
    }

private:
    PointerType m_Pointer = nullptr;
};
} // namespace phantom
