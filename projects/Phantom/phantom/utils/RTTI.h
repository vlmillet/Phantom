// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once
#include <haunt>

HAUNT_STOP;

#include <phantom/plugin.h>

namespace phantom
{
namespace lang
{
class Class;
}
struct PHANTOM_EXPORT_PHANTOM RTTI
{
    HAUNT_OFF
    RTTI() = default;
    RTTI(RTTI const&) {}
    RTTI& operator=(RTTI const&) { return *this; }
    RTTI(RTTI&&) noexcept {}
    RTTI& operator=(RTTI&&) noexcept { return *this; }

    void*        instance = nullptr;
    lang::Class* metaClass = nullptr;
};

} // namespace phantom
