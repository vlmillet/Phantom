// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

namespace phantom
{
template<typename T>
struct remove_all_pointers
{
    typedef T type;
};

template<typename T>
struct remove_all_pointers<T*>
{
    typedef typename remove_all_pointers<T>::type type;
};

template<typename T>
struct remove_all_pointers<T* const>
{
    typedef typename remove_all_pointers<T>::type type;
};

template<typename T>
struct remove_all_pointers<T* const volatile>
{
    typedef typename remove_all_pointers<T>::type type;
};

template<typename T>
struct remove_all_pointers<T* volatile>
{
    typedef typename remove_all_pointers<T>::type type;
};

} // namespace phantom
