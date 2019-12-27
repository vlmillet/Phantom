// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "crc64.h"

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
