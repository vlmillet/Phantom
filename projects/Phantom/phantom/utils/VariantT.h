// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "Variant.h"

namespace phantom
{
namespace _VariantT
{
/// @off
template<class Base, class T, class... Ts>
struct VariantT_Recursive : public VariantT_Recursive<Base, Ts...>
{
    using VariantT_Recursive<Base, Ts...>::operator=;
    Base&                                  operator=(T const& _val)
    {
        this_()->m_genericVariant = _val;
        return *this_();
    }

private:
    Base* this_() { return static_cast<Base*>(this); }
};

/// @off
template<class Base, class T>
struct VariantT_Recursive<Base, T>
{
    Base& operator=(T const& _val)
    {
        this_()->m_genericVariant = _val;
        return *this_();
    }

private:
    Base* this_() { return static_cast<Base*>(this); }
};

template<class T, class... Ts>
struct SelectFirstType
{
    using type = T;
};

template<class T>
struct SelectFirstType<T>
{
    using type = T;
};

} // namespace _VariantT

template<class... Ts>
struct VariantT : _VariantT::VariantT_Recursive<VariantT<Ts...>, Ts...>
{
    template<class Base, class T, class... Ts>
    friend struct _VariantT::VariantT_Recursive;

    using FirstType = typename _VariantT::SelectFirstType<Ts...>::type;

    using _VariantT::VariantT_Recursive<VariantT<Ts...>, Ts...>::operator=;

    VariantT() { m_genericVariant.setType(PHANTOM_TYPEOF(FirstType)); }

    lang::Type* type() const { return m_genericVariant.type(); }
    void const* data() const { return m_genericVariant.data(); }
    void*       data() { return m_genericVariant.data(); }

    /// @property
    void setGeneric(Variant _var)
    {
        lang::Types types{PHANTOM_TYPEOF(Ts)...};
        for (auto type : types)
        {
            if (type->isSame(_var.type()))
            {
                m_genericVariant = std::move(_var);
                return;
            }
        }
        PHANTOM_UNREACHABLE();
    }
    /// @property
    Variant GetGeneric() const { return m_genericVariant; }

private:
    Variant m_genericVariant;
};
} // namespace phantom
