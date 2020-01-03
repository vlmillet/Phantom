#pragma once

#include <phantom/detail/TypeOfFwd.h>

#define PHANTOM_UNIONOF(...) _PHNTM_TYPEOF_CHECK(union, Union, __VA_ARGS__)
