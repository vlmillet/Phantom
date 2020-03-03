// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/utils/crc64.h>

HAUNT_STOP

namespace phantom
{
template<class T>
struct Hasher
{
    static uint64_t apply(const T* a_pInstance)
    {
        return crc64(0, reinterpret_cast<const unsigned char*>(a_pInstance), sizeof(T));
    }
};
} // namespace phantom
