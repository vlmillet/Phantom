// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

#if !PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE

#    if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO

namespace std
{
namespace detail
{
template<typename _B1, typename _B2>
struct __and_ : public conditional<_B1::value, _B2, _B1>::type
{
};

template<typename _Pp>
struct __not_ : public std::integral_constant<bool, !_Pp::value>
{
};

template<typename _Tp>
struct __is_array_known_bounds : public std::integral_constant<bool, (extent<_Tp>::value > 0)>
{
};

template<typename _Tp>
struct __is_array_unknown_bounds : public __and_<is_array<_Tp>, __not_<extent<_Tp>>>::type
{
};

struct __do_is_default_constructible_impl
{
    template<typename _Tp>
    static true_type __test(int, decltype(_Tp())* a = nullptr);

    template<typename>
    static false_type __test(...);
};

template<typename _Tp>
struct __is_default_constructible_impl : public __do_is_default_constructible_impl
{
    typedef decltype(__test<_Tp>(0)) type;
};

template<typename _Tp>
struct __is_default_constructible_atom
    : public __and_<__not_<IsVoid<_Tp>>, __is_default_constructible_impl<_Tp>>::type
{
};

template<typename _Tp, bool = is_array<_Tp>::value>
struct __is_default_constructible_safe;

// The following technique is a workaround for a current core language
// restriction, which does not allow for array types to occur in
// functional casts of the form T().  Complete arrays can be default-
// constructed, if the element type is default-constructible, but
// arrays with unknown bounds are not.
template<typename _Tp>
struct __is_default_constructible_safe<_Tp, true>
    : public __and_<__is_array_known_bounds<_Tp>,
                    __is_default_constructible_atom<typename remove_all_extents<_Tp>::type>>::type
{
};

template<typename _Tp>
struct __is_default_constructible_safe<_Tp, false>
    : public __is_default_constructible_atom<_Tp>::type
{
};

} // namespace detail

/**
 * \struct IsDefaultConstructible
 *
 * \brief Tells if the class has a default constructor or not (doesn't take into account the
 * accessibility).
 *
 */

template<typename _Tp>
struct IsDefaultConstructible
    : public std::integral_constant<bool, (detail::__is_default_constructible_safe<_Tp>::value)>
{
};

} // namespace std

#    elif PHANTOM_COMPILER == PHANTOM_COMPILER_GCC // PHANTOM_COMPILER

namespace std
{
template<class T, bool t_is_class>
struct ____is_default_constructible_protected_skipper_helper
{
    class type : public T
    {
    };
};

template<class T>
struct ____is_default_constructible_protected_skipper_helper<T, false>
{
    typedef T type;
};

template<class T>
struct ____is_default_constructible_protected_skipper
    : public ____is_default_constructible_protected_skipper_helper<T, std::is_class<T>::value>
{
};

template<class T>
class is_default_constructible
{
    template<int x>
    class receive_size
    {
    };

    template<class U>
    static int sfinae(receive_size<sizeof U()>*);

    template<class U>
    static char sfinae(...);

public:
    enum
    {
        value = sizeof(sfinae<typename ____is_default_constructible_protected_skipper<T>::type>(
                0)) == sizeof(int)
    };
};
} // namespace std

#    else
#        error std::is_default_constructible not available for the current compiler/version, you must implement your own here (or try activating your compiler CPP0X features)
#    endif // PHANTOM_COMPILER

#else
#    include <type_traits>
#endif // PHANTOM_STD_TYPE_TRAIT_DEFINED_IS_DEFAULT_CONSTRUCTIBLE

namespace phantom
{
template<typename t_Ty>
using IsDefaultConstructible = std::is_default_constructible<t_Ty>;

template<typename T>
struct EmptyDerived : public T
{
};

template<typename T, bool is_final>
struct GetEmptyDerived
{
    using type = T;
};

template<typename T>
struct GetEmptyDerived<T, false>
{
    using type = EmptyDerived<T>;
};

namespace detail
{
template<typename T, bool is_class>
struct IsProtectedDefaultConstructibleH
    : public std::integral_constant<bool,
                                    !std::is_default_constructible<T>::value &&
                                    std::is_default_constructible<EmptyDerived<T>>::value>
{
};
template<typename T>
struct IsProtectedDefaultConstructibleH<T, false> : public std::false_type
{
};

template<typename T, bool is_class>
struct IsPublicOrProtectedDefaultConstructibleH
    : public std::is_default_constructible<
      typename GetEmptyDerived<T, std::is_final<T>::value>::type>
{
};
template<typename T>
struct IsPublicOrProtectedDefaultConstructibleH<T, false> : public std::is_default_constructible<T>
{
};

} // namespace detail

template<typename T>
struct IsProtectedDefaultConstructible
    : public detail::IsProtectedDefaultConstructibleH<T, std::is_class<T>::value>

{
};

template<typename T>
struct IsPublicOrProtectedDefaultConstructible
    : public detail::IsPublicOrProtectedDefaultConstructibleH<T, std::is_class<T>::value>
{
};

} // namespace phantom

/// @endcond
