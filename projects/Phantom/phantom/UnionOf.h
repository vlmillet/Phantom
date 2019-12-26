#pragma once

#include <phantom/TypeOf.h>

#define PHANTOM_UNIONOF(...) _PHNTM_TYPEOF_CHECK(union, Union, __VA_ARGS__)
