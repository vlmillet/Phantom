// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "String.h"
#include "StringHash.h"

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM StringWithHash
{
    StringWithHash() = default;
    StringWithHash(StringHash hash) : hash(hash)
    {
    }
    StringWithHash(String str);
    StringWithHash(StringView str);
    StringWithHash(const char* str);
    StringHash hash = 0;
    String     str;
    bool       operator<(StringWithHash const& other) const
    {
        return hash < other.hash;
    }
    inline operator StringHash() const
    {
        return hash;
    }
    inline explicit operator String() const
    {
        return str;
    }
    inline explicit operator StringView() const
    {
        return str;
    }
};

} // namespace phantom

namespace std
{
template<>
struct hash<phantom::StringWithHash>
{
public:
    phantom::size_t operator()(phantom::StringWithHash pair) const
    {
        return (phantom::size_t)pair.hash;
    }
};
} // namespace std
