// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "typedefs.h"

namespace phantom
{
// construct/assign copy must be silent because EmbeddedRtti must be filled by phantom everytime

struct PHANTOM_EXPORT_PHANTOM EmbeddedRtti
{
    EmbeddedRtti() = default;
    EmbeddedRtti(EmbeddedRtti const&)
    {
    }
    EmbeddedRtti& operator=(EmbeddedRtti const&)
    {
        return *this;
    }
    EmbeddedRtti(EmbeddedRtti&&) noexcept
    {
    }
    EmbeddedRtti& operator=(EmbeddedRtti&&) noexcept
    {
        return *this;
    }

    void asObject(Object& a_Out) const;

    void*              instance = nullptr;
    reflection::Class* metaClass = nullptr;
};

struct PHANTOM_EXPORT_PHANTOM EmbeddedProxyRtti : EmbeddedRtti
{
    EmbeddedProxyRtti() = default;
    EmbeddedProxyRtti(EmbeddedProxyRtti const&)
    {
    }
    EmbeddedProxyRtti& operator=(EmbeddedProxyRtti const&)
    {
        return *this;
    }
    EmbeddedProxyRtti(EmbeddedProxyRtti&&) noexcept
    {
    }
    EmbeddedProxyRtti& operator=(EmbeddedProxyRtti&&) noexcept
    {
        return *this;
    }

    DynamicDeleteFunc dynamicDeleteFunc = 0;
};

#define PHANTOM_EMBEDDED_RTTI                                                                                          \
public:                                                                                                                \
    phantom::EmbeddedRtti PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD;                                                          \
                                                                                                                       \
private:

} // namespace phantom
