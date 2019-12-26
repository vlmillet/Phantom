#pragma once

#include <phantom/TypeOf.h>

#define PHANTOM_ENUMOF(...) _PHNTM_TYPEOF_CHECK(enum, Enum, __VA_ARGS__)
