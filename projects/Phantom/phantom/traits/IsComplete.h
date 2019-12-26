// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/// @cond ADVANCED

namespace phantom
{
#if defined(_MSC_VER)
template<class T, int discriminator>
struct IsComplete
{
    static T& getT();
    static char (&pass(T))[2];
    static char       pass(...);
    static const bool value = sizeof(pass(getT())) == 2;
};
#else
template<class T, int discriminator>
struct IsComplete
{
    typedef char false_type;
    typedef int  true_type;
    template<class T1>
    static typename std::enable_if<sizeof(T1), true_type>::type test(int);
    template<class T1>
    static false_type test(...);
    enum ENU
    {
        value = (sizeof(test<T>(0)) == sizeof(true_type))
    };
};
#endif
} // namespace phantom

/// @endcond
