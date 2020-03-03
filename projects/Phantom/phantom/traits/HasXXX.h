// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#define PHANTOM_DEFINE_HAS_TYPEDEF(name)                                                           \
    template<typename T>                                                                           \
    struct Has_##name                                                                              \
    {                                                                                              \
    private:                                                                                       \
        typedef char yes;                                                                          \
        typedef struct                                                                             \
        {                                                                                          \
            char array[2];                                                                         \
        } no;                                                                                      \
                                                                                                   \
        template<typename C>                                                                       \
        static yes test(typename C::name*);                                                        \
        template<typename C>                                                                       \
        static no test(...);                                                                       \
                                                                                                   \
    public:                                                                                        \
        static const bool value = sizeof(test<T>(0)) == sizeof(yes);                               \
    };
