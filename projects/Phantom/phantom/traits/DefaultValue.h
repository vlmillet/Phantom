// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <type_traits>

/// @cond ADVANCED
namespace phantom
{
template<typename T, bool t_is_arithmetic>
struct DefaultValueArithmetic
{
    static T apply()
    {
        return T(0);
    }
};

template<typename T>
struct DefaultValueArithmetic<T, false>
{
    static T apply()
    {
        return T();
    }
};

template<typename T>
struct DefaultValue : public DefaultValueArithmetic<T, std::is_arithmetic<T>::value>
{
};

template<typename T>
struct DefaultValue<T&>
{
    static T& apply()
    {
        return *(T*)0;
    }
};

template<typename T>
struct DefaultValue<T*>
{
    static T* apply()
    {
        return nullptr;
    }
};

template<typename T, size_t count>
struct DefaultValue<T[count]>
{
    static T* apply()
    {
        return nullptr;
    }
};

template<typename T>
struct DefaultValue<T&&>
{
    static T&& apply()
    {
        return std::move(T());
    }
};

#define PHANTOM_DEFINE_DEFAULT_VALUE(type, value)                                                  \
    namespace phantom                                                                              \
    {                                                                                              \
    template<>                                                                                     \
    struct DefaultValue<type>                                                                      \
    {                                                                                              \
        static type apply()                                                                        \
        {                                                                                          \
            return value;                                                                          \
        }                                                                                          \
    };                                                                                             \
    }

#define PHANTOM_DEFINE_DEFAULT_VALUE_T(types, args, type, ...)                                     \
    namespace phantom                                                                              \
    {                                                                                              \
    template<PHANTOM_PP_MIX(types, args)>                                                          \
    struct DefaultValue<type<PHANTOM_PP_IDENTITY args>>                                            \
    {                                                                                              \
        static type<PHANTOM_PP_IDENTITY args> apply()                                              \
        {                                                                                          \
            return __VA_ARGS__;                                                                    \
        }                                                                                          \
    };                                                                                             \
    }

} // namespace phantom

/// @endcond
