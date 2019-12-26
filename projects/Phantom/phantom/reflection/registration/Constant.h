#pragma once

#include "Variable.h"

#include <phantom/reflection/ConstantT.h>

#define PHANTOM_CONSTANT(Name)                                                                                         \
    PHANTOM_REGISTER(Variables)                                                                                        \
    {                                                                                                                  \
        (this_()).constant(PHANTOM_PP_QUOTE(Name), Name);                                                              \
    }                                                                                                                  \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1
#define PHANTOM_MCONSTANT(Name) (this_()).constant(#Name, PHANTOM_SCOPE::Name)
#define PHANTOM_T_MCONSTANT(Name) (this_()).PHANTOM_T constant(#Name, PHANTOM_SCOPE::Name)
#define PHANTOM_STATIC_CONSTANT PHANTOM_MCONSTANT
#define PHANTOM_T_STATIC_CONSTANT PHANTOM_T_MCONSTANT
