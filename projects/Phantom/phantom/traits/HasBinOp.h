#pragma once

#include <haunt>

HAUNT_STOP;

#include "ImplicitConv.h"

#include <type_traits>

#define PHANTOM_MAKE_HAS_BIN_OP(name, op)                                                                              \
    namespace _##name                                                                                                  \
    {                                                                                                                  \
        template<class T, class = decltype(std::declval<T>() op std::declval<T>())>                                    \
        std::true_type Select(T*);                                                                                     \
                                                                                                                       \
        template<class T>                                                                                              \
        std::false_type Select(phantom::ImplicitConv<T*>);                                                             \
    }                                                                                                                  \
    template<class T>                                                                                                  \
    struct name : decltype(_##name::Select<T>(static_cast<T*>(nullptr)))                                               \
    {                                                                                                                  \
    };
