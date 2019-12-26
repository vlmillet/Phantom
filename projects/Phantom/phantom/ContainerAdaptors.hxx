#pragma once

// haunt {

#include "ContainerAdaptors.h"

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
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("ContainerAdaptors")

        PHANTOM_CLASS_T((class, class, class), (Map, Iterator, Derived), MapAdaptorIteratorBase)
        {
            this_()
        .protected_()
            .PHANTOM_T constructor<void(Iterator)>()
        
        .public_()
            .PHANTOM_T method<Derived&()>("operator++", &_::operator++)
            .PHANTOM_T method<Derived(int)>("operator++", &_::operator++)
            .PHANTOM_T method<bool(const MapAdaptorIteratorBase<Map, Iterator, Derived>&) const>("operator==", &_::operator==)
            .PHANTOM_T method<bool(const MapAdaptorIteratorBase<Map, Iterator, Derived>&) const>("operator!=", &_::operator!=)
        
        .protected_()
            .PHANTOM_T field("m_it", &_::m_it)
            ;
        }
        PHANTOM_CLASS_T((class, class), (Map, Iterator), MapKeysAdaptorIterator)
        {
            this_()
            .PHANTOM_T inherits<::phantom::MapAdaptorIteratorBase<Map, Iterator, MapKeysAdaptorIterator<Map, Iterator> >>()
        .public_()
            .PHANTOM_T constructor<void(Iterator)>()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T method<auto() const>("operator*", &_::operator*)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T method<auto() const>("operator->", &_::operator->)
            ;
        }
        PHANTOM_CLASS_T((class, class), (Map, Iterator), MapValuesAdaptorIterator)
        {
            this_()
            .PHANTOM_T inherits<::phantom::MapAdaptorIteratorBase<Map, Iterator, MapValuesAdaptorIterator<Map, Iterator> >>()
        .public_()
            .PHANTOM_T constructor<void(Iterator)>()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T method<auto() const>("operator*", &_::operator*)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .PHANTOM_T method<auto() const>("operator->", &_::operator->)
            ;
        }
        PHANTOM_CLASS_T((class), (Map), ConstMapKeysAdaptor)
        {
            using ConstIterator = typedef_<PHANTOM_TYPENAME _::ConstIterator>;
            this_()
        .public_()
            .PHANTOM_T typedef_<ConstIterator>("ConstIterator")
            .PHANTOM_T constructor<void(Map const&)>()
            .PHANTOM_T method<ConstIterator() const>("begin", &_::begin)
            .PHANTOM_T method<ConstIterator() const>("end", &_::end)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            ;
        }
        PHANTOM_CLASS_T((class), (Map), MapKeysAdaptor)
        {
            using ConstIterator = typedef_<PHANTOM_TYPENAME _::ConstIterator>;
            using Iterator = typedef_<PHANTOM_TYPENAME _::Iterator>;
            this_()
        .public_()
            .PHANTOM_T typedef_<Iterator>("Iterator")
            .PHANTOM_T typedef_<ConstIterator>("ConstIterator")
            .PHANTOM_T constructor<void(Map&)>()
            .PHANTOM_T method<ConstIterator() const>("begin", &_::begin)
            .PHANTOM_T method<ConstIterator() const>("end", &_::end)
            .PHANTOM_T method<Iterator()>("begin", &_::begin)
            .PHANTOM_T method<Iterator()>("end", &_::end)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            ;
        }
        PHANTOM_CLASS_T((class), (Map), ConstMapValuesAdaptor)
        {
            using ConstIterator = typedef_<PHANTOM_TYPENAME _::ConstIterator>;
            this_()
        .public_()
            .PHANTOM_T typedef_<ConstIterator>("ConstIterator")
            .PHANTOM_T constructor<void(Map const&)>()
            .PHANTOM_T method<ConstIterator() const>("begin", &_::begin)
            .PHANTOM_T method<ConstIterator() const>("end", &_::end)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            ;
        }
        PHANTOM_CLASS_T((class), (Map), MapValuesAdaptor)
        {
            using ConstIterator = typedef_<PHANTOM_TYPENAME _::ConstIterator>;
            using Iterator = typedef_<PHANTOM_TYPENAME _::Iterator>;
            this_()
        .public_()
            .PHANTOM_T typedef_<Iterator>("Iterator")
            .PHANTOM_T typedef_<ConstIterator>("ConstIterator")
            .PHANTOM_T constructor<void(Map&)>()
            .PHANTOM_T method<ConstIterator() const>("begin", &_::begin)
            .PHANTOM_T method<ConstIterator() const>("end", &_::end)
            .PHANTOM_T method<Iterator()>("begin", &_::begin)
            .PHANTOM_T method<Iterator()>("end", &_::end)
            .PHANTOM_T method<size_t() const>("size", &_::size)
            .PHANTOM_T method<bool() const>("empty", &_::empty)
            ;
        }
    PHANTOM_END("ContainerAdaptors")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
