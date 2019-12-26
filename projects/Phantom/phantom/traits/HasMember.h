// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#define PHANTOM_DEFINE_HAS_MEMBER(name)                                                            \
    template<typename T>                                                                           \
    struct HasMember_##name                                                                        \
    {                                                                                              \
    private:                                                                                       \
        typedef char yes;                                                                          \
        typedef long no;                                                                           \
                                                                                                   \
        template<typename TT>                                                                      \
        static yes test(decltype(&TT::name));                                                      \
        template<typename TT>                                                                      \
        static no test(...);                                                                       \
                                                                                                   \
    public:                                                                                        \
        enum                                                                                       \
        {                                                                                          \
            value = sizeof(test<T>(0)) == sizeof(yes)                                              \
        };                                                                                         \
    };
