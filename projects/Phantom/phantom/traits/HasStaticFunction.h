// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#define PHANTOM_DEFINE_HAS_STATIC_FUNCTION(traitsName, funcName, signature)                        \
    template<typename U>                                                                           \
    class traitsName                                                                               \
    {                                                                                              \
    private:                                                                                       \
        template<typename T, T>                                                                    \
        struct helper;                                                                             \
        template<typename T>                                                                       \
        static char check(helper<signature, &T::funcName>*);                                       \
        template<typename T>                                                                       \
        static short check(...);                                                                   \
                                                                                                   \
    public:                                                                                        \
        static constexpr bool value = sizeof(check<U>(0)) == sizeof(char);                         \
    }
