#pragma once

// haunt {

#include "RttiImpl.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/static_method>

namespace phantom {
namespace detail {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("RttiImpl")

        PHANTOM_CLASS_T((class), (T), RttiReturn)
        {
            this_()
            ;
        }
    PHANTOM_END("RttiImpl")
PHANTOM_END("phantom")
}
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Rtti")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(Rtti)
        {
            this_()
            .staticMethod<void(void const*, ::phantom::reflection::Class *)>("Map", &_::Map)
            .staticMethod<void(void const*)>("Unmap", &_::Unmap)
            .staticMethod<reflection::Class*(void const*)>("ClassAt", &_::ClassAt)
            .staticMethod<const RttiMapData&(void const*)>("Find", &_::Find)
            .staticMethod<reflection::Class*(void const*)>("ClassOf", &_::ClassOf)
            .staticMethod<void*(void const*)>("MostDerivedOf", &_::MostDerivedOf)
            .staticMethod<size_t(void const*, void const**, size_t)>("AddressesOf", &_::AddressesOf)
            .staticMethod<const RttiMapData*(void const*, const RttiMapData&)>("InsertCustomData", &_::InsertCustomData)
            .staticMethod<void(void const*, const RttiMapData&)>("ReplaceCustomData", &_::ReplaceCustomData)
            .staticMethod<void(void const*)>("EraseCustomData", &_::EraseCustomData)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Rtti")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
