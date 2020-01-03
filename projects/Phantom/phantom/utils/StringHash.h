#pragma once

#include "StringView.h"

#include <functional>
#include <phantom/plugin.h>

namespace phantom
{
PHANTOM_EXPORT_PHANTOM uint64_t makeStringHash(StringView a_Str);

struct StringWithHash;

struct PHANTOM_EXPORT_PHANTOM StringHash
{
    friend struct StringWithHash;

public:
    StringHash() = default;
    explicit StringHash(StringView _string);
    explicit StringHash(uint64_t _hash);

    explicit operator uint64_t() const
    {
        return m_value;
    }
    uint64_t get() const
    {
        return m_value;
    }

    StringView debugString() const
    {
        return
#if !defined(PHANTOM_NO_STRINGHASH_STRING)
        m_pdebugString ? StringView(*m_pdebugString) :
#endif
                       StringView();
    }

    bool operator<(StringHash const& _other) const
    {
        return m_value < _other.m_value;
    }
    bool operator==(StringHash const& _other) const
    {
        return m_value == _other.m_value;
    }
    bool operator!=(StringHash const& _other) const
    {
        return m_value != _other.m_value;
    }

    bool isNull() const
    {
        return m_value == 0;
    }
    bool isValid() const
    {
        return m_value != 0;
    }

private:
    uint64_t m_value = 0;

#if !defined(PHANTOM_NO_STRINGHASH_STRING)
    String* m_pdebugString = nullptr;
#endif
};
} // namespace phantom

namespace std
{
template<>
struct hash<phantom::StringHash>
{
    size_t operator()(phantom::StringHash const& _hash)
    {
        return _hash.get();
    }
};
} // namespace std
