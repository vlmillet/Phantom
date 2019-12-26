#pragma once

#include "StringView.h"
#include "typedefs.h"

namespace phantom
{
typedef hash64 StringHash;

PHANTOM_EXPORT_PHANTOM StringHash makeStringHash(StringView);
} // namespace phantom
