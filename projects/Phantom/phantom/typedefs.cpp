// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "StringWithHash.h"
#include "phantom.h"

namespace phantom
{
StringWithHash::StringWithHash(String str) : hash(makeStringHash(str)), str(str)
{
}
StringWithHash::StringWithHash(StringView str) : hash(makeStringHash(str)), str(str)
{
}
StringWithHash::StringWithHash(const char* str) : hash(makeStringHash(StringView(str))), str(str)
{
}
} // namespace phantom
