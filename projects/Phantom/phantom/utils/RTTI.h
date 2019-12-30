#pragma once
#include <haunt>

HAUNT_STOP;

#include <phantom/plugin.h>

namespace phantom
{
namespace reflection
{
class Class;
}
struct PHANTOM_EXPORT_PHANTOM RTTI
{
    HAUNT_OFF
    typedef void (*CustomDeleteFunc)(void*);
    RTTI() = default;
    RTTI(RTTI const&) {}
    RTTI& operator=(RTTI const&) { return *this; }
    RTTI(RTTI&&) noexcept {}
    RTTI& operator=(RTTI&&) noexcept { return *this; }

    void*              instance = nullptr;
    reflection::Class* metaClass = nullptr;
    CustomDeleteFunc   customDeleteFunc = 0;
};

} // namespace phantom
