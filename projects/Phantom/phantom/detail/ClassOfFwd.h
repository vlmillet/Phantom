#pragma once

#include <phantom/detail/TypeOfFwd.h>

#define PHANTOM_CLASSOF(...) _PHNTM_TYPEOF_CHECK(class, Class, __VA_ARGS__)
