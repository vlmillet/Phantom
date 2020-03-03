// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "StringHash.h"

#include "SmallMap.h"

#include <phantom/thread/SpinMutex.h>

#if PHANTOM_STRINGHASH_CHECK_COLLISIONS
static SmallMap<hash64, String> s_DebugCheckCollisions;
auto                            found = s_DebugCheckCollisions.find(result);
if (found != s_DebugCheckCollisions.end())
{
    PHANTOM_ASSERT(found->second == String(a_Str, a_Len), "hash collision detected between '%s' and '%s'",
                   found->second.c_str(), String(a_Str, a_Len).c_str());
}
else
{
    s_DebugCheckCollisions[result] = String(a_Str, a_Len);
}
#endif
namespace phantom
{
namespace
{
#if PHANTOM_STRINGHASH_CHECK_COLLISIONS
phantom::SpinMutex              g_spinMutex;
SmallMap<uint64_t, String, 128> g_hashToString;
#endif
} // namespace

StringHash::StringHash(StringView _str) : m_value(makeStringHash(phantom::StringView(_str.data(), _str.size())))
{
#if defined(PHANTOM_STRINGHASH_CHECK_COLLISIONS)
    auto lock = g_spinMutex.autoLock();
    {
        auto found = g_hashToString.find(m_value);
        if (found == g_hashToString.end())
        {
            auto& str = g_hashToString[m_value] = String(_str);
            m_pdebugString = &str;
        }
        else
        {
            PHANTOM_ASSERT(found->second == _str);
            m_pdebugString = &found->second;
        }
    }
#endif
}

StringHash::StringHash(uint64_t _hash) : m_value(_hash)
{
#if defined(PHANTOM_STRINGHASH_CHECK_COLLISIONS)
    auto lock = g_spinMutex.autoLock();
    {
        auto found = g_hashToString.find(m_value);
        if (found != g_hashToString.end())
        {
            m_pdebugString = &found->second;
        }
    }
#endif
}

} // namespace phantom
