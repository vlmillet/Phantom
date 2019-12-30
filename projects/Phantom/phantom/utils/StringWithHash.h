#pragma once

#include "String.h"
#include "StringHash.h"
#include "StringView.h"

namespace phantom
{
struct PHANTOM_EXPORT_PHANTOM StringWithHash
{
public:
    StringWithHash() = default;
    StringWithHash(StringWithHash const& _other)
        : m_hash(_other.m_hash),
#if defined(PHANTOM_NO_STRINGHASH_STRING)
          m_string(_other.getString()),
          m_pString(&m_string)
#else
          m_pString(m_hash.m_pdebugString)
#endif
    {
    }
    StringWithHash& operator=(StringWithHash const& _other)
    {
        m_hash = _other.m_hash;
#if defined(PHANTOM_NO_STRINGHASH_STRING)
        m_string = _other.getString();
        m_pString = &m_string;
#else
        m_pString = m_hash.m_pdebugString;
#endif
        return *this;
    }

    explicit StringWithHash(StringView _string)
        : m_hash(_string),
#if defined(PHANTOM_NO_STRINGHASH_STRING)
          m_string(_string),
          m_pString(&m_string)
#else
          m_pString(m_hash.m_pdebugString)
#endif
    {
    }

    explicit operator uint64_t() const
    {
        return getHash().get();
    }

    StringHash getHash() const
    {
        return m_hash;
    }

    uint64_t getHashValue() const
    {
        return m_hash.get();
    }

    StringView getString() const
    {
        return *m_pString;
    }

    bool operator<(StringWithHash const& _other) const
    {
        return m_hash < _other.m_hash;
    }
    bool operator==(StringWithHash const& _other) const
    {
        return m_hash == _other.m_hash;
    }
    bool operator!=(StringWithHash const& _other) const
    {
        return m_hash != _other.m_hash;
    }

    bool isNull() const
    {
        return m_hash.isNull();
    }
    bool isValid() const
    {
        return m_hash.isValid();
    }

private:
    StringHash m_hash;
#if defined(PHANTOM_NO_STRINGHASH_STRING)
    String  m_string;
    String* m_pString = &m_string;
#else
    String* m_pString = m_hash.m_pdebugString;
#endif
};
} // namespace phantom

namespace std
{
template<>
struct hash<phantom::StringWithHash>
{
    size_t operator()(phantom::StringWithHash const& _hash)
    {
        return _hash.getHashValue();
    }
};
} // namespace std
