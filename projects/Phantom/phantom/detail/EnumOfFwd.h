#pragma once

#include <phantom/detail/TypeOfFwd.h>

#define PHANTOM_ENUMOF(...) _PHNTM_TYPEOF_CHECK(enum, Enum, __VA_ARGS__)
