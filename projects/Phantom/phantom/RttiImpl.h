// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/phantom.h>

namespace phantom
{
namespace detail
{
template<class T>
struct RttiReturn;
}

HAUNT_PACKAGE("phantom");
HAUNT_SOURCE("Rtti");

struct PHANTOM_EXPORT_PHANTOM Rtti
{
    static void Map(void const* a_pThis, reflection::Class* a_pClass);
    template<class T>
    static void               Map(T const* a_pThis);
    static void               Unmap(void const* a_pThis);
    static reflection::Class* ClassAt(void const* a_pThis);
    static const RttiMapData& Find(void const* a_pThis);
    template<class T>
    static typename detail::RttiReturn<T>::type Find(T const* a_pThis);

    static reflection::Class* ClassOf(void const* a_pThis);
    template<class T>
    static reflection::Class* ClassOf(T const* a_pThis);

    static void* MostDerivedOf(void const* a_pThis);
    template<class T>
    static void const* MostDerivedOf(T const* a_pThis);
    template<class T>
    static void* MostDerivedOf(T* a_pThis);

    static size_t AddressesOf(void const* a_pThis, void const** a_pBuffer, size_t a_BufferSize);

    // expert only
    static const RttiMapData* InsertCustomData(void const* a_pThis, const RttiMapData& a_RttiData);
    static void               ReplaceCustomData(void const* a_pThis, const RttiMapData& a_RttiData);
    static void               EraseCustomData(void const* a_pThis);
};

HAUNT_END("Rtti");
HAUNT_END("phantom");
} // namespace phantom
